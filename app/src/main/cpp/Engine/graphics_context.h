#ifndef GRAPHICS_CONTEXT_H
#define GRAPHICS_CONTEXT_H

#include "primitive_type.h"
#include "vector.h"

#include <GLES2/gl2.h>
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
    uint32_t Width;
    uint32_t Height;
};

#endif // GRAPHICS_CONTEXT_H