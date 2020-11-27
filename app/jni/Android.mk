# Set JNI local path
LOCAL_PATH := $(call my-dir)

# Clear LOCAL variables
include $(CLEAR_VARS)

# C++ Settings
LOCAL_CPP_EXTENSION := .cpp
LOCAL_CPPFLAGS := -std=c++14
LOCAL_CFLAGS := -Wall -Wextra

# NDK_BUILD Build Settings
LOCAL_MODULE := CppAndroidEngineJNI

# NDK_BUILD Link Settings
LOCAL_LDLIBS := -landroid -llog -lEGL -lGLESv2 -lOpenSLES

# C++ Paths
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../src/main/cpp/Engine/ \
                    $(LOCAL_PATH)/../src/main/cpp/ThirdParty/

LOCAL_SRC_FILES := $(LOCAL_PATH)/../src/main/cpp/Engine/utils.cpp \
                   $(LOCAL_PATH)/../src/main/cpp/Engine/clock.cpp \
                   $(LOCAL_PATH)/../src/main/cpp/Engine/touchscreen.cpp \
                   $(LOCAL_PATH)/../src/main/cpp/Engine/graphics_context.cpp \
                   $(LOCAL_PATH)/../src/main/cpp/Engine/asset_manager.cpp \
                   $(LOCAL_PATH)/../src/main/cpp/Engine/asset.cpp \
                   $(LOCAL_PATH)/../src/main/cpp/Engine/shader_compiler.cpp \
                   $(LOCAL_PATH)/../src/main/cpp/Engine/vertex_buffer.cpp \
                   $(LOCAL_PATH)/../src/main/cpp/Engine/math.cpp \
                   $(LOCAL_PATH)/../src/main/cpp/main.cpp

# Build as shared library
include $(BUILD_SHARED_LIBRARY)
