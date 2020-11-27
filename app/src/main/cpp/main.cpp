#include "engine.h"

typedef struct {
    float Position[3];
} VertexInput;

const VertexInput data[] = {
    { -0.50f ,-0.25f, 1.0f },
    {  0.00f , 0.25f, 1.0f },
    {  0.50f ,-0.25f, 1.0f },
}; const uint32_t nVertices = sizeof(data) / sizeof(VertexInput);

VertexBuffer vBuffer;

void Application::Create()
{
    Shader vertexShader;
    Shader pixelShader;

    gfxCompileShaderFromAsset("vertex_shader.vsh", ShaderType::VertexShader, vertexShader);
    gfxCompileShaderFromAsset("pixel_shader.psh", ShaderType::PixelShader, pixelShader);

    gfxBindShader(vertexShader);
    gfxBindShader(pixelShader);

    const VertexElement layout[] = { VertexElement::Position };

    vBuffer.Stride = sizeof(VertexInput);
    vBuffer.Size   = sizeof(data);
    vBuffer.Data   = (void*)data;

    gfxCreateVertexBuffer(layout, 1, vBuffer);
    gfxBindVertexBuffer(vBuffer);

    gfxSetPrimitiveType(PrimitiveType::TriangleList);
}

void Application::Update(const float deltaTime)
{
    const TouchScreenId id = TouchScreenId::Touch;

    const float posX = getTouchScreenX(id);
    const float posY = getTouchScreenY(id);

    gfxClearBackBuffer(45.0f, 45.0f, 45.0f);
    gfxDraw(0, nVertices);
}

void Application::Destroy()
{
    gfxDestroyVertexBuffer(vBuffer);
}