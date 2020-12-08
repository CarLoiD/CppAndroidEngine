attribute vec4 Position;
attribute vec4 Color;

varying vec4 v_Color;

uniform mat4 ModelViewProj;

void main()
{
    gl_Position = ModelViewProj * Position;
    v_Color = Color;
}
