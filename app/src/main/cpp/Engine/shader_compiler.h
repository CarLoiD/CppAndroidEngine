#ifndef SHADER_COMPILER_H
#define SHADER_COMPILER_H

#include <GLES2/gl2.h>

typedef enum class SHADER_TYPE : uint32_t {
    VertexShader = GL_VERTEX_SHADER,
    PixelShader = GL_FRAGMENT_SHADER
} ShaderType;

typedef struct {
    uint32_t Id;
    ShaderType Type;
} Shader;

void CompileShader(const char* code, const ShaderType& type, Shader& object);

#endif // SHADER_COMPILER_H