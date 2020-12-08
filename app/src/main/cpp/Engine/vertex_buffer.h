#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include "vertex_layout.h"

#include <GLES2/gl2.h>
#include <cstdint>

typedef struct {
    uint32_t Id;
    uint32_t Stride;
    uint32_t Size;
    void* Data;
} VertexBuffer;

void CreateVertexBuffer(const uint32_t program, const VertexElement* layout, const uint32_t count, VertexBuffer& buffer, const bool dynamic);
void DestroyVertexBuffer(const VertexBuffer& buffer);
void BindVertexBuffer(const VertexBuffer& buffer);
void UpdateVertexBuffer(const void* data, const uint32_t size, const VertexBuffer& buffer);

#endif // VERTEX_BUFFER_H