#include "texture2d.h"

#define STB_IMAGE_STATIC
#include "stb_image/stb_image.h"

void CreateTexture2D(Asset& asset, Texture2D& texture, const bool filtered, const bool repeat)
{
    glGenTextures(1, &texture.Id);
    glBindTexture(GL_TEXTURE_2D, texture.Id);

    const uint32_t length = asset.GetLength();
    uint8_t* buffer = new uint8_t[length];

    asset.Read((char*)buffer, length);
    texture.Data = stbi_load_from_memory(buffer, length, (int32_t*)&texture.Width,
                                        (int32_t*)&texture.Height, nullptr, 4);

    delete[] buffer;

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.Width, texture.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.Data);

    const int32_t filter = filtered ? GL_LINEAR : GL_NEAREST;
    const int32_t wrap = repeat ? GL_REPEAT : GL_CLAMP_TO_EDGE;

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);


    glBindTexture(GL_TEXTURE_2D, 0);
}

void DestroyTexture2D(Texture2D& texture)
{
    stbi_image_free(texture.Data);
    glDeleteTextures(1, &texture.Id);

    texture.Width  = 0;
    texture.Height = 0;
}

void BindTexture2D(const Texture2D& texture)
{
    glBindTexture(GL_TEXTURE_2D, texture.Id);
}