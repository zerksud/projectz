LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE	:= jsoncpp
FILE_LIST := $(shell find $(LOCAL_PATH) -regex .*\.cpp)
LOCAL_C_INCLUDES := jni/lib/
LOCAL_CPPFLAGS  := -fexceptions
LOCAL_SRC_FILES	:= $(FILE_LIST:$(LOCAL_PATH)/%=%)

include $(BUILD_STATIC_LIBRARY)
