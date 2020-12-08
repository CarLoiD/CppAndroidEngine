#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include "asset.h"

#include <GLES2/gl2.h>
#include <cstdint>

typedef struct {
    uint32_t Id;
    uint32_t Width;
    uint32_t Height;
    uint8_t* Data;
} Texture2D;

void CreateTexture2D(Asset& asset, Texture2D& texture, const bool filtered, const bool repeat);
void DestroyTexture2D(Texture2D& texture);
void BindTexture2D(const Texture2D& texture);

#endif // TEXTURE2D_H