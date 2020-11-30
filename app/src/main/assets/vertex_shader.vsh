attribute vec4 Position;

uniform mat4 ModelViewProj;

void main()
{
    gl_Position = ModelViewProj * Position;
}
