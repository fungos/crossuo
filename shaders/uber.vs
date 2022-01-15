#version 330
// #ifdef GL_FRAGMENT_PRECISION_HIGH
// precision highp float;
// #else
// precision mediump float;
// #endif

uniform mat4 uProjectionView;

uniform mat4 uModel;
uniform vec4 uColor;
uniform vec2 uUV;
uniform bool uMirror;

out vec2 vUV;

const int VertexCount = 6;
// clang-format off
const vec2 Position[VertexCount] = vec2[](
    vec2(0.0, 1.0),
    vec2(1.0, 0.0),
    vec2(0.0, 0.0),
    vec2(0.0, 1.0),
    vec2(1.0, 1.0),
    vec2(1.0, 0.0));

// FIXME
const vec2 Position_Mirrored[VertexCount] = vec2[](
    vec2(1.0, 0.0),
    vec2(1.0, 1.0),
    vec2(0.0, 1.0),

    vec2(0.0, 0.0),
    vec2(1.0, 0.0),
    vec2(0.0, 1.0));

const vec2 Texcoords[VertexCount] = vec2[](
    vec2(0.0, 1.0),
    vec2(1.0, 0.0),
    vec2(0.0, 0.0),
    vec2(0.0, 1.0),
    vec2(1.0, 1.0),
    vec2(1.0, 0.0));
// clang-format on

void main()
{
    vUV = uUV * Texcoords[gl_VertexID];
    if (uMirror)
        gl_Position = uProjectionView * uModel * vec4(Position_Mirrored[gl_VertexID], 0.0, 1.0);
    else
        gl_Position = uProjectionView * uModel * vec4(Position[gl_VertexID], 0.0, 1.0);
}