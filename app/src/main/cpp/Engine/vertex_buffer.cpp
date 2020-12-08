#include "vertex_buffer.h"

#include "utils.h"

inline void DefineVertexAttribPointer(const uint32_t program, const VertexElement& element,
                                      const uint32_t stride, uint16_t& offset)
{
    // Element attribute location
    const uint32_t attribLocation = [&]() {
        uint32_t tmpLocation;
        switch (element) {
            case VertexElement::Position:
                tmpLocation = (uint32_t)glGetAttribLocation(program, "Position");
                break;
            case VertexElement::Color:
                tmpLocation = (uint32_t)glGetAttribLocation(program, "Color");
                break;
            case VertexElement::TexCoord:
                tmpLocation = (uint32_t)glGetAttribLocation(program, "TexCoord");
                break;
            case VertexElement::Normal:
                tmpLocation = (uint32_t)glGetAttribLocation(program, "Normal");
                break;
        } return tmpLocation;
    }();

    LogDebug("AttribLocation -> %d", attribLocation);

    // Element size
    const uint32_t elementSize = [&]() {
        uint32_t tmpSize;
        switch (element) {
            case VertexElement::Position:
                tmpSize = 3;    // XYZ
                break;
            case VertexElement::Color:
                tmpSize = 4;    // RGBA
                break;
            case VertexElement::TexCoord:
                tmpSize = 2;    // UV
                break;
            case VertexElement::Normal:
                tmpSize = 3;    // N (XYZ)
                break;
        } return tmpSize;
    }();

    glVertexAttribPointer(attribLocation, elementSize, GL_FLOAT, GL_FALSE, stride, (void*)offset);
    offset += elementSize * sizeof(float);
}

void CreateVertexBuffer(const uint32_t program, const VertexElement* layout,
                        const uint32_t count, VertexBuffer& buffer, const bool dynamic)
{
    const uint32_t usage = dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;

    glGenBuffers(1, &buffer.Id);

    glBindBuffer(GL_ARRAY_BUFFER, buffer.Id);
    glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)buffer.Size, buffer.Data, usage);

    uint16_t offsetPointerValue = 0;

    for (uint16_t index = 0; index < count; ++index) {
        glEnableVertexAttribArray(index);
        DefineVertexAttribPointer(program, layout[index], buffer.Stride, offsetPointerValue);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void DestroyVertexBuffer(const VertexBuffer& buffer)
{
    glDeleteBuffers(1, &buffer.Id);
}

void BindVertexBuffer(const VertexBuffer& buffer)
{
    glBindBuffer(GL_ARRAY_BUFFER, buffer.Id);
}

void UpdateVertexBuffer(const void* data, const uint32_t size, const VertexBuffer& buffer)
{
    glBindBuffer(GL_ARRAY_BUFFER, buffer.Id);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}