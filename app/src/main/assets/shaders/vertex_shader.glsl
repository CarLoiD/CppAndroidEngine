attribute vec4 Position;
attribute vec4 Color;
attribute vec2 TexCoord;

varying vec4 v_Color;
varying vec2 v_TexCoord;

uniform mat4 ModelViewProj;

void main()
{
    gl_Position = Position * ModelViewProj;
    v_TexCoord = TexCoord;
    v_Color = Color;
}