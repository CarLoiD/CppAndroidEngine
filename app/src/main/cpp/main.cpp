#include "engine.h"

const char* g_vsCode = R"(
attribute vec4 Position;
attribute vec4 Color;
attribute vec2 TexCoord;

varying vec4 v_Color;
varying vec2 v_TexCoord;

uniform mat4 ModelViewProj;

void main()
{
    gl_Position = Position * ModelViewProj;
    v_TexCoord = TexCoord;
    v_Color = Color;
}
)";

const char* g_psCode = R"(
precision mediump float;

varying vec4 v_Color;
varying vec2 v_TexCoord;

uniform sampler2D tex2d;

void main()
{
    vec4 texColor = texture2D(tex2d, v_TexCoord);

    if (texColor.a < 0.1) {
        discard;
    }

    gl_FragColor = texColor * v_Color;
}
)";

constexpr const float g_dinoScale = 1.5f;
constexpr const float g_groundScale = 1.0f;

uint32_t g_clearColor = 0xFFFFFFFF;
uint32_t g_objectsColor = 0x505050FF;

Texture2D dinoTex;
Texture2D groundTex;

Sprite dino;
Sprite ground;

void LoadTextures();
void LoadSprites();
void DestroyTextures();
void DestroySprites();

void Application::Create()
{
    gfxSetWorkResolution({ 1280.0f, 720.0f });  // 720p as default work resolution

    Shader vertexShader;
    Shader pixelShader;

    glViewport(0.0f, 0.0f, (float)gfxGetDisplayWidth(), (float)gfxGetDisplayHeight());

    gfxCompileShaderFromCode(g_vsCode, ShaderType::VertexShader, vertexShader);
    gfxCompileShaderFromCode(g_psCode, ShaderType::PixelShader, pixelShader);

    gfxBindShader(vertexShader);
    gfxBindShader(pixelShader);

    gfxSetPrimitiveType(PrimitiveType::TriangleList);
    gfxDisableDepthBufferTesting();

    LoadTextures();
    LoadSprites();

    gfxSpriteSetPosition(dino, { 0.0f, 200.0f });

    const Vec2 displayRes = { (float)gfxGetDisplayWidth(), (float)gfxGetDisplayHeight() };
    const ScreenRect projRect = { 0.0f, displayRes.X, displayRes.Y, 0.0f };

    gfxSetWorldMatrix(mtxIdentity());
    gfxSetViewMatrix(mtxIdentity());
    gfxSetProjectionMatrix(mtxOrthoOffCenter(projRect, 0.0f, 1.0f));
}

void Application::Update(const float deltaTime)
{
    if (hasTouchEvent()) {

    }

    gfxFlushMVPMatrix();

    gfxClearBackBuffer(g_clearColor);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(SpriteVertex), (void*)dino.BufferData->Position);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex), (void*)dino.BufferData->Color);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex), (void*)dino.BufferData->TexCoord);

    gfxDrawSprite(dino);


    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(SpriteVertex), (void*)ground.BufferData->Position);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex), (void*)ground.BufferData->Color);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(SpriteVertex), (void*)ground.BufferData->TexCoord);

    gfxDrawSprite(ground);
}

void Application::Destroy()
{
    DestroySprites();
    DestroyTextures();
}

void LoadTextures()
{
    gfxCreateTexture2D("textures/dino.png", dinoTex, false);
    gfxCreateTexture2D("textures/ground.png", groundTex, false, true);
}

void LoadSprites()
{
    // Dino
    gfxCreateSprite(dino, dinoTex);
    gfxSpriteSetColor(dino, g_objectsColor);
    gfxSpriteSetScale(dino, { g_dinoScale, g_dinoScale });
//    gfxSpriteSetTexRect(dino, { 0.0f, 0.0f, 84, 87 });

//    // Ground
    gfxCreateSprite(ground, groundTex);
    gfxSpriteSetColor(ground, g_objectsColor);
//    gfxSpriteSetPosition(ground, { 0.0f, (float)gfxGetDisplayHeight() - ground.Size.Y });
}

void DestroyTextures()
{
    gfxDestroyTexture2D(dinoTex);
    gfxDestroyTexture2D(groundTex);
}

void DestroySprites()
{
    gfxDestroySprite(dino);
    gfxDestroySprite(ground);
}