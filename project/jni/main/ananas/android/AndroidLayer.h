/*
 * AndroidLayer.h
 *
 *  Created on: Mar 13, 2013
 *      Author: zerksud
 */

#ifndef ANDROIDLAYER_H_
#define ANDROIDLAYER_H_

#include <string>

#include <jni.h>
#include <android/asset_manager.h>

#include "AssetManager.h"

namespace ann {
namespace android {

class AndroidLayer {
public:
    static AndroidLayer& getInstance();

    void setJavaVM(JavaVM *javaVM);

    AAssetManager* getAAssetManager() const;
    void setAAssetManager(jobject assetManagerGlobalRef);

    void setNativeProxy(jobject nativeProxy);

    AssetManager* getAssetManager() const;

    void postNotification(const std::string& name, const std::string& dict);

private:
    AndroidLayer();
    AndroidLayer(AndroidLayer& other);
    virtual ~AndroidLayer();
    AndroidLayer& operator=(const AndroidLayer& other);

    JavaVM *mJavaVM;

    AAssetManager *mAAssetManager;
    jobject mAssetManagerGlobalRef;

    AssetManager *mAssetManager;

    jobject mNativeProxy;
    jmethodID mPostNotificationMethodID;
};

} /* namespace android*/
} /* namespace ann */
#endif /* ANDROIDLAYER_H_ */
