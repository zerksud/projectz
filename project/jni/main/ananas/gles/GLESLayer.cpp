/*
 * GLESLayer.cpp
 *
 *  Created on: Mar 6, 2013
 *      Author: zerksud
 */

#include "GLESLayer.h"

#include <stddef.h>
#include <stdint.h>

#include <GLES2/gl2.h>

#include <glm/gtc/matrix_transform.hpp>

#include <ananas/Engine.h>

#include <ananas/util/ZLogger.h>
#include "Helpers.h"

namespace ann {
namespace gles {

GLESLayer::GLESLayer() {
    mWindow = nullptr;
    mDisplay = EGL_NO_DISPLAY;
    mSurface = EGL_NO_SURFACE;
    mContext = EGL_NO_CONTEXT;
    mRenderEnabled = false;
    mInitialized = false;

    mShaderManager = new ShaderManager();
    mTextureManager = new TextureManager();
    mMeshManager = new MeshManager();
    mSceneManager = new SceneManager();
}

GLESLayer::~GLESLayer() {
    delete mShaderManager;
    delete mTextureManager;
    delete mMeshManager;
}

void GLESLayer::initGL(ANativeWindow *window) {
    LOGV("GLESLayer::initGL");
    mWindow = window;
    ANativeWindow_acquire(mWindow);
    if (mDisplay != EGL_NO_DISPLAY ) {
        LOGE("Already got EGLDisplay");
        return;
    }

    mDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY );
    if (mDisplay == EGL_NO_DISPLAY ) {
        HANDLE_EGL_ERROR("Can't obtain the EGL display connection for the native display");
        return;
    }

    EGLBoolean success = eglInitialize(mDisplay, nullptr, nullptr);
    if (success == EGL_FALSE) {
        HANDLE_EGL_ERROR("Can't initialize the EGL display connection");
        return;
    }

    const EGLint config_attrib_list[] = {
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_RED_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_BLUE_SIZE, 8,
            EGL_DEPTH_SIZE, 8,
            EGL_NONE
    };
    //TODO: RGBA_4444

    EGLConfig config;
    EGLint numConfigs;
    success = eglChooseConfig(mDisplay, config_attrib_list, &config, 1, &numConfigs);
    if (success == EGL_FALSE) {
        HANDLE_EGL_ERROR("Can't choose any EGL frame buffer configuration that match specified attributes");
        //TODO: release allocated resources?
        return;
    }
    helpers::printEGLConfig(mDisplay, config);

    EGLint format;
    success = eglGetConfigAttrib(mDisplay, config, EGL_NATIVE_VISUAL_ID, &format);
    if (success == EGL_FALSE) {
        HANDLE_EGL_ERROR("Can't get the ID of the associated native visual");
        return;
    }

    ANativeWindow_setBuffersGeometry(mWindow, 0, 0, format);

    mSurface = eglCreateWindowSurface(mDisplay, config, window, nullptr);

    const EGLint context_attrib_list[] = {
            EGL_CONTEXT_CLIENT_VERSION, 2,
            EGL_NONE
    };
    mContext = eglCreateContext(mDisplay, config, nullptr, context_attrib_list);

    success = eglMakeCurrent(mDisplay, mSurface, mSurface, mContext);
    if (success == EGL_FALSE) {
        HANDLE_EGL_ERROR("Can't attach an EGL rendering context to EGL surfaces");
        return;
    }

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    LOGV("glContext created successfully");

    ann::Engine::getInstance().getGLThread()->addTask([this]() {
        reloadResources();
    });

    mInitialized = true;
}

void GLESLayer::reloadResources() {
    mShaderManager->glContextRecreated();
    mTextureManager->glContextRecreated();
    mMeshManager->glContextRecreated();
}

void GLESLayer::resizeWindow(int width, int height) {
    if (!mInitialized) {
        LOGE("Can't resize view port in uninitialized state");
        return;
    }

    glViewport(0, 0, width, height);

    glm::mat4 perspectiveMatrix = glm::perspective(60.0f, static_cast<float>(width) / height, 0.1f, 10.0f); // TODO: move far distance to config?
    mSceneManager->setProjectionMatrix(perspectiveMatrix);
}

void GLESLayer::destroyContext() {
    if (!mInitialized) {
        LOGE("Can't destroy not created context");
        return;
    }
    LOGV("GLESLayer::destroyContext");

    mShaderManager->glContextDestroyed();
    mTextureManager->glContextDestroyed();
    mMeshManager->glContextDestroyed();

    EGLBoolean success = eglMakeCurrent(mDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT );
    if (success == EGL_FALSE) {
        HANDLE_EGL_ERROR("Can't release an EGL rendering context");
    }

    success = eglDestroyContext(mDisplay, mContext);
    if (success == EGL_FALSE) {
        HANDLE_EGL_ERROR("Can't destroy an EGL rendering context");
    }
    mContext = EGL_NO_CONTEXT;

    success = eglDestroySurface(mDisplay, mSurface);
    if (success == EGL_FALSE) {
        HANDLE_EGL_ERROR("Can't destroy an EGL surface");
    }
    mSurface = EGL_NO_SURFACE;

    success = eglTerminate(mDisplay);
    if (success == EGL_FALSE) {
        HANDLE_EGL_ERROR("Can't terminate an EGL display connection");
    }
    mDisplay = EGL_NO_DISPLAY;

    ANativeWindow_release(mWindow);
    mWindow = nullptr;

    mInitialized = false;
}

void GLESLayer::render() {
    if (!mInitialized || !mRenderEnabled || mContext == EGL_NO_CONTEXT ) {    //TODO: use one flag somehow
        return;
    }
    //TODO: need some thread-safe check if we already lost context/window/smth, then render nothing
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mSceneManager->render();

    // TODO: eglWaitGL for some kind of vsync?
    EGLBoolean success = eglSwapBuffers(mDisplay, mSurface);
    if (success != EGL_TRUE) {
        HANDLE_EGL_ERROR("Swapping of the surface buffers failed");
    }
}

void GLESLayer::enableRender() {
    LOGV("Renderer enabled");
    mRenderEnabled = true;
}

void GLESLayer::disableRender() {
    LOGV("Renderer disabled");
    mRenderEnabled = false;
}

bool GLESLayer::renderEnabled() const {
    return mRenderEnabled;
}

bool GLESLayer::glContextIsActive() const {
    return mRenderEnabled;  //TODO: actual context?
}

ShaderManager* GLESLayer::getShaderManager() const {
    return mShaderManager;
}

TextureManager* GLESLayer::getTextureManager() const {
    return mTextureManager;
}

MeshManager* GLESLayer::getMeshManager() const {
    return mMeshManager;
}

SceneManager* GLESLayer::getSceneManager() const {
    return mSceneManager;
}

} /* namespace gles */
} /* namespace ann */
