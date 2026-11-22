uniform sampler2D texture;
uniform float time;
uniform int state;

void main()
{
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
    float multiplier = 1.0;

    if (state == 1 || state == 3) {
        multiplier = time;
        if (multiplier < 0.33) {
            multiplier = 0.33;
        }
    }
    else if (state == 2) {
        multiplier = 0.33;
    }

    gl_FragColor = gl_Color * pixel * vec4(vec3(multiplier), 1.0);
}
