#ifndef MAIN_H
#define MAIN_H

#define _DEBUG_ASSERT_ENABLED 1

#include "Engine/utils.h"

// JNI
#include <jni.h>

// GLES
#include <GLES2/gl2.h>

// C++
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

namespace Application
{
    static void Create();
    static void Update();
    static void Destroy();
}

extern "C" JNIEXPORT void JNICALL
Java_com_carloid_cppandroidengine_EngineGLRenderer_ApplicationCreate(JNIEnv* env, jobject obj) {
    Application::Create();
}

extern "C" JNIEXPORT void JNICALL
Java_com_carloid_cppandroidengine_EngineGLRenderer_ApplicationUpdate(JNIEnv* env, jobject obj) {
    Application::Update();
}

extern "C" JNIEXPORT void JNICALL
Java_com_carloid_cppandroidengine_MainActivity_ApplicationDestroy(JNIEnv* env, jobject obj) {
    Application::Destroy();
}

#endif // MAIN_H