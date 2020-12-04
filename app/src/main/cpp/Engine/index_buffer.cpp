#include "index_buffer.h"

void CreateIndexBuffer(IndexBuffer& buffer)
{
    glGenBuffers(1, &buffer.Id);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.Id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(buffer.Size * buffer.Stride), buffer.Data, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void DestroyIndexBuffer(const IndexBuffer& buffer)
{
    glDeleteBuffers(1, &buffer.Id);
}

void BindIndexBuffer(const IndexBuffer& buffer)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.Id);
}