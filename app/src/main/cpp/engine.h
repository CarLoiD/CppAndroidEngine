#ifndef ENGINE_H
#define ENGINE_H

// Engine
#include "Engine/utils.h"
#include "Engine/asset_manager.h"
#include "Engine/clock.h"
#include "Engine/touchscreen.h"
#include "Engine/gfx_math.h"
#include "Engine/graphics_context.h"
#include "Engine/shader_compiler.h"
#include "Engine/vertex_buffer.h"
#include "Engine/index_buffer.h"

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

uint32_t g_shaderProgram = 0;
uint32_t g_vertexShaderId = 0;
uint32_t g_pixelShaderId = 0;

PrimitiveType g_primitiveType = PrimitiveType::TriangleList;

Matrix g_world;
Matrix g_view;
Matrix g_projection;

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

    g_shaderProgram = glCreateProgram();

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

/// TOUCHSCREEN

inline float getTouchScreenX(const TouchScreenId& id)
{
    return g_displayInput.GetTouchScreenX(id);
}

inline float getTouchScreenY(const TouchScreenId& id)
{
    return g_displayInput.GetTouchScreenY(id);
}

inline Vec2 getTouchScreenXY(const TouchScreenId& id)
{
    return g_displayInput.GetTouchScreenXY(id);
}

inline bool hasTouchEvent()
{
    const TouchScreenId id = TouchScreenId::Touch;
    return getTouchScreenX(id) != -1.0f && getTouchScreenY(id) != -1.0f;
}

/// ASSET

inline Asset openAsset(const char* filename)
{
    return g_assetManager.OpenAsset(filename);
}

/// GFX

inline uint32_t gfxGetDisplayWidth()
{
    return g_gfxContext.GetDisplayWidth();
}

inline uint32_t gfxGetDisplayHeight()
{
    return g_gfxContext.GetDisplayHeight();
}

inline void gfxClearBackBuffer(const float r, const float g, const float b, const float a = 255.0f)
{
    g_gfxContext.ClearBackBuffer(r, g, b, a);
}


inline void gfxCompileShaderFromCode(const char* code, const ShaderType& type, Shader& object)
{
    CompileShader(code, type, object);
}

inline void gfxCompileShaderFromAsset(const char* filename, const ShaderType& type, Shader& object)
{
    Asset shaderAsset = openAsset(filename);

    if (shaderAsset.IsOpen())
    {
        const uint32_t length = shaderAsset.GetLength();
        char* buffer = new char[length];

        shaderAsset.Read(buffer, length);
        CompileShader(buffer, type, object);

        delete[] buffer;

        shaderAsset.Close();
    }
}

inline void gfxBindShader(const Shader& shader)
{
    switch (shader.Type)
    {
    case ShaderType::VertexShader:
        LogDebug("BindShader (VertexShader, Id: %d)", shader.Id);

        glAttachShader(g_shaderProgram, shader.Id);
        g_vertexShaderId = shader.Id;

        break;

    case ShaderType::PixelShader:
        LogDebug("BindShader (PixelShader, Id: %d)", shader.Id);

        if (g_vertexShaderId > 0)
        {
            LogDebug("BindShaderProgram Id -> %d", g_shaderProgram);
            glAttachShader(g_shaderProgram, shader.Id);

            glLinkProgram(g_shaderProgram);
            glUseProgram(g_shaderProgram);

            g_pixelShaderId = shader.Id;
        } else {
            LogError("gfxError: Trying to bind a vertex shader to a pipeline missing a vertex shader");
        }

        break;
    }
}

inline void gfxCreateVertexBuffer(const VertexElement* layout, const uint32_t count, VertexBuffer& buffer)
{
    CreateVertexBuffer(g_shaderProgram, layout, count, buffer);
}

inline void gfxDestroyVertexBuffer(const VertexBuffer& buffer)
{
    DestroyVertexBuffer(buffer);
}

inline void gfxBindVertexBuffer(const VertexBuffer& buffer)
{
    BindVertexBuffer(buffer);
}

inline void gfxSetPrimitiveType(const PrimitiveType& type)
{
    g_primitiveType = type;
}

inline void gfxDraw(const uint32_t offset, const uint32_t count)
{
    g_gfxContext.Draw(g_primitiveType, offset, count);
}

inline void gfxDrawIndexed(const uint32_t count)
{
    g_gfxContext.DrawIndexed(g_primitiveType, count);
}

inline void gfxSetWorldMatrix(const Matrix& mtx)
{
    g_world = mtxTranspose(mtx);
}

inline void gfxSetViewMatrix(const Matrix& mtx)
{
    g_view = mtxTranspose(mtx);
}

inline void gfxSetProjectionMatrix(const Matrix& mtx)
{
    g_projection = mtxTranspose(mtx);
}

inline void gfxFlushMVPMatrix()
{
    const int32_t location = glGetUniformLocation(g_shaderProgram, "ModelViewProj");

    if (location == EOF) {
         // LogError("gfxError: Invalid shader uniform location :: gfxFlushMVPMatrix()");
    } else {
        const Matrix mvp = g_projection * g_view * g_world;
        glUniformMatrix4fv(location, 1, GL_FALSE, &mvp.M[0][0]);
    }
}

inline void gfxEnableDepthBufferTesting()
{
    g_gfxContext.EnableDepthBufferTesting();
}

inline void gfxDisableDepthBufferTesting()
{
    g_gfxContext.DisableDepthBufferTesting();
}

inline void gfxCreateIndexBuffer(IndexBuffer& buffer)
{
    CreateIndexBuffer(buffer);
}

inline void gfxDestroyIndexBuffer(const IndexBuffer& buffer)
{
    DestroyIndexBuffer(buffer);
}

inline void gfxBindIndexBuffer(const IndexBuffer& buffer)
{
    BindIndexBuffer(buffer);
}

#endif // ENGINE_H