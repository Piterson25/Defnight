uniform vec4 lightColor;
uniform vec2 lightCenter;
uniform float lightRadius;
uniform float windowSizeY;

void main()
{
    vec2 pixel = vec2(gl_FragCoord.x, windowSizeY - gl_FragCoord.y);
    float d = length(pixel - lightCenter) / lightRadius;
    float alpha = pow(max(0.0, 1.0 - d), 2.5);

    gl_FragColor = vec4(lightColor.rgb, lightColor.a * alpha);
}

