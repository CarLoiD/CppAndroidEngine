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

float g_angle = 0.0f;

const float width = 762.0f;
const float height = 87.0f;

const float entityColor = 0.31f;

typedef struct {
    float Position[3];
    float Color[4];
    float TexCoord[2];
} VertexInput;

const VertexInput data[] = {
    { {  0.0f,   0.0f, 0.0f }, { entityColor, entityColor, entityColor, 1.0f }, { 0.0f, 0.0f } },
    { { width, height, 0.0f }, { entityColor, entityColor, entityColor, 1.0f }, { 1.0f, 1.0f } },
    { {  0.0f, height, 0.0f }, { entityColor, entityColor, entityColor, 1.0f }, { 0.0f, 1.0f } },
    { { width,   0.0f, 0.0f }, { entityColor, entityColor, entityColor, 1.0f }, { 1.0f, 0.0f } },
};

const uint16_t indices[] = {
    0, 1, 2,
    0, 3, 1
}; const uint32_t nIndices = sizeof(indices) / sizeof(uint16_t);

VertexBuffer vBuffer;
IndexBuffer iBuffer;

Texture2D catInLoveTex;

void Application::Create()
{
    Shader vertexShader;
    Shader pixelShader;

    glViewport(0.0f, 0.0f, (float)gfxGetDisplayWidth(), (float)gfxGetDisplayHeight());

    gfxCompileShaderFromCode(g_vsCode, ShaderType::VertexShader, vertexShader);
    gfxCompileShaderFromCode(g_psCode, ShaderType::PixelShader, pixelShader);

    gfxBindShader(vertexShader);
    gfxBindShader(pixelShader);

    const VertexElement layout[] = {
        VertexElement::Position,
        VertexElement::Color,
        VertexElement::TexCoord
    }; const uint32_t nLayout = sizeof(layout) / sizeof(VertexElement);

    // Vertex buffer setting
    vBuffer.Stride = sizeof(VertexInput);
    vBuffer.Size   = sizeof(data);
    vBuffer.Data   = (void*)data;

    gfxCreateVertexBuffer(layout, nLayout, vBuffer, true);

    iBuffer.Stride = sizeof(uint32_t);
    iBuffer.Size   = nIndices;
    iBuffer.Data   = (void*)indices;

    gfxCreateIndexBuffer(iBuffer);

    gfxBindVertexBuffer(vBuffer);
    gfxBindIndexBuffer(iBuffer);

    gfxSetPrimitiveType(PrimitiveType::TriangleList);
    gfxDisableDepthBufferTesting();

    gfxCreateTexture2D("textures/dino.png", catInLoveTex, false);
    gfxBindTexture2D(catInLoveTex);
}

void Application::Update(const float deltaTime)
{
    if (hasTouchEvent()) {
        g_angle += deltaTime;
    }

    const Vec2 displayRes = { (float)gfxGetDisplayWidth(), (float)gfxGetDisplayHeight() };
    const ScreenRect projRect = { 0.0f, displayRes.X, displayRes.Y, 0.0f };

    gfxSetWorldMatrix(mtxRotateZ(g_angle));
    gfxSetViewMatrix(mtxIdentity());
    gfxSetProjectionMatrix(mtxOrthoOffCenter(projRect, 0.0f, 1.0f));

    gfxFlushMVPMatrix();

    gfxClearBackBuffer(255.0f, 255.0f, 255.0f);
    gfxDrawIndexed(nIndices);
}

void Application::Destroy()
{
    gfxDestroyTexture2D(catInLoveTex);

    gfxDestroyIndexBuffer(iBuffer);
    gfxDestroyVertexBuffer(vBuffer);
}