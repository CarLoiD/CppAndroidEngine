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

typedef struct {
    uint32_t GlyphOffset;
    uint32_t GlyphCount;
    Rect2D BaseRect;
    BatchedSprite* Glyphs;
} BitmapText;

// Engine settings
constexpr const uint32_t g_maxSprites = 50;
const Vec2 g_gameWorkRes = { 1280.0f, 720.0f };

// Buffer measures
constexpr const uint32_t g_vertexBufferSize = 4 * g_maxSprites * sizeof(Vertex);
constexpr const uint32_t g_indexBufferSize = 6 * g_maxSprites * sizeof(uint16_t);

// Sprite scale
constexpr const float g_commonScale = 1.5f;
constexpr const float g_crexLogoScale = 4.0f;
constexpr const float g_developerInfoScale = 1.8f;
constexpr const float g_touchHintScale = 3.0f;

constexpr const float g_scoreIndicatorScale = 1.4f;
// Float constants
constexpr const float g_dinoPosX = 100.0f;
constexpr const float g_objectsSpeed = 900.0f;
constexpr const float g_maxGravity = 1500.0f;
constexpr const float g_jumpForce = 1650.0f;
constexpr const float g_jumpWeight = 4750.0f;
constexpr const float g_cloudsDistance = 400.0f;
constexpr const float g_cloudsSpeed = 200.0f;
constexpr const float g_fadeStep = 0.7f;
constexpr const float g_recoverTime = 0.2f;
constexpr const float g_dinoCollisionThreshold = 0.9f;
constexpr const float g_scoreStep = 0.1f;
constexpr const float g_colorFadingDelay = 2.0f;

// Float color constants
constexpr const float g_dayTimeObjColor = (float)0x50;
constexpr const float g_nightTimeObjColor = (float)0xFF;
constexpr const float g_dayTimeBgColor = (float)0xFF;
constexpr const float g_nightTimeBgColor = (float)0x00;

// Unsigned integer constants
constexpr const uint32_t g_cloudsMaxUpRange = 40;
constexpr const uint32_t g_cloudsMaxDownRange = 400;
constexpr const uint32_t g_whiteColor = 0xFFFFFFFF;
constexpr const uint32_t g_greyColor = 0x505050FF;
constexpr const uint32_t g_maxClouds = 3;
constexpr const uint32_t g_maxCactus = 4;
constexpr const uint32_t g_scoreToFade = 100;

// Vec2 constants
const Vec2 g_touchHintPos = { 470.0f, 350.0f };
const Vec2 g_gameOverPos = { 375.0f, 280.0f };
const Vec2 g_retryButtonPos = { 610.0f, 370.0f };
const Vec2 g_currentScorePos = { 1000.0f, 100.0f };
const Vec2 g_highScorePos = { 820.0f, 100.0f };
const Vec2 g_highIndicatorPos = { 740.0f, 100.0f };
const Vec2 g_moonPos = { 800.0f, 190.0f };

const Rect2D g_cactusRect[] = {
    { 447.0f, 3.0f, 30, 66 },   // Single small cactus
    { 482.0f, 3.0f, 64, 66 },   // Double small cactus
    { 654.0f, 3.0f, 46, 96 },   // Single big cactus
    { 654.0f, 3.0f, 94, 96 },   // Double big cactus
    { 654.0f, 3.0f,146, 96 },   // Triple big cactus
};

uint32_t g_spriteId = 0;
uint32_t g_clearColor = g_whiteColor;
uint32_t g_objectsColor = g_greyColor;
uint32_t g_touchHintAlpha = 255;
uint32_t g_dinoAnimationIndex = 0;
uint32_t g_pteroAnimationIndex = 0;
uint32_t g_currentScore = 0;
uint32_t g_highScore = 0;

float g_objectsVelocity = 1.0f;
float g_gravity = 0.0f;
float g_alphaTimer = 0.0f;
float g_moveTimer = 0.0f;
float g_dinoAnimationTimer = 0.0f;
float g_pteroAnimationTimer = 0.0f;
float g_respawnTimer = 0.0f;
float g_scoreTimer = 0.0f;
float g_colorFadeTimer = 0.0f;
float g_jumpInfluence = 1.0f;

