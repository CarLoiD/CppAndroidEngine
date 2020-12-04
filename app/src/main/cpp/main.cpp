#include "engine.h"

#define STB_IMAGE_STATIC
#include "stb_image/stb_image.h"

const char* g_vsCode = R"(
attribute vec4 Position;
attribute vec2 TexCoord;

varying vec2 v_TexCoord;

uniform mat4 ModelViewProj;

void main()
{
    gl_Position = Position * ModelViewProj;
    v_TexCoord = TexCoord;
}
)";

const char* g_psCode = R"(
precision mediump float;

varying vec2 v_TexCoord;

uniform sampler2D tex2d;

void main()
{
    gl_FragColor = texture2D(tex2d, v_TexCoord);
}
)";

float g_angle = 0.0f;

typedef struct {
    float Position[3];
    float TexCoord[2];
} VertexInput;

const VertexInput data[] = {
    { {  0.0f,   0.0f, 0.0f }, { 0.0f, 0.0f } },
    { {300.0f, 300.0f, 0.0f }, { 1.0f, 1.0f } },
    { {  0.0f, 300.0f, 0.0f }, { 0.0f, 1.0f } },
    { {300.0f,   0.0f, 0.0f }, { 1.0f, 0.0f } },
};

const uint16_t indices[] = {
    0, 1, 2,
    0, 3, 1
}; const uint32_t nIndices = sizeof(indices) / sizeof(uint16_t);

VertexBuffer vBuffer;
IndexBuffer iBuffer;

uint32_t g_texId;

uint8_t* tex2d;
uint32_t tex2dWidth  = 0;
uint32_t tex2dHeight = 0;

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
        VertexElement::TexCoord
    }; const uint32_t nLayout = sizeof(layout) / sizeof(VertexElement);

    // Vertex buffer setting
    vBuffer.Stride = sizeof(VertexInput);
    vBuffer.Size   = sizeof(data);
    vBuffer.Data   = (void*)data;

    gfxCreateVertexBuffer(layout, nLayout, vBuffer);

    iBuffer.Stride = sizeof(uint32_t);
    iBuffer.Size   = nIndices;
    iBuffer.Data   = (void*)indices;

    gfxCreateIndexBuffer(iBuffer);

    gfxBindVertexBuffer(vBuffer);
    gfxBindIndexBuffer(iBuffer);

    gfxSetPrimitiveType(PrimitiveType::TriangleList);
    gfxDisableDepthBufferTesting();

    glActiveTexture(GL_TEXTURE0);

    glGenTextures(1, &g_texId);
    glBindTexture(GL_TEXTURE_2D, g_texId);

    Asset shaderAsset = openAsset("cat_in_love.png");

    if (shaderAsset.IsOpen())
    {
        const uint32_t length = shaderAsset.GetLength();
        uint8_t* buffer = new uint8_t[length];

        shaderAsset.Read((char*)buffer, length);
        tex2d = stbi_load_from_memory(buffer, length, (int32_t*)&tex2dWidth, (int32_t*)&tex2dHeight, nullptr, 4);

        delete[] buffer;

        shaderAsset.Close();
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex2dWidth, tex2dHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex2d);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void Application::Update(const float deltaTime)
{
    if (hasTouchEvent()) {
        g_angle += deltaTime;
    }

    const Vec3 eye = { 0.0f, 0.0f,-2.0f };
    const Vec3 at  = { 0.0f, 0.0f, 0.0f };
    const Vec3 up  = { 0.0f, 1.0f, 0.0f };

    const Vec2 displayRes = { (float)gfxGetDisplayWidth(), (float)gfxGetDisplayHeight() };
    const ScreenRect projRect = { 0.0f, displayRes.X, displayRes.Y, 0.0f };

    gfxSetWorldMatrix(mtxRotateZ(g_angle));
    gfxSetViewMatrix(mtxIdentity());
    gfxSetProjectionMatrix(mtxOrthoOffCenter(projRect, 0.0f, 1.0f));

    gfxFlushMVPMatrix();

    gfxClearBackBuffer(45.0f, 45.0f, 45.0f);
    gfxDrawIndexed(nIndices);
}

void Application::Destroy()
{
    stbi_image_free(tex2d);
    glDeleteTextures(1, &g_texId);

    gfxDestroyIndexBuffer(iBuffer);
    gfxDestroyVertexBuffer(vBuffer);
}