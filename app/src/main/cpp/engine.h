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
#include "Engine/texture2d.h"
#include "Engine/sprite.h"

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
static Clock g_mainClock;
static TouchScreen g_displayInput;
static GraphicsContext g_gfxContext;
static AssetManager g_assetManager;

static uint32_t g_shaderProgram = 0;
static uint32_t g_vertexShaderId = 0;
static uint32_t g_pixelShaderId = 0;

static PrimitiveType g_primitiveType = PrimitiveType::TriangleList;

static Matrix g_world;
static Matrix g_view;
static Matrix g_projection;

static Vec2 g_workRes;

namespace Application
{
    void Create();
    void Update(const float deltaTime);
    void Destroy();
}

extern "C" JNIEXPORT void JNICALL
Java_com_carloid_cppandroidengine_EngineGLRenderer_ApplicationCreate(JNIEnv* env, jobject obj, jint width, jint height, jobject assetManager)
{
    g_workRes = { (float)width, (float)height };

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

inline void gfxClearBackBuffer(const uint32_t color)
{
    float r, g, b, a;
    DwordToColor(color, r, g, b, a);

    gfxClearBackBuffer(r, g, b, a);
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
        char* buffer = new char[length + 1];

        shaderAsset.Read(buffer, length);
        buffer[length] = '\0';

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

inline void gfxCreateVertexBuffer(const VertexElement* layout, const uint32_t count, VertexBuffer& buffer, const bool dynamic = false)
{
    CreateVertexBuffer(g_shaderProgram, layout, count, buffer, dynamic);
}

inline void gfxDestroyVertexBuffer(const VertexBuffer& buffer)
{
    DestroyVertexBuffer(buffer);
}

inline void gfxBindVertexBuffer(const VertexBuffer& buffer)
{
    BindVertexBuffer(buffer);
}

inline void gfxUpdateVertexBuffer(const void* data, const uint32_t size, const VertexBuffer& buffer)
{
    UpdateVertexBuffer(data, size, buffer);
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

inline void gfxCreateTexture2D(const char* path, Texture2D& texture, const bool filtered = true, const bool repeat = false)
{
    Asset textureAsset = openAsset(path);

    if (textureAsset.IsOpen())
    {
        CreateTexture2D(textureAsset, texture, filtered, repeat);
        textureAsset.Close();
    } else {
        LogError("gfxError: Failed to open the texture asset file :: gfxCreateTexture2D()");
    }
}

inline void gfxDestroyTexture2D(Texture2D& texture)
{
    DestroyTexture2D(texture);
}

inline void gfxBindTexture2D(const Texture2D& texture)
{
    BindTexture2D(texture);
}

inline void gfxSetWorkResolution(const Vec2& workRes)
{
    g_workRes = workRes;
    glViewport(0, 0, (uint32_t)workRes.X, (uint32_t)workRes.Y);
}

inline Vec2 gfxGetWorkResolution()
{
    return g_workRes;
}

inline Vec2 gfxGetWorkResScale()
{
    const float workResScaleX = (float)gfxGetDisplayWidth() / g_workRes.X;
    const float workResScaleY = (float)gfxGetDisplayHeight() / g_workRes.Y;

    return { workResScaleX, workResScaleY };
}

inline void gfxCreateSprite(Sprite& sprite, Texture2D& texture, const Vec2& position = { 0.0f, 0.0f })
{
    CreateSprite(sprite, g_shaderProgram, gfxGetWorkResScale(), texture, position);
}

inline void gfxDestroySprite(Sprite& sprite)
{
    DestroySprite(sprite);
}

inline void gfxSpriteSetPosition(Sprite& sprite, const Vec2& position)
{
    SpriteSetPosition(sprite, position);
}

inline void gfxSpriteSetSize(Sprite& sprite, const Vec2& size)
{
    SpriteSetSize(sprite, size);
}

inline void gfxSpriteSetScale(Sprite& sprite, const Vec2& scale)
{
    SpriteSetScale(sprite, scale);
}

inline void gfxSpriteSetColor(Sprite& sprite, const uint32_t color)
{
    SpriteSetColor(sprite, color);
}

inline void gfxSpriteSetTexRect(Sprite& sprite, const Rect2D& texrect)
{
    SpriteSetTexRect(sprite, texrect);
}

inline void gfxDrawSprite(Sprite& sprite)
{
    SpriteDraw(sprite, gfxGetWorkResScale());
}

#endif // ENGINE_H