bool g_isJumping = false;
bool g_isPlaying = false;
bool g_isInPauseScreen = true;
bool g_isFadingOut = true;
bool g_isFirstMove = false;
bool g_isDucking = false;
bool g_isDinoDead = false;
bool g_isRespawning = false;
bool g_isNightTime = false;
bool g_isFadingTime = false;

char g_scoreBuffer[5];
char g_highScoreBuffer[5];

Texture2D g_spritesTex;

VertexBuffer g_spritesBuffer;
IndexBuffer g_spritesIndexBuffer;

Vertex* g_spritesVertices;
uint16_t* g_spritesIndices;

const uint32_t nIndices = g_indexBufferSize / sizeof(uint16_t);

BatchedSprite dino;
BatchedSprite ground;
BatchedSprite clouds[g_maxClouds];
BatchedSprite crexLogo;
BatchedSprite developerInfo;
BatchedSprite touchHint;
BatchedSprite cactus[g_maxCactus];
BatchedSprite gameOver;
BatchedSprite retry;
BatchedSprite highIndicator;
BatchedSprite moon;
BatchedSprite pterodactyl;

Animation dinoIdle;
Animation dinoRun;
Animation dinoDuckRun;
Animation dinoDied;

Animation pterodactylAnim;

BitmapText currentScore;
BitmapText highScore;

Animation* g_dinoAnimation = &dinoIdle;
Animation* g_pteroAnimation = &pterodactylAnim;

void SetupSprites();
void UpdateVertexData(const BatchedSprite& sprite);
void SetupIndexData();
void FlushBufferData();
void SetupAnimations();
void SetObjectAboveGround(BatchedSprite& object);
void UpdateSpriteAnimation(BatchedSprite& sprite, const Animation& animation, float& timer, uint32_t& index);
uint32_t GenerateRandomNumRange(const uint32_t min, const uint32_t max);
bool HasAABBCollided(const BatchedSprite& lhe, const BatchedSprite& rhe, const Vec2 lheRectThreshold = { 1.0f }, const Vec2 rheRectThreshold = { 1.0f });
bool CheckTouchAgainstSprite(const BatchedSprite& sprite);
void RestartObjectsPosition();
void Uint32ToStr(const uint32_t integer, char* buffer, const uint32_t size);
void SetupBitmapText(BitmapText& text, const Vec2& position, const Vec2& scale, const uint32_t glyphCount, const Rect2D& base);
void DestroyBitmapText(BitmapText& text);
void SetBitmapTextString(BitmapText& text, const char* buffer, const uint32_t size);
void SetBitmapTextVerticalPos(BitmapText& text, const float position);
void SetBitmapTextVerticalColor(BitmapText& text, const uint32_t color);
float Lerp(const float lhe, const float rhe, const float delta);
bool IsTheBestDayOfTheWeek();   // Jessica's Easter Egg

