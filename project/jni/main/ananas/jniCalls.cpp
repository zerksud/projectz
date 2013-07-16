/*
 * jniCalls.cpp
 *
 *  Created on: Mar 13, 2013
 *      Author: zerksud
 */

#include "jniCalls.h"

#include <android/native_window_jni.h>

#include <ananas/Engine.h>

#include <ananas/android/AndroidLayer.h>

#include <ananas/util/ZLogger.h>

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    ann::android::AndroidLayer::getInstance().setJavaVM(vm);

    return JNI_VERSION_1_6;
}

// TODO: JNI_OnUnload to delete global references?

JNIEXPORT void JNICALL Java_com_gmail_zerksud_projectz_general_NativeProxy_nativeSetSurface(JNIEnv* jenv, jobject obj, jobject surface) {
    ANativeWindow *window = ANativeWindow_fromSurface(jenv, surface);

    ann::Engine::getInstance().getGLThread()->addTask([window]() {
        ann::Engine::getInstance().setWindow(window);
    });
}

JNIEXPORT void JNICALL Java_com_gmail_zerksud_projectz_general_NativeProxy_nativeSetAssetManager(JNIEnv* jenv, jobject obj, jobject assetManager) {
    jobject assetManagerGlobalRef = jenv->NewGlobalRef(assetManager);

    ann::Engine::getInstance().getGLThread()->addTask([assetManagerGlobalRef]() {
        ann::android::AndroidLayer::getInstance().setAAssetManager(assetManagerGlobalRef);
    });
}

void Java_com_gmail_zerksud_projectz_general_NativeProxy_nativeSetNativeProxy(JNIEnv* jenv, jobject obj) {
    jobject nativeProxyGlobalRef = jenv->NewGlobalRef(obj);

    ann::Engine::getInstance().getGLThread()->addTask([nativeProxyGlobalRef]() {
        ann::android::AndroidLayer::getInstance().setNativeProxy(nativeProxyGlobalRef);
    });
}

void Java_com_gmail_zerksud_projectz_general_NativeProxy_nativeProxyNotification(JNIEnv* jenv, jobject obj, jstring name, jstring dict) {
    const char* nameCStr = jenv->GetStringUTFChars(name, 0);
    const char* dictCStr = jenv->GetStringUTFChars(dict, 0);

    LOGD("Got notification %s: %s", nameCStr, dictCStr);
    std::string nameString = nameCStr;
    std::string dictString = dictCStr;

    ann::Engine::getInstance().getGLThread()->addTask([nameString, dictString]() {
        ann::Engine::getInstance().importNotification(nameString, dictString);
    });

    jenv->ReleaseStringUTFChars(name, nameCStr);
    jenv->ReleaseStringUTFChars(dict, dictCStr);
}
