/*
 * GLESLayer.h
 *
 *  Created on: Mar 6, 2013
 *      Author: zerksud
 */

#ifndef GLESLAYER_H_
#define GLESLAYER_H_

#include <EGL/egl.h>

#include "resources/ShaderManager.h"
#include "resources/TextureManager.h"
#include "resources/MeshManager.h"
#include "SceneManager.h"

namespace ann {
namespace gles {

class GLESLayer {
public:
    GLESLayer();
    virtual ~GLESLayer();

    void initGL(ANativeWindow *window);
    void resizeWindow(int width, int height);
    void destroyContext();

    void render();
    void enableRender();
    void disableRender();
    bool renderEnabled() const;

    bool glContextIsActive() const;

    ShaderManager* getShaderManager() const;
    TextureManager* getTextureManager() const;
    MeshManager* getMeshManager() const;
    SceneManager* getSceneManager() const;

private:
    void reloadResources();

    ANativeWindow* mWindow;
    EGLDisplay mDisplay;
    EGLSurface mSurface;
    EGLContext mContext;
    bool mRenderEnabled;
    bool mInitialized;

    ShaderManager* mShaderManager;
    TextureManager* mTextureManager;
    MeshManager* mMeshManager;
    SceneManager* mSceneManager;
};

} /* namespace gles */
} /* namespace ann */
#endif /* GLESLAYER_H_ */
