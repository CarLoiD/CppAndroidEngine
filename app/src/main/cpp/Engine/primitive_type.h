#ifndef PRIMITIVE_TYPE_H
#define PRIMITIVE_TYPE_H

#include <cstdint>
#include <GLES2/gl2.h>

typedef enum class PRIMITIVE_TYPE : uint32_t {
    PointList     = GL_POINTS,
    LineList      = GL_LINES,
    LineStrip     = GL_LINE_STRIP,
    TriangleList  = GL_TRIANGLES,
    TriangleStrip = GL_TRIANGLE_STRIP
} PrimitiveType;

#endif // PRIMITIVE_TYPE_H