void Application::Create()
{
    // Seed the RNG
    srand(time(0));

    g_spritesVertices = new Vertex[4 * g_maxSprites];
    g_spritesIndices = new uint16_t[6 * g_maxSprites];

    // Clear the buffer data
    memset(g_spritesVertices, 0, g_vertexBufferSize);
    memset(g_spritesIndices, 0, g_indexBufferSize);

    gfxSetWorkResolution(g_gameWorkRes);  // 720p as default work resolution
    glViewport(0, 0, gfxGetDisplayWidth(), gfxGetDisplayHeight());

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

    const Vec2 currentPos = { g_currentScorePos.X, -g_gameWorkRes.Y };
    const Vec2 highPos    = { g_highScorePos.X, -g_gameWorkRes.Y };
    const Rect2D baseRect = { 1293.0f, 2.0f, 20, 21 };

    SetupBitmapText(currentScore, currentPos, { g_commonScale }, sizeof(g_scoreBuffer), baseRect);
    SetupBitmapText(highScore, highPos, { g_commonScale }, sizeof(g_highScoreBuffer), baseRect);

    g_spritesBuffer.Stride = sizeof(Vertex);
    g_spritesBuffer.Size   = g_vertexBufferSize;
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
    g_spritesIndexBuffer.Size   = g_indexBufferSize;
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

    // Update timers

    g_dinoAnimationTimer += deltaTime;
    g_pteroAnimationTimer += deltaTime;
    g_alphaTimer += deltaTime;
    g_scoreTimer += deltaTime;
    g_colorFadeTimer = g_isFadingTime ? g_colorFadeTimer + deltaTime : 0.0f;

    UpdateSpriteAnimation(dino, *g_dinoAnimation, g_dinoAnimationTimer, g_dinoAnimationIndex);

    if (g_isRespawning)
    {
        g_respawnTimer += deltaTime;

        if (g_respawnTimer > g_recoverTime) {
            g_respawnTimer = 0.0f;
            g_isRespawning = false;
        }
    }

    // Touch happens in the right half of the screen, jump
    if (touchX > g_gameWorkRes.X / 2.0f && !g_isDinoDead && !g_isRespawning)
    {
        if (!g_isJumping)
        {
            g_gravity = -g_jumpForce;
            g_isJumping = true;

            g_dinoAnimation = &dinoIdle;
        }
    }

    // Touch happens in the left half of the screen, duck
    else if (touchX > 0.0f && g_isPlaying && !g_isDinoDead && !g_isRespawning)
    {
        if (!g_isJumping)
        {
            g_dinoAnimation = &dinoDuckRun;
            g_isDucking = true;

            UpdateSpriteAnimation(dino, *g_dinoAnimation, g_dinoAnimationTimer, g_dinoAnimationIndex);
            SetObjectAboveGround(dino);
        } else {
            g_jumpInfluence = 3.0f;
        }
    }

    // Back to normal state (running)
    else
    {
        if (g_isDucking)
        {
            g_dinoAnimation = &dinoRun;
            g_isDucking = false;
        }
    }

    // Do that little horizontal padding after being in game (only happens once)

    if (g_isFirstMove) {
        dino.Position.X += 50.0f * deltaTime;
        g_moveTimer += deltaTime;
    }

    if (g_moveTimer > 0.4f) {
        g_isFirstMove = false;
    }

    if (g_alphaTimer > g_fadeStep)
    {
        if (!g_isPlaying && g_isInPauseScreen)
        {
            if (g_isFadingOut) {
                touchHint.Position = { -g_gameWorkRes.X, 0.0f };
                g_isFadingOut = false;
            } else {
                touchHint.Position = g_touchHintPos;
                g_isFadingOut = true;
            }
        }

        g_alphaTimer = 0.0f;
    }

    // Update the jump motion
    g_gravity += g_jumpWeight * g_jumpInfluence * deltaTime;
    ClampMax(g_gravity, g_maxGravity);

    // Apply the force
    if (!g_isDinoDead) {
        dino.Position.Y += g_gravity * deltaTime;
    }

    // Animate things when playing

    if (g_isPlaying && !g_isFirstMove)
    {
        UpdateSpriteAnimation(pterodactyl, *g_pteroAnimation, g_pteroAnimationTimer, g_pteroAnimationIndex);

        if (g_isJumping) {
            g_dinoAnimation = &dinoIdle;
        }

        if (g_scoreTimer > g_scoreStep)
        {
            g_objectsVelocity += 0.001f;

            if (++g_currentScore > 99999) {
                g_currentScore = 99999;
            }

            Uint32ToStr(g_currentScore, g_scoreBuffer, sizeof(g_scoreBuffer));
            SetBitmapTextString(currentScore, g_scoreBuffer, sizeof(g_scoreBuffer));

            g_scoreTimer = 0.0f;
        }

        // Process the time fading

        if (g_isFadingTime)
        {
            if (g_colorFadeTimer < g_colorFadingDelay)
            {
                // Normalize the difference
                const float delta = (g_colorFadingDelay - g_colorFadeTimer) / g_colorFadingDelay;

                // Object color
                const float toObjColor = g_isNightTime ? g_nightTimeObjColor : g_dayTimeObjColor;
                const float fromObjColor = g_isNightTime ? g_dayTimeObjColor : g_nightTimeObjColor;

                const float objColor = Lerp(toObjColor, fromObjColor, delta);
                ColorToDword(objColor, objColor, objColor, 0xFF, g_objectsColor);

                // Background color
                const float toBgColor = g_isNightTime ? g_nightTimeBgColor : g_dayTimeBgColor;
                const float fromBgColor = g_isNightTime ? g_dayTimeBgColor : g_nightTimeBgColor;

                const float bgColor = Lerp(toBgColor, fromBgColor, delta);
                ColorToDword(bgColor, bgColor, bgColor, 0xFF, g_clearColor);
            }

            else {
                g_isFadingTime = false;
            }
        }

        if (g_currentScore > 0 && g_currentScore % g_scoreToFade == 0)
        {
            if (!g_isFadingTime) {
                g_isNightTime = !g_isNightTime;
                g_isFadingTime = true;
            }
        }

        // Scroll the ground infinitely
        ground.Position.X -= g_objectsSpeed * g_objectsVelocity * deltaTime;

        // Scroll the clouds
        for (uint32_t index = 0; index < g_maxClouds; ++index)
        {
            // Move
            BatchedSprite* actualCloud = &clouds[index];
            actualCloud->Position.X -= g_cloudsSpeed * deltaTime;
        }

        // Scroll the cactus
        for (uint32_t index = 0; index < g_maxCactus; ++index) {
            cactus[index].Position.X -= g_objectsSpeed * g_objectsVelocity * deltaTime;
        }

        // Scroll the pterodactyl
        pterodactyl.Position.X -= g_objectsSpeed * (g_objectsVelocity + 0.15f) * deltaTime;
    }

    // Check ground collision
    if (dino.Position.Y + (dino.Size.Y * dino.Scale.Y) > ground.Position.Y + (ground.Size.Y * ground.Scale.Y))
    {
        g_jumpInfluence = 1.0f;
        SetObjectAboveGround(dino);

        if (!g_isPlaying && g_isJumping)
        {
            // Hide main menu title's
            touchHint.Position     = { -g_gameWorkRes.X, 0.0f };
            crexLogo.Position      = { -g_gameWorkRes.X, 0.0f };
            developerInfo.Position = { -g_gameWorkRes.X, 0.0f };
            highIndicator.Position = g_highIndicatorPos;

            SetBitmapTextVerticalPos(currentScore, g_currentScorePos.Y);
            SetBitmapTextVerticalPos(highScore, g_highScorePos.Y);

            g_isFirstMove = true;
            g_isPlaying = true;

            g_dinoAnimation = &dinoRun;
            g_isInPauseScreen = false;

            g_scoreTimer = 0.0f;
        }

        if (g_isPlaying && !g_isFirstMove && !g_isDucking) {
            g_dinoAnimation = &dinoRun;
        }

        g_isJumping = false;
    }

    if (ground.Position.X < -((ground.Size.X * ground.Scale.X) / 2.0f)) {
        ground.Position.X = 0.0f;
    }

    for (uint32_t index = 0; index < g_maxClouds; ++index)
    {
        BatchedSprite* actualCloud = &clouds[index];

        // Check collision with the left-most border
        if (actualCloud->Position.X < -(actualCloud->Size.X * actualCloud->Scale.X))
        {
            // Randomly regenerate the Y position of the cloud
            const float cloudsRangeY = (float)GenerateRandomNumRange(g_cloudsMaxUpRange, g_cloudsMaxDownRange);
            actualCloud->Position = { g_gameWorkRes.X, cloudsRangeY };
        }

        UpdateVertexData(clouds[index]);
    }

    // Check objects collision

    for (uint32_t index = 0; index < g_maxCactus; ++index)
    {
        if (cactus[index].Position.X < -(cactus[index].Size.X * cactus[index].Scale.X))
        {
            const uint32_t cactusRect = GenerateRandomNumRange(0, 4);
            const float offset = 0.1f * GenerateRandomNumRange(7, 11);

            cactus[index].TexRect    = g_cactusRect[cactusRect];
            cactus[index].Size       = { (float)cactus[index].TexRect.Width, (float)cactus[index].TexRect.Height };
            cactus[index].Position.X = (g_gameWorkRes.X + 200.0f) * g_maxCactus;// * offset;

            SetObjectAboveGround(cactus[index]);
        }

        if (HasAABBCollided(dino, cactus[index], { g_dinoCollisionThreshold })) {
            g_isPlaying = false;
            g_isDinoDead = true;
        }

        cactus[index].Color = g_objectsColor;
        UpdateVertexData(cactus[index]);
    }

    if (pterodactyl.Position.X < -(pterodactyl.Size.X * pterodactyl.Scale.X)) {
        pterodactyl.Position.X = g_gameWorkRes.X * (float)GenerateRandomNumRange(2, 6);
    }

    if (HasAABBCollided(dino, pterodactyl, { 0.9f, 0.8f }, { 0.5f, 0.9f })) {
        g_isPlaying = false;
        g_isDinoDead = true;
    }

    // Ded :P

    if (g_isDinoDead)
    {
        g_dinoAnimation = &dinoDied;

        // Update high score
        if (g_currentScore > g_highScore)
        {
            g_highScore = g_currentScore;

            Uint32ToStr(g_highScore, g_highScoreBuffer, sizeof(g_highScoreBuffer));
            SetBitmapTextString(highScore, g_highScoreBuffer, sizeof(g_highScoreBuffer));
        }

        if (CheckTouchAgainstSprite(retry))
        {
            RestartObjectsPosition();

            g_isDinoDead = false;
            g_isPlaying = true;
            g_isRespawning = true;

            // Update score
            g_currentScore = 0.0f;

            // Force daytime
            const uint32_t objColor = (uint32_t)g_dayTimeObjColor;
            ColorToDword(objColor, objColor, objColor, 0xFF, g_objectsColor);

            const uint32_t bgColor = (uint32_t)g_dayTimeBgColor;
            ColorToDword(bgColor, bgColor, bgColor, 0xFF, g_clearColor);

            g_isNightTime = false;

            g_objectsVelocity = 1.0f;
        }
    }

    // Show game_over and retry_button when dino is ded :P
    gameOver.Position = g_isDinoDead ? g_gameOverPos : Vec2(-g_gameWorkRes.X, 0.0f);
    retry.Position = g_isDinoDead ? g_retryButtonPos : Vec2(-g_gameWorkRes.X, 0.0f);

    // Update object colors

    dino.Color = g_objectsColor;
    ground.Color = g_objectsColor;
    gameOver.Color = g_objectsColor;
    retry.Color = g_objectsColor;
    highIndicator.Color = g_objectsColor;
    pterodactyl.Color = g_objectsColor;

    SetBitmapTextVerticalColor(currentScore, g_objectsColor);
    SetBitmapTextVerticalColor(highScore, g_objectsColor);

    // Update dynamic buffer data changes
    UpdateVertexData(touchHint);
    UpdateVertexData(dino);
    UpdateVertexData(ground);
    UpdateVertexData(crexLogo);
    UpdateVertexData(developerInfo);
    UpdateVertexData(gameOver);
    UpdateVertexData(retry);
    UpdateVertexData(highIndicator);
    UpdateVertexData(moon);
    UpdateVertexData(pterodactyl);

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

    DestroyBitmapText(currentScore);

    // After GPU unbind buffer data, make sure to deallocate the used heap memory
    delete[] g_spritesIndices;
    delete[] g_spritesVertices;
}

