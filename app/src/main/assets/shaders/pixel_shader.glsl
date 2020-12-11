precision mediump float;

varying vec4 v_Color;
varying vec2 v_TexCoord;

uniform sampler2D tex2d;

void main()
{
    vec4 texColor = texture2D(tex2d, v_TexCoord);

    if (texColor.a < 0.1) {
        discard;
    }

    gl_FragColor = texColor * v_Color;
}