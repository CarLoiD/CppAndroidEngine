precision mediump float;

varying vec4 v_Color;

void main()
{
    const vec4 green = vec4(0.0, 1.0, 0.0, 1.0);
    gl_FragColor = v_Color;
}
