#ifndef SPRITE_H
#define SPRITE_H

#include "vertex_buffer.h"
#include "index_buffer.h"
#include "texture2d.h"
#include "gfx_math.h"

typedef struct {
    Vec2 Position;
    Vec2 Size;
    Rect2D TexRect;
    Texture2D* Texture2D;
    VertexBuffer VertexBuffer;
    IndexBuffer IndexBuffer;
} Sprite;

void CreateSprite(Texture2D& texture, const Vec2& workRes, Sprite& sprite, const Vec2& position = { 0.0f, 0.0f });
void DestroySprite(Sprite& sprite);
void UpdateSpriteBufferData(Sprite& sprite);

#endif // SPRITE_H