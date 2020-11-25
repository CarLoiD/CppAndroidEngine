#ifndef MAIN_H
#define MAIN_H

#define _DEBUG_ASSERT_ENABLED 1

#include "Engine/utils.h"
#include "Engine/clock.h"
#include "Engine/touchscreen.h"

// JNI
#include <jni.h>

// GLES
#include <GLES2/gl2.h>

// C++
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <sstream>

Clock g_mainClock;
TouchScreen g_displayInput;

namespace Application
{
    void Create();
    void Update(const float deltaTime);
    void Destroy();
}

extern "C" JNIEXPORT void JNICALL
Java_com_carloid_cppandroidengine_EngineGLRenderer_ApplicationCreate(JNIEnv* env, jobject obj, jint width, jint height)
{
    g_displayInput.Create((uint32_t*)env, width, height);
    Application::Create();
}

extern "C" JNIEXPORT void JNICALL
Java_com_carloid_cppandroidengine_EngineGLRenderer_ApplicationUpdate(JNIEnv* env, jobject obj)
{
    float deltaTime = g_mainClock.GetElapsedTime();
    g_mainClock.Restart();

    Application::Update(deltaTime);
}

extern "C" JNIEXPORT void JNICALL
Java_com_carloid_cppandroidengine_MainActivity_ApplicationDestroy(JNIEnv* env, jobject obj)
{
    Application::Destroy();
}

#endif // MAIN_H