void SetupSprites()
{
    // Moon

    moon.Id       = g_spriteId++;
    moon.Scale    = { g_commonScale };
    moon.TexRect  = { 1154.0f, 2.0f, 40, 80 };
    moon.Size     = { (float)moon.TexRect.Width, (float)moon.TexRect.Height };
    moon.Position = g_moonPos;
    moon.Color    = g_whiteColor;

    // Clouds

    for (uint32_t index = 0; index < g_maxClouds; ++index)
    {
        const float maxCloudRangeY = (float)GenerateRandomNumRange(g_cloudsMaxUpRange, g_cloudsMaxDownRange);

        clouds[index].Id       = g_spriteId++;
        clouds[index].Scale    = { g_commonScale };
        clouds[index].TexRect  = { 166.0f, 0.0f, 92, 29 };
        clouds[index].Size     = { (float)clouds[index].TexRect.Width, (float)clouds[0].TexRect.Height };
        clouds[index].Position = { g_gameWorkRes.X + (index * g_cloudsDistance), maxCloudRangeY };
        clouds[index].Color    = g_objectsColor;

        UpdateVertexData(clouds[index]);
    }

    // Dino

    dino.Id       = g_spriteId++;
    dino.Scale    = { g_commonScale };
    dino.TexRect  = { 1680.0f, 4.0f, 81, 92 };
    dino.Size     = { (float)dino.TexRect.Width, (float)dino.TexRect.Height };
    dino.Color    = g_objectsColor;
    dino.Position = { g_dinoPosX, 0.0f };

    // Ground

    ground.Id       = g_spriteId++;
    ground.Scale    = { g_commonScale };
    ground.TexRect  = { 0.0f, 103.0f, 2446 * 2, 26 };
    ground.Size     = { (float)ground.TexRect.Width, (float)ground.TexRect.Height };
    ground.Position = { 0.0f, g_gameWorkRes.Y - (ground.Size.Y * ground.Scale.Y) - 50.0f };
    ground.Color    = g_objectsColor;

    SetObjectAboveGround(dino);

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

    const float developerInfoX = g_gameWorkRes.X - (developerInfo.Size.X * developerInfo.Scale.X) - 15.0f;
    const float developerInfoY = g_gameWorkRes.Y - (developerInfo.Size.Y * developerInfo.Scale.Y) - 15.0f;

    developerInfo.Position = { developerInfoX, developerInfoY };

    // Touch Hint

    touchHint.Id       = g_spriteId++;
    touchHint.Scale    = { g_touchHintScale, g_touchHintScale };
    touchHint.TexRect  = { 1487.0f, 69.0f, 123, 11 };
    touchHint.Size     = { (float)touchHint.TexRect.Width, (float)touchHint.TexRect.Height };
    touchHint.Position = g_touchHintPos;
    touchHint.Color    = g_whiteColor;

    // Cactus

    for (uint32_t index = 0; index < g_maxCactus; ++index)
    {
        const uint32_t cactusRect = GenerateRandomNumRange(0, 4);
        const float offset = 0.1f * GenerateRandomNumRange(7, 11);

        cactus[index].Id       = g_spriteId++;
        cactus[index].Scale    = { g_commonScale };
        cactus[index].TexRect  = g_cactusRect[cactusRect];
        cactus[index].Size     = { (float)cactus[index].TexRect.Width, (float)cactus[index].TexRect.Height };
        cactus[index].Position = { (g_gameWorkRes.X + 200.0f) * (index + 1), 0.0f };
        cactus[index].Color    = g_objectsColor;

        SetObjectAboveGround(cactus[index]);
        UpdateVertexData(cactus[index]);
    }

    // Game Over

    gameOver.Id       = g_spriteId++;
    gameOver.Scale    = { g_commonScale };
    gameOver.TexRect  = { 1293.0f, 28.0f, 381, 21 };
    gameOver.Size     = { (float)gameOver.TexRect.Width, (float)gameOver.TexRect.Height };
    gameOver.Position = { -g_gameWorkRes.X, 0.0f };
    gameOver.Color    = g_objectsColor;

    // Retry button

    retry.Id       = g_spriteId++;
    retry.Scale    = { g_commonScale };
    retry.TexRect  = { 3.0f, 3.0f, 68, 60 };
    retry.Size     = { (float)retry.TexRect.Width, (float)retry.TexRect.Height };
    retry.Position = { -g_gameWorkRes.X, 0.0f };
    retry.Color    = g_objectsColor;

    // High Score Indicator

    highIndicator.Id       = g_spriteId++;
    highIndicator.Scale    = { g_scoreIndicatorScale };
    highIndicator.TexRect  = { 1494.0f, 2.0f, 38, 21 };
    highIndicator.Size     = { (float)highIndicator.TexRect.Width, (float)highIndicator.TexRect.Height };
    highIndicator.Position = { g_highIndicatorPos.X, -g_gameWorkRes.Y };
    highIndicator.Color    = g_objectsColor;

    // High Score Indicator

    pterodactyl.Id       = g_spriteId++;
    pterodactyl.Scale    = { g_commonScale };
    pterodactyl.TexRect  = { 264.0f, 6.0f, 84, 72 };
    pterodactyl.Size     = { (float)pterodactyl.TexRect.Width, (float)pterodactyl.TexRect.Height };
    pterodactyl.Position = { g_gameWorkRes.X * (float)GenerateRandomNumRange(2, 6), 470.0f };
    pterodactyl.Color    = g_objectsColor;

    UpdateVertexData(dino);
    UpdateVertexData(ground);
    UpdateVertexData(crexLogo);
    UpdateVertexData(developerInfo);
    UpdateVertexData(touchHint);
    UpdateVertexData(gameOver);
    UpdateVertexData(retry);
    UpdateVertexData(highIndicator);
    UpdateVertexData(moon);
    UpdateVertexData(pterodactyl);
}

