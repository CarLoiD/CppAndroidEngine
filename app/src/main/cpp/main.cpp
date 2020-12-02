#include "engine.h"

const char* g_vsCode = R"(
attribute vec4 Position;
attribute vec4 Color;

varying vec4 v_Color;

uniform mat4 ModelViewProj;

void main()
{
    gl_Position = ModelViewProj * Position;
    v_Color = Color;
}
)";

const char* g_psCode = R"(
precision mediump float;

varying vec4 v_Color;

void main()
{
    const vec4 green = vec4(0.0, 1.0, 0.0, 1.0);
    gl_FragColor = v_Color;
}
)";

#define ASPECT_16_9 16.0f / 9.0f

float g_angle = 0.0f;

typedef struct {
    float Position[3];
    float Color[4];
} VertexInput;

const VertexInput data[] = {
    { { -0.50f ,-0.25f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
    { {  0.00f , 0.25f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
    { {  0.50f ,-0.25f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } },
}; const uint32_t nVertices = sizeof(data) / sizeof(VertexInput);

VertexBuffer vBuffer;

void Application::Create()
{
    Shader vertexShader;
    Shader pixelShader;

    gfxCompileShaderFromCode(g_vsCode, ShaderType::VertexShader, vertexShader);
    gfxCompileShaderFromCode(g_psCode, ShaderType::PixelShader, pixelShader);

    gfxBindShader(vertexShader);
    gfxBindShader(pixelShader);

    const VertexElement layout[] = {
        VertexElement::Position,
        VertexElement::Color
    }; const uint32_t nLayout = sizeof(layout) / sizeof(VertexElement);

    vBuffer.Stride = sizeof(VertexInput);
    vBuffer.Size   = sizeof(data);
    vBuffer.Data   = (void*)data;

    gfxCreateVertexBuffer(layout, nLayout, vBuffer);
    gfxBindVertexBuffer(vBuffer);

    gfxSetPrimitiveType(PrimitiveType::TriangleList);
    gfxEnableDepthBufferTesting();
}

void Application::Update(const float deltaTime)
{
    const TouchScreenId id = TouchScreenId::Touch;

    const float posX = getTouchScreenX(id);
    const float posY = getTouchScreenY(id);

    if (posX > 0.0f || posY > 0.0f) {
        g_angle += deltaTime * 2.0f;
    }

    gfxSetWorldMatrix(mtxScale({ 4.0f, 4.0f, 4.0f }) * mtxRotateZ(g_angle));
    gfxSetViewMatrix(mtxLookAt({ 0.0f, 1.0f, -5.0f }, { 0.0f, 0.7f, 0.0f }, { 0.0f, 1.0f, 0.0f }));
    gfxSetProjectionMatrix(mtxPerspective(70.0f, ASPECT_16_9, 0.001f, 1000.0f));

    gfxFlushMVPMatrix();

    gfxClearBackBuffer(45.0f, 45.0f, 45.0f);
    gfxDraw(0, nVertices);
}

void Application::Destroy()
{
    gfxDestroyVertexBuffer(vBuffer);
}