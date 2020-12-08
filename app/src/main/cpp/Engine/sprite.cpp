#include "sprite.h"

typedef struct {
    float Position[3];
    float Color[4];
    float TexCoord[2];
} VertexInput;

void CreateSprite(Texture2D& texture, const Vec2& workRes, Sprite& sprite, const Vec2& position)
{
    sprite.Texture2D = &texture;
    sprite.Position = position;
    sprite.Size =
}

void DestroySprite(Sprite& sprite)
{

}

void UpdateSpriteBufferData(Sprite& sprite)
{

}