void UpdateVertexData(const BatchedSprite& sprite)
{
    const Vec2 workResScale = gfxGetWorkResScale();

    const float texWidth  = (float)g_spritesTex.Width;
    const float texHeight = (float)g_spritesTex.Height;

    // Setup position and size based on the work resolution
    // This is a personal design for me as it stands for "uniform resolution" in 2D rendering

    const float posX = sprite.Position.X * workResScale.X;
    const float posY = sprite.Position.Y * workResScale.Y;
    const float posSizeX = (sprite.Position.X + sprite.Size.X * sprite.Scale.X) * workResScale.X;
    const float posSizeY = (sprite.Position.Y + sprite.Size.Y * sprite.Scale.Y) * workResScale.Y;

    // Normalize texture coordinates

    const float texWidthX  = sprite.TexRect.X / texWidth;
    const float texHeightY = sprite.TexRect.Y / texHeight;

    const float texWidthOffsetX  = (sprite.TexRect.X + sprite.TexRect.Width) / texWidth;
    const float texHeightOffsetY = (sprite.TexRect.Y + sprite.TexRect.Height) / texHeight;

    float r, g, b, a;
    DwordToColorNormalized(sprite.Color, r, g, b, a);

    // Batch vertices by id

    g_spritesVertices[ 4 * sprite.Id + 0 ] = { { posX    , posY    , 0.0f }, { r, g, b, a }, { texWidthX      , texHeightY       } };
    g_spritesVertices[ 4 * sprite.Id + 1 ] = { { posSizeX, posSizeY, 0.0f }, { r, g, b, a }, { texWidthOffsetX, texHeightOffsetY } };
    g_spritesVertices[ 4 * sprite.Id + 2 ] = { { posX    , posSizeY, 0.0f }, { r, g, b, a }, { texWidthX      , texHeightOffsetY } };
    g_spritesVertices[ 4 * sprite.Id + 3 ] = { { posSizeX, posY    , 0.0f }, { r, g, b, a }, { texWidthOffsetX, texHeightY       } };
}

