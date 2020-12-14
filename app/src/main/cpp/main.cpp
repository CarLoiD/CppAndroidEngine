#include "engine.h"

#include <vector>

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

typedef struct {
    float FrameStep;
    std::vector<Rect2D> Frames;
} Animation;

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
constexpr const float g_fadeStep = 0.7f;
constexpr const uint32_t g_cloudsMaxUpRange = 40;
constexpr const uint32_t g_cloudsMaxDownRange = 400;
constexpr const uint32_t g_whiteColor = 0xFFFFFFFF;
constexpr const uint32_t g_maxClouds = 3;

const Vec2 g_touchHintPos = { 470.0f, 350.0f };

uint32_t g_spriteId = 0;
uint32_t g_clearColor = 0xFFFFFFFF;
uint32_t g_objectsColor = 0x505050FF;
uint32_t g_touchHintAlpha = 255;
uint32_t g_dinoAnimationIndex = 0;

float g_objectsVelocity = 1.0f;
float g_gravity = 0.0f;
float g_alphaTimer = 0.0f;
float g_moveTimer = 0.0f;
float g_dinoAnimationTimer = 0.0f;

bool g_isJumping = false;
bool g_isPlaying = false;
bool g_isFadingOut = true;
bool g_isFirstMove = false;
bool g_isDucking = false;

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

Animation dinoIdle;
Animation dinoRun;
Animation dinoDuckRun;
Animation dinoDied;

Animation* g_dinoAnimation = &dinoIdle;

void SetupSprites();
void UpdateVertexData(const BatchedSprite& sprite);
void SetupIndexData();
void FlushBufferData();
void SetupAnimations();
void UpdateSpriteAnimation(BatchedSprite& sprite, const Animation& animation, float& timer, uint32_t& index);
uint32_t GenerateRandomNumRange(const uint32_t min, const uint32_t max);

