#include "engine.h"

#include <vector>
#include <unordered_map>
#include <string>

typedef struct {
    float Position[3];
    float Color[4];
    float TexCoord[2];
} Vertex;

typedef struct {
    uint32_t Id;
    Vec2 Position;
    Vec2 Size;
    Vec2 Scale;
    Rect2D TexRect;
    uint32_t Color;
} CustomSprite;

constexpr const uint32_t g_maxSprites = 2;

constexpr const float g_dinoScale = 1.5f;
constexpr const float g_groundScale = 1.5f;

constexpr const float g_dinoPosX = 100.0f;
constexpr const float g_objectsSpeed = 300.0f;
constexpr const float g_gravity = 200.0f;

uint32_t g_clearColor = 0xFFFFFFFF;
uint32_t g_objectsColor = 0x505050FF;

float g_objectsVelocity = 1.0f;

bool g_isJumping = false;

Texture2D g_spritesTex;

VertexBuffer g_spritesBuffer;
IndexBuffer g_spritesIndexBuffer;

Vertex g_spritesVertices[4 * g_maxSprites];
uint16_t g_spritesIndices[6 * g_maxSprites];

const uint32_t nIndices = sizeof(g_spritesIndices) / sizeof(uint16_t);

CustomSprite dino;
CustomSprite ground;

void SetupSprites();
void UpdateVertexData(const CustomSprite& sprite);
void SetupIndexData();
void FlushBufferData();

void Application::Create()
{
    gfxSetWorkResolution({ 1280.0f, 720.0f });  // 720p as default work resolution

    Shader vertexShader;
    Shader pixelShader;

    gfxCompileShaderFromAsset("shaders/vertex_shader.glsl", ShaderType::VertexShader, vertexShader);
    gfxCompileShaderFromAsset("shaders/pixel_shader.glsl", ShaderType::PixelShader, pixelShader);

    gfxBindShader(vertexShader);
    gfxBindShader(pixelShader);

    gfxCreateTexture2D("textures/game_sprites.png", g_spritesTex, false, true);
    gfxBindTexture2D(g_spritesTex);

    SetupSprites();

    g_spritesBuffer.Stride = sizeof(Vertex);
    g_spritesBuffer.Size   = sizeof(g_spritesVertices);
    g_spritesBuffer.Data   = (void*)g_spritesVertices;

    const VertexElement layout[] = {
        VertexElement::Position,
        VertexElement::Color,
        VertexElement::TexCoord
    }; const uint32_t nLayout = sizeof(layout) / sizeof(VertexElement);

    gfxCreateVertexBuffer(layout, nLayout, g_spritesBuffer, true);
    gfxBindVertexBuffer(g_spritesBuffer);

    // Setup index data before binding it to the buffer
    SetupIndexData();
    
    g_spritesIndexBuffer.Stride = sizeof(uint16_t);
    g_spritesIndexBuffer.Size   = sizeof(g_spritesIndices);
    g_spritesIndexBuffer.Data   = (void*)g_spritesIndices;

    gfxCreateIndexBuffer(g_spritesIndexBuffer);
    gfxBindIndexBuffer(g_spritesIndexBuffer);

    const Vec2 displayRes = { (float)gfxGetDisplayWidth(), (float)gfxGetDisplayHeight() };
    const ScreenRect projRect = { 0.0f, displayRes.X, displayRes.Y, 0.0f };

    gfxSetWorldMatrix(mtxIdentity());
    gfxSetViewMatrix(mtxIdentity());
    gfxSetProjectionMatrix(mtxOrthoOffCenter(projRect, 0.0f, 1.0f));

    gfxSetPrimitiveType(PrimitiveType::TriangleList);

    // 2D rendering, disable depth test stage
    gfxDisableDepthBufferTesting();
}

void Application::Update(const float deltaTime)
{
    if (hasTouchEvent())
    {
        if (!g_isJumping)
        {
            dino.Position.Y -= 140.0f;
            g_isJumping = true;
        }
    }

    dino.Position.Y += g_gravity * deltaTime;

    if (dino.Position.Y + (dino.Size.Y * dino.Scale.Y) > ground.Position.Y + (ground.Size.Y * ground.Scale.Y))
    {
        const float groundBottom = ground.Position.Y + ground.Size.Y * ground.Scale.Y;
        const float dinoSize = dino.Size.Y * dino.Scale.Y;

        dino.Position.Y = groundBottom - dinoSize;

        g_isJumping = false;
    }

    ground.Position.X -= g_objectsSpeed * g_objectsVelocity * deltaTime;

    // Scroll the ground infinitely
    if (ground.Position.X < -((ground.Size.X * ground.Scale.X) / 2.0f)) {
        ground.Position.X = 0.0f;
    }

    UpdateVertexData(ground);
    UpdateVertexData(dino);

    FlushBufferData();
    gfxFlushMVPMatrix();

    gfxClearBackBuffer(g_clearColor);
    gfxDrawIndexed(nIndices);
}

