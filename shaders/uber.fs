#version 330
// #ifdef GL_FRAGMENT_PRECISION_HIGH
// precision highp float;
// #else
// precision mediump float;
// #endif

in vec2 vUV;

uniform vec4 uColor;
uniform vec2 uAlphaTest; // x = enabled (1)/disabled (0), y = alpha threshold
uniform bool uColorMapEnabled;
uniform sampler2D uTex;

#define ALPHATEST_ENABLED   uAlphaTest.x
#define ALPHATEST_THRESHOLD uAlphaTest.y

bool AlphaTestPass(float alpha)
{
    return ALPHATEST_ENABLED <= 0.001 || alpha > ALPHATEST_THRESHOLD;
}

bool ColorMapEnabled()
{
    return uColorMapEnabled;
}

void main()
{
    vec4 c = uColor;
    if (ColorMapEnabled())
        c *= texture2D(uTex, vUV);

    if (!AlphaTestPass(c.a))
        discard;

    gl_FragColor = c;
}