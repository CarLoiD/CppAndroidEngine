#include "sprite.h"

inline void SetupVertexData(Sprite& sprite, const Vec2& workResScale)
{
    const float texWidth  = (float)sprite.Texture->Width;
    const float texHeight = (float)sprite.Texture->Height;

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

    sprite.BufferData[0] = { { posX    , posY    , 0.0f }, { r, g, b, a }, { texWidthX      , texHeightY       } };
    sprite.BufferData[1] = { { posSizeX, posSizeY, 0.0f }, { r, g, b, a }, { texWidthOffsetX, texHeightOffsetY } };
    sprite.BufferData[2] = { { posX    , posSizeY, 0.0f }, { r, g, b, a }, { texWidthX      , texHeightOffsetY } };
    sprite.BufferData[3] = { { posSizeX, posY    , 0.0f }, { r, g, b, a }, { texWidthOffsetX, texHeightY       } };
}

inline void InitializeSprite(Sprite& sprite, const uint32_t program, const Vec2& workResScale)
{
    SetupVertexData(sprite, workResScale);

    const VertexElement layout[] = {
        VertexElement::Position,
        VertexElement::Color,
        VertexElement::TexCoord
    }; const uint32_t nLayout = sizeof(layout) / sizeof(VertexElement);

    sprite.VertexBuffer.Stride = sizeof(SpriteVertex);
    sprite.VertexBuffer.Size   = sizeof(sprite.BufferData);
    sprite.VertexBuffer.Data   = (void*)sprite.BufferData;

    CreateVertexBuffer(program, layout, nLayout, sprite.VertexBuffer, true);

    const uint16_t indices[] = { 0, 1, 2, 0, 3, 1 };

    sprite.IndexBuffer.Stride = sizeof(uint16_t);
    sprite.IndexBuffer.Size   = sizeof(indices) / sizeof(uint16_t);
    sprite.IndexBuffer.Data   = (void*)indices;

    CreateIndexBuffer(sprite.IndexBuffer);
}

inline void UpdateVertexBufferData(Sprite& sprite)
{
    UpdateVertexBuffer(sprite.BufferData, sizeof(sprite.BufferData), sprite.VertexBuffer);
}

void CreateSprite(Sprite& sprite, const uint32_t program, const Vec2& workResScale, Texture2D& texture, const Vec2& position)
{
    if (texture.Data != nullptr)
    {
        sprite.Texture  = &texture;
        sprite.Position = position;
        sprite.Size     = { (float)texture.Width, (float)texture.Height };
        sprite.TexRect  = { 0.0f, 0.0f, texture.Width, texture.Height };
        sprite.Color    = 0xFFFFFFFF;

        InitializeSprite(sprite, program, workResScale);
    } else {
        LogError("gfxError: Texture2D might not have been initialized :: CreateSprite()");
    }
}

void DestroySprite(Sprite& sprite)
{
    DestroyIndexBuffer(sprite.IndexBuffer);
    DestroyVertexBuffer(sprite.VertexBuffer);

    sprite.Texture  = nullptr;
    sprite.TexRect  = { 0.0f, 0.0f, 0, 0 };
    sprite.Position = { 0.0f, 0.0f };
    sprite.Size     = { 0.0f, 0.0f };
    sprite.Color    = 0x00000000;
}

void SpriteSetPosition(Sprite& sprite, const Vec2& position)
{
    if (position != sprite.Position)
    {
        sprite.Position = position;
        sprite.NeedBufferUpdate = true;
    }
}

void SpriteSetSize(Sprite& sprite, const Vec2& size)
{
    if (size != sprite.Size)
    {
        sprite.Size = size;
        sprite.NeedBufferUpdate = true;
    }
}

void SpriteSetScale(Sprite& sprite, const Vec2& scale)
{
    if (scale != Vec2(1.0f, 1.0f))
    {
        sprite.Scale = scale;
        sprite.NeedBufferUpdate = true;
    }
}

void SpriteSetColor(Sprite& sprite, const uint32_t color)
{
    if (color != sprite.Color)
    {
        sprite.Color = color;
        sprite.NeedBufferUpdate = true;
    }
}

void SpriteSetTexRect(Sprite& sprite, const Rect2D& texrect)
{
    if (texrect != sprite.TexRect)
    {
        sprite.TexRect = texrect;
        sprite.Size = { (float)texrect.Width, (float)texrect.Height };

        sprite.NeedBufferUpdate = true;
    }
}

void SpriteDraw(Sprite& sprite, const Vec2& workResScale)
{
    if (sprite.NeedBufferUpdate)
    {
        SetupVertexData(sprite, workResScale);
        UpdateVertexBufferData(sprite);

        sprite.NeedBufferUpdate = false;
    }

    glBindTexture(GL_TEXTURE_2D, sprite.Texture->Id);

    BindVertexBuffer(sprite.VertexBuffer);
    BindIndexBuffer(sprite.IndexBuffer);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glBindTexture(GL_TEXTURE_2D, 0);
}