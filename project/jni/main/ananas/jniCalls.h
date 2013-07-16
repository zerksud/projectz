/*
 * jniCalls.h
 *
 *  Created on: Mar 13, 2013
 *      Author: zerksud
 */

#ifndef JNICALLS_H_
#define JNICALLS_H_

#include <jni.h>

extern "C" {

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved);

JNIEXPORT void JNICALL Java_com_gmail_zerksud_projectz_general_NativeProxy_nativeSetSurface(JNIEnv* jenv, jobject obj, jobject surface);

JNIEXPORT void JNICALL Java_com_gmail_zerksud_projectz_general_NativeProxy_nativeSetAssetManager(JNIEnv* jenv, jobject obj, jobject assetManager);

JNIEXPORT void JNICALL Java_com_gmail_zerksud_projectz_general_NativeProxy_nativeSetNativeProxy(JNIEnv* jenv, jobject obj);

JNIEXPORT void JNICALL Java_com_gmail_zerksud_projectz_general_NativeProxy_nativeProxyNotification(JNIEnv* jenv, jobject obj, jstring name, jstring dict);

}

#endif /* JNICALLS_H_ */
