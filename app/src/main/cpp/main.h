#ifndef MAIN_H
#define MAIN_H

// Engine
#include "Engine/utils.h"
#include "Engine/clock.h"
#include "Engine/touchscreen.h"
#include "Engine/graphics_context.h"
#include "Engine/asset_manager.h"
#include "Engine/vector.h"
#include "Engine/matrix.h"

// JNI
#include <jni.h>
#include <android/asset_manager_jni.h>

// GLES2
#include <GLES2/gl2.h>

// C++
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <sstream>

// Subsystems
Clock g_mainClock;
TouchScreen g_displayInput;
GraphicsContext g_gfxContext;
AssetManager g_assetManager;

namespace Application
{
    void Create();
    void Update(const float deltaTime);
    void Destroy();
}

extern "C" JNIEXPORT void JNICALL
Java_com_carloid_cppandroidengine_EngineGLRenderer_ApplicationCreate(JNIEnv* env, jobject obj, jint width, jint height, jobject assetManager)
{
    g_displayInput.Create((uint32_t*)env, width, height);
    g_gfxContext.Create(width, height);

    AAssetManager* assetManagerPtr = AAssetManager_fromJava(env, assetManager);
    g_assetManager.Create(assetManagerPtr);

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

// Inline function aliases

static inline float GetTouchScreenX(const TouchScreenId& id)
{
    return g_displayInput.GetTouchScreenX(id);
}

static inline float GetTouchScreenY(const TouchScreenId& id)
{
    return g_displayInput.GetTouchScreenY(id);
}

static inline Vec2 GetTouchScreenXY(const TouchScreenId& id)
{
    return g_displayInput.GetTouchScreenXY(id);
}

static inline void gfxClearBackBuffer(const float r, const float g, const float b, const float a = 255.0f)
{
    g_gfxContext.ClearBackBuffer(r, g, b, a);
}

static inline Asset OpenAsset(const char* filename)
{
    return g_assetManager.OpenAsset(filename);
}

#endif // MAIN_H