#include "shader_compiler.h"

#include "utils.h"

void DebugShaderCompileError(const uint32_t id)
{
    int32_t errorStrSize = 0;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &errorStrSize);

    if (errorStrSize)
    {
        char* buffer = new char[errorStrSize];

        glGetShaderInfoLog(id, errorStrSize, nullptr, buffer);
        LogError("Shader Compilation Error: %s", buffer);

        delete[] buffer;
    }
}

void CompileShader(const char* code, const ShaderType& type, Shader& object)
{
    object.Type = type;
    object.Id = glCreateShader((uint32_t)type);

    if (object.Id)
    {
        glShaderSource(object.Id, 1, &code, nullptr);
        glCompileShader(object.Id);

        int32_t compileStatus = 0;
        glGetShaderiv(object.Id, GL_COMPILE_STATUS, &compileStatus);

        if (!compileStatus)
        {
            DebugShaderCompileError(object.Id);
            glDeleteShader(object.Id);

            object.Id = 0;
        }
    }
}