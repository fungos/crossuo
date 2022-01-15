// MIT License
// Copyright (C) August 2016 Hotride

#pragma once

#include <external/gfx/gfx.h>

// clang-format off

extern const char* g_uber_vs_src;
extern const char* g_uber_fs_src;


#if defined(USE_GLES) || defined(USE_GL3)

/*
drawMode:

enum SHADER_DRAW_MODE
{
    SDM_NO_COLOR = 0,
    SDM_COLORED = 1,
    SDM_PARTIAL_HUE = 2,
    SDM_TEXT_COLORED_NO_BLACK = 3,
    SDM_TEXT_COLORED = 4,
    SDM_LAND = 6,
    SDM_LAND_COLORED = 7,
    SDM_SPECTRAL = 10,
    SDM_SPECIAL_SPECTRAL = 11,
    SDM_SHADOW = 12
};
*/

#endif


#if defined(USE_GL2)

static const char *g_Vert_ShaderData =
    GL_SHADER_HEADER
    GL_SHADER_OUT "vec3 l;\n"
    GL_SHADER_OUT "vec3 n;\n"
    "uniform int drawMode;\n"
    "void main(void)\n"
    "{\n"
    "	if (drawMode > 5)\n"
    "	{\n"
    "		l = normalize(vec3(gl_LightSource[0].position));\n"
    "		n = normalize(gl_NormalMatrix * gl_Normal);\n"
    "	}\n"
    "	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
    "	gl_TexCoord[0] = gl_MultiTexCoord0;\n"
    "	gl_FrontColor = gl_Color;\n"
    "}";

static const char *g_Frag_DeathShaderData =
    GL_SHADER_HEADER
    GL_SHADER_IN "vec3 l;\n"
    GL_SHADER_IN "vec3 n;\n"
    "uniform sampler2D usedTexture;\n"
    "uniform int drawMode;\n"
    "void main(void)\n"
    "{\n"
    "	vec4 textureColor = texture2D(usedTexture, gl_TexCoord[0].xy);\n"
    "	if (textureColor.a != 0.0)\n"
    "	{\n"
    "		if (drawMode > 5)\n"
    "		{\n"
    "			vec3 n2 = normalize(n);\n"
    "			vec3 l2 = normalize(l);\n"
    "			vec4 normal = vec4(max(dot(n2, l2) + 0.5, 0.0));\n"
    "			textureColor = textureColor * normal;\n"
    "		}\n"
    "		float red = (textureColor.r * 0.6 + textureColor.g * 0.05);\n"
    "		textureColor = vec4(red, red, red, textureColor.a);\n"
    "	}\n"
    "	gl_FragColor = textureColor;\n"
    "}";

static const char *g_Frag_LightShaderData =
    GL_SHADER_HEADER
    "uniform sampler2D usedTexture;\n"
    "uniform int drawMode;\n"
    "uniform float colors[96];\n"
    "void main(void)\n"
    "{\n"
    "	vec4 textureColor = texture2D(usedTexture, gl_TexCoord[0].xy);\n"
    "	if (textureColor.a != 0.0 && drawMode == 1)\n"
    "	{\n"
    "		int index = int(textureColor.r * 7.96875) * 3;\n"
    "		gl_FragColor = (textureColor * vec4(colors[index], colors[index + 1], colors[index + 2], 1.0)) * 3.0;\n"
    "	}\n"
    "	else\n"
    "		gl_FragColor = textureColor;\n"
    "}";

static const char *g_Frag_FontShaderData =
    GL_SHADER_HEADER
    "uniform sampler2D usedTexture;\n"
    "uniform int drawMode;\n"
    "uniform float colors[96];\n"
    "void main(void)\n"
    "{\n"
    "	vec4 textureColor = texture2D(usedTexture, gl_TexCoord[0].xy);\n"
    "	if (textureColor.a != 0.0)\n"
    "	{\n"
    "		if (drawMode == 1 || (drawMode == 2 && textureColor.r == textureColor.g && textureColor.r == textureColor.b))\n"
    "		{\n"
    "			int index = int(textureColor.r * 31.875) * 3;\n"
    "			gl_FragColor = vec4(colors[index], colors[index + 1], colors[index + 2], textureColor.a) * gl_Color;\n"
    "		}\n"
    "		else if (drawMode == 4 || (drawMode == 3 && textureColor.r > 0.04))\n"
    "		{\n"
    "			gl_FragColor = vec4(colors[90], colors[91], colors[92], textureColor.a) * gl_Color;\n"
    "		}\n"
    "		else\n"
    "			gl_FragColor = textureColor * gl_Color;\n"
    "	}\n"
    "	else\n"
    "		gl_FragColor = textureColor * gl_Color;\n"
    "}";

static const char *g_Frag_ColorizerShaderData =
    GL_SHADER_HEADER
    GL_SHADER_IN "vec3 l;\n"
    GL_SHADER_IN "vec3 n;\n"
    "uniform sampler2D usedTexture;\n"
    "uniform int drawMode;\n"
    "uniform float colors[96];\n"
    "void main(void)\n"
    "{\n"
    "	vec4 textureColor = texture2D(usedTexture, gl_TexCoord[0].xy);\n"
    "	if (textureColor.a != 0.0)\n"
    "	{\n"
    "		if (drawMode == 1 || (drawMode == 2 && textureColor.r == textureColor.g && textureColor.r == textureColor.b))\n"
    "		{\n"
    "			int index = int(textureColor.r * 31.875) * 3;\n"
    "			gl_FragColor = vec4(colors[index], colors[index + 1], colors[index + 2], textureColor.a) * gl_Color;\n"
    "		}\n"
    "		else if (drawMode > 5)\n"
    "		{\n"
    "			if (drawMode > 9)\n"
    "			{\n"
    "				float red = textureColor.r;\n"
    "				if (drawMode > 10)\n"
    "				{\n"
    "					if (drawMode > 11)\n"
    "						red = 0.6;\n"
    "					else\n"
    "						red *= 0.5;\n"
    "				}\n"
    "				else\n"
    "					red *= 1.5;\n"
    "				gl_FragColor = vec4(red, red, red, textureColor.a) * gl_Color;\n"
    "			}\n"
    "			else\n"
    "			{\n"
    "				vec3 n2 = normalize(n);\n"
    "				vec3 l2 = normalize(l);\n"
    "				vec4 normal = vec4(max(dot(n2, l2) + 0.5, 0.0));\n"
    "				if (drawMode > 6)\n"
    "				{\n"
    "					int index = int(textureColor.r * 31.875) * 3;\n"
    "					gl_FragColor = (vec4(colors[index], colors[index + 1], colors[index + 2], textureColor.a) * gl_Color) * normal;\n"
    "				}\n"
    "				else\n"
    "					gl_FragColor = (textureColor * gl_Color) * normal;\n"
    "			}\n"
    "		}\n"
    "		else\n"
    "			gl_FragColor = textureColor * gl_Color;\n"
    "	}\n"
    "	else\n"
    "		gl_FragColor = textureColor * gl_Color;\n"
    "}";
#endif

// clang-format on