void Application::Destroy()
{
    gfxDestroyTexture2D(g_spritesTex);
    gfxDestroyIndexBuffer(g_spritesIndexBuffer);
    gfxDestroyVertexBuffer(g_spritesBuffer);
}

void SetupSprites()
{
    // Dino

    dino.Id       = 0;
    dino.Scale    = { g_dinoScale, g_dinoScale };
    dino.TexRect  = { 1680.0f, 4.0f, 81, 92 };
    dino.Size     = { (float)dino.TexRect.Width, (float)dino.TexRect.Height };
    dino.Color    = g_objectsColor;

    // Ground

    ground.Id       = 1;
    ground.Scale    = { g_groundScale, g_groundScale };
    ground.TexRect  = { 0.0f, 103.0f, 2446 * 2, 26 };
    ground.Size     = { (float)ground.TexRect.Width, (float)ground.TexRect.Height };
    ground.Position = { 0.0f, (float)gfxGetDisplayHeight() - (ground.Size.Y * ground.Scale.Y) - 50.0f };
    ground.Color    = g_objectsColor;

    // Move dino to be at the ground
    const float groundBottom = ground.Position.Y + ground.Size.Y * ground.Scale.Y;
    const float dinoSize = dino.Size.Y * dino.Scale.Y;

    dino.Position = { g_dinoPosX, groundBottom - dinoSize };

    UpdateVertexData(dino);
    UpdateVertexData(ground);
}

void UpdateVertexData(const CustomSprite& sprite)
{
    const Vec2 workResScale = gfxGetWorkResScale();

    const float texWidth  = (float)g_spritesTex.Width;
    const float texHeight = (float)g_spritesTex.Height;

    const float posX = sprite.Position.X * workResScale.X;
    const float posY = sprite.Position.Y * workResScale.Y;
    const float posSizeX = (sprite.Position.X + sprite.Size.X * sprite.Scale.X) * workResScale.X;
    const float posSizeY = (sprite.Position.Y + sprite.Size.Y * sprite.Scale.Y) * workResScale.Y;

    const float texWidthX  = sprite.TexRect.X / texWidth;
    const float texHeightY = sprite.TexRect.Y / texHeight;

    const float texWidthOffsetX  = (sprite.TexRect.X + sprite.TexRect.Width) / texWidth;
    const float texHeightOffsetY = (sprite.TexRect.Y + sprite.TexRect.Height) / texHeight;

    float r, g, b, a;
    DwordToColorNormalized(sprite.Color, r, g, b, a);

    g_spritesVertices[ 4 * sprite.Id + 0 ] = { { posX    , posY    , 0.0f }, { r, g, b, a }, { texWidthX      , texHeightY       } };
    g_spritesVertices[ 4 * sprite.Id + 1 ] = { { posSizeX, posSizeY, 0.0f }, { r, g, b, a }, { texWidthOffsetX, texHeightOffsetY } };
    g_spritesVertices[ 4 * sprite.Id + 2 ] = { { posX    , posSizeY, 0.0f }, { r, g, b, a }, { texWidthX      , texHeightOffsetY } };
    g_spritesVertices[ 4 * sprite.Id + 3 ] = { { posSizeX, posY    , 0.0f }, { r, g, b, a }, { texWidthOffsetX, texHeightY       } };
}

void SetupIndexData()
{
    for (uint32_t index = 0; index < g_maxSprites; ++index)
    {
        g_spritesIndices[6 * index + 0] = 4 * index + 0;
        g_spritesIndices[6 * index + 1] = 4 * index + 1;
        g_spritesIndices[6 * index + 2] = 4 * index + 2;
        g_spritesIndices[6 * index + 3] = 4 * index + 0;
        g_spritesIndices[6 * index + 4] = 4 * index + 3;
        g_spritesIndices[6 * index + 5] = 4 * index + 1;
    }
}

void FlushBufferData()
{
    gfxUpdateVertexBuffer(g_spritesVertices, sizeof(g_spritesVertices), g_spritesBuffer);
}