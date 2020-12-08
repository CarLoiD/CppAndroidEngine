#ifndef SPRITE_H
#define SPRITE_H

#include "utils.h"
#include "vertex_buffer.h"
#include "index_buffer.h"
#include "texture2d.h"
#include "gfx_math.h"

typedef struct {
    float Position[3];
    float Color[4];
    float TexCoord[2];
} SpriteVertex;

typedef struct {
    Vec2 Position;
    Vec2 Size;
    Vec2 Scale;
    Rect2D TexRect;
    uint32_t Color;
    Texture2D* Texture;
    VertexBuffer VertexBuffer;
    IndexBuffer IndexBuffer;
    SpriteVertex BufferData[4];
} Sprite;

void CreateSprite(Sprite& sprite, const uint32_t program, const Vec2& workResScale, Texture2D& texture, const Vec2& position = { 0.0f, 0.0f });
void DestroySprite(Sprite& sprite);
void SpriteSetPosition(Sprite& sprite, const Vec2& workResScale, const Vec2& position);
void SpriteSetSize(Sprite& sprite, const Vec2& workResScale, const Vec2& size);
void SpriteSetScale(Sprite& sprite, const Vec2& workResScale, const Vec2& scale);
void SpriteSetColor(Sprite& sprite, const Vec2& workResScale, const uint32_t color);
void SpriteSetTexRect(Sprite& sprite, const Vec2& workResScale, const Rect2D& texrect);
void SpriteDraw(const Sprite& sprite);

#endif // SPRITE_H