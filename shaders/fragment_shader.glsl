uniform sampler2D texture;
uniform vec3 color;

void main()
{
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
    gl_FragColor = gl_Color * pixel * vec4(color, 1.0);
}
