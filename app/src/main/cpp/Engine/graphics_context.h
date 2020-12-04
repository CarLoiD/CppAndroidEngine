#ifndef GRAPHICS_CONTEXT_H
#define GRAPHICS_CONTEXT_H

#include "primitive_type.h"
#include <cstdint>

class GraphicsContext final
{
public:
    void Create(const uint32_t width, const uint32_t height);

    void ClearBackBuffer(const float r, const float g, const float b, const float a);
    void EnableDepthBufferTesting();
    void DisableDepthBufferTesting();
    void Draw(const PrimitiveType& type, const uint32_t offset, const uint32_t count);
    void DrawIndexed(const PrimitiveType& type, const uint32_t count);

    uint32_t GetDisplayWidth() const;
    uint32_t GetDisplayHeight() const;

private:
    uint32_t mWidth;
    uint32_t mHeight;
};

#endif // GRAPHICS_CONTEXT_H