void Application::Create()
{
    // Seed the RNG
    srand(time(0));

    g_spritesVertices = new Vertex[4 * g_maxSprites];
    g_spritesIndices = new uint16_t[6 * g_maxSprites];

    // Clear the buffer data
    memset(g_spritesVertices, 0, (4 * g_maxSprites) * sizeof(Vertex));
    memset(g_spritesIndices, 0, (6 * g_maxSprites) * sizeof(uint16_t));

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
    SetupAnimations();

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
    const TouchScreenId id = TouchScreenId::Touch;

    const float touchX = getTouchScreenX(id) * (float)gfxGetDisplayWidth();
    const float touchY = getTouchScreenY(id) * (float)gfxGetDisplayHeight();

    if (touchX > (float)gfxGetDisplayWidth() / 2.0f)
    {
        if (!g_isJumping)
        {
            g_gravity = -g_jumpForce;
            g_isJumping = true;

            g_dinoAnimation = &dinoIdle;
        }
    }
    else if (touchX > 0.0f && !g_isJumping && g_isPlaying)
    {
        g_dinoAnimation = &dinoDuckRun;
        g_isDucking = true;

        UpdateSpriteAnimation(dino, *g_dinoAnimation, g_dinoAnimationTimer, g_dinoAnimationIndex);

        const float groundBottom = ground.Position.Y + ground.Size.Y * ground.Scale.Y;
        const float dinoSize = dino.Size.Y * dino.Scale.Y;

        dino.Position.Y = groundBottom - dinoSize;
    }
    else
    {
        if (g_isDucking)
        {
            g_dinoAnimation = &dinoRun;

            UpdateSpriteAnimation(dino, *g_dinoAnimation, g_dinoAnimationTimer, g_dinoAnimationIndex);

            const float groundBottom = ground.Position.Y + ground.Size.Y * ground.Scale.Y;
            const float dinoSize = dino.Size.Y * dino.Scale.Y;

            dino.Position.Y = groundBottom - dinoSize;

            g_isDucking = false;
        }
    }

    g_dinoAnimationTimer += deltaTime;
    g_alphaTimer += deltaTime;

    if (g_isFirstMove) {
        dino.Position.X += 50.0f * deltaTime;
        g_moveTimer += deltaTime;
    }

    if (g_moveTimer > 0.4f) {
        g_isFirstMove = false;
    }

    if (g_alphaTimer > g_fadeStep)
    {
        if (!g_isPlaying)
        {
            if (g_isFadingOut) {
                touchHint.Position = { (float) gfxGetDisplayWidth(), 0.0f };
                g_isFadingOut = false;
            } else {
                touchHint.Position = g_touchHintPos;
                g_isFadingOut = true;
            }
        }

        g_alphaTimer = 0.0f;
    }

    // Update the jump motion
    g_gravity += g_jumpWeight * deltaTime;
    ClampMax(g_gravity, g_maxGravity);

    // Apply the force
    dino.Position.Y += g_gravity * deltaTime;

    if (g_isPlaying && !g_isFirstMove)
    {
        // Scroll the ground infinitely
        ground.Position.X -= g_objectsSpeed * g_objectsVelocity * deltaTime;

        for (uint32_t index = 0; index < g_maxClouds; ++index)
        {
            // Move
            BatchedSprite *actualCloud = &clouds[index];
            actualCloud->Position.X -= g_cloudsSpeed * deltaTime;
        }
    }

    // Check ground collision
    if (dino.Position.Y + (dino.Size.Y * dino.Scale.Y) > ground.Position.Y + (ground.Size.Y * ground.Scale.Y))
    {
        const float groundBottom = ground.Position.Y + ground.Size.Y * ground.Scale.Y;
        const float dinoSize = dino.Size.Y * dino.Scale.Y;

        dino.Position.Y = groundBottom - dinoSize;

        if (!g_isPlaying && g_isJumping)
        {
            touchHint.Position     = { (float)gfxGetDisplayWidth(), 0.0f };
            crexLogo.Position      = { (float)gfxGetDisplayWidth(), 0.0f };
            developerInfo.Position = { (float)gfxGetDisplayWidth(), 0.0f };

            g_isFirstMove = true;
            g_isPlaying = true;

            g_dinoAnimation = &dinoRun;
        }

        if (g_isPlaying && !g_isFirstMove && !g_isDucking) {
            g_dinoAnimation = &dinoRun;
        }

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

    UpdateSpriteAnimation(dino, *g_dinoAnimation, g_dinoAnimationTimer, g_dinoAnimationIndex);

    // Update dynamic buffer data changes
    UpdateVertexData(touchHint);
    UpdateVertexData(dino);
    UpdateVertexData(ground);
    UpdateVertexData(crexLogo);
    UpdateVertexData(developerInfo);

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

    dino.Id       = g_spriteId++;
    dino.Scale    = { g_dinoScale, g_dinoScale };
    dino.TexRect  = { 1680.0f, 4.0f, 81, 92 };
    dino.Size     = { (float)dino.TexRect.Width, (float)dino.TexRect.Height };
    dino.Color    = g_objectsColor;

    // Ground

    ground.Id       = g_spriteId++;
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

        clouds[index].Id       = g_spriteId++;
        clouds[index].Scale    = { g_cloudsScale, g_cloudsScale };
        clouds[index].TexRect  = { 166.0f, 0.0f, 92, 29 };
        clouds[index].Size     = { (float)clouds[index].TexRect.Width, (float)clouds[0].TexRect.Height };
        clouds[index].Position = { (float)gfxGetDisplayWidth() + (index * g_cloudsDistance), maxCloudRangeY };
        clouds[index].Color    = g_objectsColor;

        UpdateVertexData(clouds[index]);
    }

    // C-Rex Logo

    crexLogo.Id       = g_spriteId++;
    crexLogo.Scale    = { g_crexLogoScale, g_crexLogoScale };
    crexLogo.TexRect  = { 1293.0f, 58.0f, 178, 25 };
    crexLogo.Size     = { (float)crexLogo.TexRect.Width, (float)crexLogo.TexRect.Height };
    crexLogo.Position = { 295.0f, 100.0f };
    crexLogo.Color    = g_whiteColor;

    // Developer Info

    developerInfo.Id       = g_spriteId++;
    developerInfo.Scale    = { g_developerInfoScale, g_developerInfoScale };
    developerInfo.TexRect  = { 1487.0f, 54.0f, 178, 11 };
    developerInfo.Size     = { (float)developerInfo.TexRect.Width, (float)developerInfo.TexRect.Height };
    developerInfo.Color    = g_whiteColor;

    const float developerInfoX = (float)gfxGetDisplayWidth() - (developerInfo.Size.X * developerInfo.Scale.X) - 15.0f;
    const float developerInfoY = (float)gfxGetDisplayHeight() - (developerInfo.Size.Y * developerInfo.Scale.Y) - 15.0f;

    developerInfo.Position = { developerInfoX, developerInfoY };

    touchHint.Id       = g_spriteId++;
    touchHint.Scale    = { g_touchHintScale, g_touchHintScale };
    touchHint.TexRect  = { 1487.0f, 69.0f, 123, 11 };
    touchHint.Size     = { (float)touchHint.TexRect.Width, (float)touchHint.TexRect.Height };
    touchHint.Position = g_touchHintPos;
    touchHint.Color    = g_whiteColor;

    UpdateVertexData(dino);
    UpdateVertexData(ground);
    UpdateVertexData(crexLogo);
    UpdateVertexData(developerInfo);
    UpdateVertexData(touchHint);
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

void SetupAnimations()
{
    // C-Rex Idle
    dinoIdle.FrameStep = 1.0f;
    dinoIdle.Frames.push_back({ 1680.0f, 5.0f, 81, 92 });

    // C-Rex Running
    dinoRun.FrameStep = 0.1f;
    dinoRun.Frames.push_back({ 1857.0f, 5.0f, 80, 86 });
    dinoRun.Frames.push_back({ 1945.0f, 5.0f, 80, 86 });

    // C-Rex Duck Running
    dinoDuckRun.FrameStep = 0.1f;
    dinoDuckRun.Frames.push_back({ 2211.0f, 39.0f, 110, 52 });
    dinoDuckRun.Frames.push_back({ 2329.0f, 39.0f, 110, 52 });
}

void UpdateSpriteAnimation(BatchedSprite& sprite, const Animation& animation, float& timer, uint32_t& index)
{
    if (timer > animation.FrameStep)
    {
        timer = 0.0f;
        ++index;
    }

    if (index > animation.Frames.size() - 1) {
        index = 0;
    }

    sprite.TexRect = animation.Frames.at(index);
    sprite.Size = { (float)sprite.TexRect.Width, (float)sprite.TexRect.Height };
}

uint32_t GenerateRandomNumRange(const uint32_t min, const uint32_t max)
{
    return min + (rand() % max);
}