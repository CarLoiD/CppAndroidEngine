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
} BatchedSprite;

constexpr const uint32_t g_maxSprites = 50;

constexpr const float g_dinoScale = 1.5f;
constexpr const float g_groundScale = 1.5f;
constexpr const float g_cloudsScale = 1.5f;
constexpr const float g_crexLogoScale = 4.0f;
constexpr const float g_developerInfoScale = 1.8f;
constexpr const float g_touchHintScale = 3.0f;

constexpr const float g_dinoPosX = 100.0f;
constexpr const float g_objectsSpeed = 900.0f;
constexpr const float g_maxGravity = 1500.0f;
constexpr const float g_jumpForce = 1650.0f;
constexpr const float g_jumpWeight = 4750.0f;
constexpr const float g_cloudsDistance = 400.0f;
constexpr const float g_cloudsSpeed = 200.0f;
constexpr const uint32_t g_cloudsMaxUpRange = 40;
constexpr const uint32_t g_cloudsMaxDownRange = 400;
constexpr const uint32_t g_maxClouds = 3;

uint32_t g_clearColor   = 0xFFFFFFFF;
uint32_t g_objectsColor = 0x505050FF;
uint32_t g_touchHintAlpha = 255;

float g_objectsVelocity = 1.0f;
float g_gravity = 0.0f;
float g_alphaTimer = 0.0f;

bool g_isJumping = false;
bool g_isPlaying = false;
bool g_isFadingOut = true;

Texture2D g_spritesTex;

VertexBuffer g_spritesBuffer;
IndexBuffer g_spritesIndexBuffer;

Vertex* g_spritesVertices;
uint16_t* g_spritesIndices;

const uint32_t nIndices = ((6 * g_maxSprites) * sizeof(uint16_t)) / sizeof(uint16_t);

BatchedSprite dino;
BatchedSprite ground;
BatchedSprite clouds[g_maxClouds];
BatchedSprite crexLogo;
BatchedSprite developerInfo;
BatchedSprite touchHint;

void SetupSprites();
void UpdateVertexData(const BatchedSprite& sprite);
void SetupIndexData();
void FlushBufferData();
uint32_t GenerateRandomNumRange(const uint32_t min, const uint32_t max);

