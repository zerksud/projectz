LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE	:= main
FILE_LIST := $(shell find $(LOCAL_PATH) -regex .*\.cpp)
LOCAL_SRC_FILES	:= $(FILE_LIST:$(LOCAL_PATH)/%=%)
LOCAL_C_INCLUDES := jni/lib/ jni/main
LOCAL_CPPFLAGS	:= -std=c++11 -Werror -Wall -Wextra -Wno-unused-parameter -Wswitch-default
LOCAL_LDLIBS	:= -llog -landroid -lEGL -lGLESv2 -lz
LOCAL_STATIC_LIBRARIES := png jsoncpp

include $(BUILD_SHARED_LIBRARY)