void SetupIndexData()
{
    for (uint32_t index = 0; index < g_maxSprites; ++index)
    {
        // Batch indices by id

        g_spritesIndices[ 6 * index + 0 ] = 4 * index + 0;
        g_spritesIndices[ 6 * index + 1 ] = 4 * index + 1;
        g_spritesIndices[ 6 * index + 2 ] = 4 * index + 2;
        g_spritesIndices[ 6 * index + 3 ] = 4 * index + 0;
        g_spritesIndices[ 6 * index + 4 ] = 4 * index + 3;
        g_spritesIndices[ 6 * index + 5 ] = 4 * index + 1;
    }
}

void FlushBufferData()
{
    gfxUpdateVertexBuffer(g_spritesVertices, g_vertexBufferSize, g_spritesBuffer);
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

    // C-Rex Dead
    dinoDied.FrameStep = 1.0f;
    dinoDied.Frames.push_back({ 2033.0f, 5.0f, 80, 86 });

    // Pterodactyl default
    pterodactylAnim.FrameStep = 0.1f;
    pterodactylAnim.Frames.push_back({ 264.0f, 6.0f, 84, 72 });
    pterodactylAnim.Frames.push_back({ 356.0f, 6.0f, 84, 72 });
}

void SetObjectAboveGround(BatchedSprite& object)
{
    const float groundBottom = ground.Position.Y + ground.Size.Y * ground.Scale.Y;
    const float objSize = object.Size.Y * object.Scale.Y;

    object.Position.Y = groundBottom - objSize;
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

bool HasAABBCollided(const BatchedSprite& lhe, const BatchedSprite& rhe, const Vec2 lheRectThreshold, const Vec2 rheRectThreshold)
{
    const Vec2 pos1 = lhe.Position;
    const Vec2 pos2 = rhe.Position;
    const Vec2 size1 = Vec2(lhe.Size.X * lhe.Scale.X, lhe.Size.Y * lhe.Scale.Y) * lheRectThreshold;
    const Vec2 size2 = Vec2(rhe.Size.X * rhe.Scale.X, rhe.Size.Y * rhe.Scale.Y) * rheRectThreshold;

    if (pos1.X < pos2.X + size2.X && pos1.X + size1.X > pos2.X &&	// Horizontal check
        pos1.Y < pos2.Y + size2.Y && pos1.Y + size1.Y > pos2.Y)		// Vertical check
    {
        return true;
    }

    return false;
}

bool CheckTouchAgainstSprite(const BatchedSprite& sprite)
{
    const TouchScreenId id = TouchScreenId::Touch;

    const float touchX = getTouchScreenX(id) * g_gameWorkRes.X;
    const float touchY = getTouchScreenY(id) * g_gameWorkRes.Y;

    // Same as AABB collision, but with touchscreen props

    const Vec2 pos1 = sprite.Position;
    const Vec2 pos2 = { touchX, touchY };
    const Vec2 size1 = { sprite.Size.X * sprite.Scale.X, sprite.Size.Y * sprite.Scale.Y };
    const Vec2 size2 = { 1.0f, 1.0f };

    if (pos1.X < pos2.X + size2.X && pos1.X + size1.X > pos2.X &&	// Horizontal check
        pos1.Y < pos2.Y + size2.Y && pos1.Y + size1.Y > pos2.Y)		// Vertical check
    {
        return true;
    }

    return false;
}

void RestartObjectsPosition()
{
    SetObjectAboveGround(dino);

    for (uint32_t index = 0; index < g_maxCactus; ++index)
    {
        const float offset = 0.1f * GenerateRandomNumRange(7, 11);
        cactus[index].Position.X = (g_gameWorkRes.X + 200.0f) * (index + 1);// * offset;
    }

    pterodactyl.Position.X = g_gameWorkRes.X * (float)GenerateRandomNumRange(2, 6);
}

void Uint32ToStr(const uint32_t integer, char* buffer, const uint32_t size)
{
    // Max size of 99 char's array in this project
    if (size > 99) {
        return;
    }

    char format[6];
    sprintf(format, "%s%d%s", "%0", size, "d");

    sprintf(buffer, format, integer);
}

void SetupBitmapText(BitmapText& text, const Vec2& position, const Vec2& scale, const uint32_t glyphCount, const Rect2D& base)
{
    text.GlyphCount  = glyphCount;
    text.GlyphOffset = base.Width;
    text.BaseRect    = base;

    text.Glyphs = new BatchedSprite[glyphCount];

    for (uint32_t index = 0; index < glyphCount; ++index)
    {
        const Vec2 finalPos = { position.X + ((base.Width * scale.X) * index), position.Y };

        text.Glyphs[index].Id       = g_spriteId++;
        text.Glyphs[index].Position = finalPos;
        text.Glyphs[index].TexRect  = base;
        text.Glyphs[index].Scale    = scale;
        text.Glyphs[index].Size     = { (float)base.Width, (float)base.Height };
        text.Glyphs[index].Color    = g_objectsColor;

        UpdateVertexData(text.Glyphs[index]);
    }
}

void DestroyBitmapText(BitmapText& text)
{
    text.GlyphCount = 0;
    text.GlyphOffset = 0;

    delete[] text.Glyphs;
}

void SetBitmapTextString(BitmapText& text, const char* buffer, const uint32_t size)
{
    if (size > text.GlyphCount) {
        return;
    }

    for (uint32_t index = 0; index < size; ++index)
    {
        int32_t number = (int32_t)buffer[index] - '0';

        if (number < 0 || number > 9) {
            continue;
        }

        text.Glyphs[index].TexRect.X = text.BaseRect.X + (number * text.GlyphOffset);
        UpdateVertexData(text.Glyphs[index]);
    }
}

void SetBitmapTextVerticalPos(BitmapText& text, const float position)
{
    for (uint32_t index = 0; index < text.GlyphCount; ++index) {
        text.Glyphs[index].Position.Y = position;
        UpdateVertexData(text.Glyphs[index]);
    }
}

void SetBitmapTextVerticalColor(BitmapText& text, const uint32_t color)
{
    for (uint32_t index = 0; index < text.GlyphCount; ++index) {
        text.Glyphs[index].Color = color;
        UpdateVertexData(text.Glyphs[index]);
    }
}

float Lerp(const float lhe, const float rhe, const float delta)
{
    return (1.0f - delta) * lhe + delta * rhe;
}