void Application::Create()
{
    // Seed the RNG
    srand(time(0));

    g_spritesVertices = new Vertex[4 * g_maxSprites];
    g_spritesIndices = new uint16_t[6 * g_maxSprites];

    // Clear the buffer data
    memset(g_spritesVertices, 0, sizeof(g_spritesVertices));
    memset(g_spritesIndices, 0, sizeof(g_spritesIndices));

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
    g_spritesBuffer.Size   = (4 * g_maxSprites) * sizeof(Vertex);
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
    g_spritesIndexBuffer.Size   = (6 * g_maxSprites) * sizeof(uint16_t);
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
            g_gravity = -g_jumpForce;
            g_isJumping = true;
        }
    }

    g_alphaTimer += deltaTime;

    if (g_alphaTimer > 2.0f) {
        touchHint.Color = 0xFFFFFF00;
    }

    // Update the jump motion
    g_gravity += g_jumpWeight * deltaTime;
    ClampMax(g_gravity, g_maxGravity);

    // Apply the force
    dino.Position.Y += g_gravity * deltaTime;

    if (g_isPlaying)
    {
        // Scroll the ground infinitely
        ground.Position.X -= g_objectsSpeed * g_objectsVelocity * deltaTime;

        for (uint32_t index = 0; index < g_maxClouds; ++index)
        {
            BatchedSprite *actualCloud = &clouds[index];

            // Move
            actualCloud->Position.X -= g_cloudsSpeed * deltaTime;
        }
    }

    // Check ground collision
    if (dino.Position.Y + (dino.Size.Y * dino.Scale.Y) > ground.Position.Y + (ground.Size.Y * ground.Scale.Y))
    {
        const float groundBottom = ground.Position.Y + ground.Size.Y * ground.Scale.Y;
        const float dinoSize = dino.Size.Y * dino.Scale.Y;

        dino.Position.Y = groundBottom - dinoSize;
        g_isJumping = false;
    }

    if (ground.Position.X < -((ground.Size.X * ground.Scale.X) / 2.0f)) {
        ground.Position.X = 0.0f;
    }

    // Scroll the clouds

    for (uint32_t index = 0; index < g_maxClouds; ++index)
    {
        BatchedSprite* actualCloud = &clouds[index];

        // Check collision with the left-most border
        if (actualCloud->Position.X < -(actualCloud->Size.X * actualCloud->Scale.X))
        {
            const float cloudsRangeY = (float)GenerateRandomNumRange(g_cloudsMaxUpRange, g_cloudsMaxDownRange);
            actualCloud->Position = { (float)gfxGetDisplayWidth(), cloudsRangeY };
        }

        UpdateVertexData(clouds[index]);
    }

    // Update dynamic buffer data changes
    UpdateVertexData(ground);
    UpdateVertexData(dino);
    UpdateVertexData(crexLogo);
    UpdateVertexData(developerInfo);
    UpdateVertexData(touchHint);

    // Flush ModelViewProj and map updated dynamic buffer data
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

    // After GPU unbind buffer data, make sure to deallocate the used heap memory
    delete[] g_spritesIndices;
    delete[] g_spritesVertices;
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

    // Clouds

    for (uint32_t index = 0; index < g_maxClouds; ++index)
    {
        const float maxCloudRangeY = (float)GenerateRandomNumRange(g_cloudsMaxUpRange, g_cloudsMaxDownRange);

        clouds[index].Id       = 2 + index;
        clouds[index].Scale    = { g_cloudsScale, g_cloudsScale };
        clouds[index].TexRect  = { 166.0f, 0.0f, 92, 29 };
        clouds[index].Size     = { (float)clouds[index].TexRect.Width, (float)clouds[0].TexRect.Height };
        clouds[index].Position = { (float)gfxGetDisplayWidth() + (index * g_cloudsDistance), maxCloudRangeY };
        clouds[index].Color    = g_objectsColor;

        UpdateVertexData(clouds[index]);
    }

    // C-Rex Logo

    crexLogo.Id       = 5;
    crexLogo.Scale    = { g_crexLogoScale, g_crexLogoScale };
    crexLogo.TexRect  = { 1293.0f, 58.0f, 178, 25 };
    crexLogo.Size     = { (float)crexLogo.TexRect.Width, (float)crexLogo.TexRect.Height };
    crexLogo.Position = { 295.0f, 100.0f };
    crexLogo.Color    = 0xFFFFFFFF;

    // Developer Info

    developerInfo.Id       = 6;
    developerInfo.Scale    = { g_developerInfoScale, g_developerInfoScale };
    developerInfo.TexRect  = { 1487.0f, 54.0f, 178, 11 };
    developerInfo.Size     = { (float)developerInfo.TexRect.Width, (float)developerInfo.TexRect.Height };
    developerInfo.Color    = 0xFFFFFFFF;

    const float developerInfoX = (float)gfxGetDisplayWidth() - (developerInfo.Size.X * developerInfo.Scale.X) - 15.0f;
    const float developerInfoY = (float)gfxGetDisplayHeight() - (developerInfo.Size.Y * developerInfo.Scale.Y) - 15.0f;

    developerInfo.Position = { developerInfoX, developerInfoY };

    touchHint.Id       = 7;
    touchHint.Scale    = { g_touchHintScale, g_touchHintScale };
    touchHint.TexRect  = { 1487.0f, 69.0f, 123, 11 };
    touchHint.Size     = { (float)touchHint.TexRect.Width, (float)touchHint.TexRect.Height };
    touchHint.Position = { 470.0f, 350.0f };
    touchHint.Color    = 0xFFFFFFFF;

    UpdateVertexData(touchHint);
    UpdateVertexData(developerInfo);
    UpdateVertexData(crexLogo);
    UpdateVertexData(dino);
    UpdateVertexData(ground);
}

void UpdateVertexData(const BatchedSprite& sprite)
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
    gfxUpdateVertexBuffer(g_spritesVertices, (4 * g_maxSprites) * sizeof(Vertex), g_spritesBuffer);
}

uint32_t GenerateRandomNumRange(const uint32_t min, const uint32_t max)
{
    return min + (rand() % max);
}