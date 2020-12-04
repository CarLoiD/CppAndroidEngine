#ifndef INDEX_BUFFER_H
#define INDEX_BUFFER_H

#include <GLES2/gl2.h>
#include <cstdint>

typedef struct {
    uint32_t Id;
    uint32_t Size;
    uint32_t Stride;
    void* Data;
} IndexBuffer;

void CreateIndexBuffer(IndexBuffer& buffer);
void DestroyIndexBuffer(const IndexBuffer& buffer);
void BindIndexBuffer(const IndexBuffer& buffer);

#endif // INDEX_BUFFER_H