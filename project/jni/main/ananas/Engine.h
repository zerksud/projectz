/*
 * Engine.h
 *
 *  Created on: Mar 6, 2013
 *      Author: zerksud
 */

#ifndef ENGINE_H_
#define ENGINE_H_

#include <ananas/util/Thread.h>

#include "gles/GLESLayer.h"
#include "anim/AnimationManager.h"

#include "engine/UserData.h"
#include "engine/UpdateManager.h"
#include "engine/FPSCalculator.h"
#include "engine/NotificationProxy.h"

extern void ananas_startup();

namespace ann {

class Engine {
public:
    static Engine& getInstance();

    util::Thread* getGLThread();
    gles::GLESLayer* getGLESLayer();

    void setUserData(UserData* data);
    UserData* getUserData();

    UpdateManager* getUpdateManaget();
    anim::AnimationManager* getAnimationManager();

    void setWindow(ANativeWindow* window);

    void importNotification(const std::string& name, const std::string& dict);

private:
    Engine();
    Engine(Engine& other);
    virtual ~Engine();
    const Engine& operator=(const Engine& other);

    void onCreate();
    void onResume();
    void onPause();
    void onStop();

    void resizeWindow(int width, int height);
    void releaseWindow();

    gles::GLESLayer *mGLESLayer;
    util::Thread *mGLThread;

    bool mAnanasStartUpCalled;

    UserData* mUserData;

    UpdateManager* mUpdateManager;
    anim::AnimationManager* mAnimationManager;
    FPSCalculator* mFPSCalculator;

    NotificationProxy* mNotificationProxy;
};

} /* namespace ann */

#endif /* ENGINE_H_ */
