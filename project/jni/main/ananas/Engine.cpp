/*
 * Engine.cpp
 *
 *  Created on: Mar 6, 2013
 *      Author: zerksud
 */

#include "Engine.h"

#include <ananas/util/Constants.h>
#include <ananas/util/ZLogger.h>

namespace ann {

Engine& Engine::getInstance() {
    static Engine instance;
    return instance;
}

Engine::Engine() {
    mGLESLayer = new gles::GLESLayer();
    mGLThread = new util::Thread();
    mGLThread->run();

    mAnanasStartUpCalled = false;

    mUserData = nullptr;

    mUpdateManager = new UpdateManager();

    mAnimationManager = new anim::AnimationManager();   //TODO: construct AnimationManager in glThread?
    mUpdateManager->addUpdatable(mAnimationManager);

    mFPSCalculator = new FPSCalculator();
    mUpdateManager->addUpdatable(mFPSCalculator);

    mNotificationProxy = new NotificationProxy();

    ann::util::NotificationCenter& nc = ann::util::NotificationCenter::getInstance();

    nc.addObserver(ann::util::Constants::kActivityOnCreate, this, [this](const ann::util::Dictionary& dict) {this->onCreate();});
    nc.addObserver(ann::util::Constants::kActivityOnResume, this, [this](const ann::util::Dictionary& dict) {this->onResume();});
    nc.addObserver(ann::util::Constants::kActivityOnPause, this, [this](const ann::util::Dictionary& dict) {this->onPause();});
    nc.addObserver(ann::util::Constants::kActivityOnStop, this, [this](const ann::util::Dictionary& dict) {this->onStop();});

    nc.addObserver(ann::util::Constants::kSurfaceSizeChanged, this, [this](const ann::util::Dictionary& dict) {
        int width = dict["width"].asInt();
        int height = dict["height"].asInt();
        this->resizeWindow(width, height);
    });
    nc.addObserver(ann::util::Constants::kSurfaceDestroyed, this, [this](const ann::util::Dictionary& dict) {this->releaseWindow();});
}

void Engine::setUserData(UserData* data) {
    mUserData = data;
}

UserData* Engine::getUserData() {
    return mUserData;
}

UpdateManager* Engine::getUpdateManaget() {
    return mUpdateManager;
}

anim::AnimationManager* Engine::getAnimationManager() {
    return mAnimationManager;
}

void Engine::importNotification(const std::string& name, const std::string& dict) {
    mNotificationProxy->importNotification(name, dict);
}

Engine::~Engine() {
    delete mGLESLayer;
    delete mGLThread;
    delete mUserData;   // TODO: release memory in glThread?
    delete mUpdateManager;
    delete mAnimationManager;
    delete mFPSCalculator;
    delete mNotificationProxy;
}

util::Thread* Engine::getGLThread() {
    return mGLThread;
}

gles::GLESLayer* Engine::getGLESLayer() {
    return mGLESLayer;
}

void Engine::onCreate() {
    LOGV("Engine::onCreate");

    if (!mAnanasStartUpCalled) {
        ananas_startup();
        mAnanasStartUpCalled = true;
    }
}

void Engine::onResume() {
    LOGV("onResume");

    mGLESLayer->enableRender();

    gles::GLESLayer *glLayer = mGLESLayer;
    UpdateManager* updateManager = mUpdateManager;
    mGLThread->addRecurringTask([updateManager, glLayer]() {
        updateManager->update();
        glLayer->render();

        bool repeat = glLayer->renderEnabled();

        return repeat;
    });
}

void Engine::onPause() {
    LOGV("Engine::onPause");
    mGLESLayer->disableRender();
}

void Engine::onStop() {
    LOGV("Engine::onStop");
}

void Engine::setWindow(ANativeWindow* window) {
    LOGV("setWindow");
    mGLESLayer->initGL(window);
}

void Engine::resizeWindow(int width, int height) {
    LOGV("resizeWindow %dx%d", width, height);
    mGLESLayer->resizeWindow(width, height);
}

void Engine::releaseWindow() {
    LOGV("Engine::releaseWindow");
    mGLESLayer->destroyContext();
}

} /* namespace ann */
