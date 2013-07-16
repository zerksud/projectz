/*
 * AndroidLayer.cpp
 *
 *  Created on: Mar 13, 2013
 *      Author: zerksud
 */

#include "AndroidLayer.h"

#include <stddef.h>

#include <android/asset_manager_jni.h>

namespace ann {
namespace android {

AndroidLayer& AndroidLayer::getInstance() {
    static AndroidLayer instance;
    return instance;
}

AndroidLayer::AndroidLayer() {
    mJavaVM = nullptr;
    mAssetManagerGlobalRef = nullptr;
    mAAssetManager = nullptr;
    mAssetManager = new AssetManager();

    mNativeProxy = nullptr;
}

void AndroidLayer::postNotification(const std::string& name, const std::string& dict) {
    JNIEnv *jniEnv = nullptr;
    mJavaVM->AttachCurrentThread(&jniEnv, nullptr);

    jstring nameJavaString = jniEnv->NewStringUTF(name.c_str());
    jstring dictJavaString = jniEnv->NewStringUTF(dict.c_str());

    jniEnv->CallVoidMethod(mNativeProxy, mPostNotificationMethodID, nameJavaString, dictJavaString);

    jniEnv->DeleteLocalRef(nameJavaString);
    jniEnv->DeleteLocalRef(dictJavaString);

    mJavaVM->DetachCurrentThread();
}

AndroidLayer::~AndroidLayer() {
    JNIEnv *jniEnv = nullptr;
    mJavaVM->AttachCurrentThread(&jniEnv, nullptr);

    if (mAssetManagerGlobalRef != nullptr) {
        jniEnv->DeleteGlobalRef(mAssetManagerGlobalRef);
    }

    if (mNativeProxy != nullptr) {
        jniEnv->DeleteGlobalRef(mNativeProxy);
    }

    mJavaVM->DetachCurrentThread();

    delete mAssetManager;
}

void AndroidLayer::setJavaVM(JavaVM *javaVM) {
    mJavaVM = javaVM;
}

AAssetManager* AndroidLayer::getAAssetManager() const {
    return mAAssetManager;
}

void AndroidLayer::setAAssetManager(jobject assetManagerGlobalRef) {
    JNIEnv *jniEnv = nullptr;
    mJavaVM->AttachCurrentThread(&jniEnv, nullptr);

    if (mAssetManagerGlobalRef != nullptr) {
        jniEnv->DeleteGlobalRef(mAssetManagerGlobalRef);
    }

    mAssetManagerGlobalRef = assetManagerGlobalRef;
    mAAssetManager = AAssetManager_fromJava(jniEnv, mAssetManagerGlobalRef);

    mJavaVM->DetachCurrentThread();
}

void AndroidLayer::setNativeProxy(jobject nativeProxy) {
    JNIEnv *jniEnv = nullptr;
    mJavaVM->AttachCurrentThread(&jniEnv, nullptr);

    if (mNativeProxy != nullptr) {
        jniEnv->DeleteGlobalRef(mNativeProxy);
    }

    mNativeProxy = nativeProxy;
    jclass mNativeProxyClass = jniEnv->GetObjectClass(mNativeProxy);
    mPostNotificationMethodID = jniEnv->GetMethodID(mNativeProxyClass, "postNotification", "(Ljava/lang/String;Ljava/lang/String;)V");
    jniEnv->DeleteLocalRef(mNativeProxyClass);

    mJavaVM->DetachCurrentThread();
}

AssetManager* AndroidLayer::getAssetManager() const {
    return mAssetManager;
}

} /* namespace android*/
} /* namespace ann */
