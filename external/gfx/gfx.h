// AGPLv3 License
// Copyright (c) 2019 Danny Angelo Carminati Grein

#ifndef EXTERNAL_GFX_H
#define EXTERNAL_GFX_H

#if defined(USE_GL3)
    #define USE_GL
    #define USE_GL3W
    #define SOKOL_GLCORE33
    #define SOKOL_GLES3
    #define GFX_GL_MAJOR 3
    #if defined(__APPLE__)
        #define GL_SHADER_VERSION "150"
        #define GFX_GL_MINOR 2
    #else // #if defined(__APPLE__)
        #define GL_SHADER_VERSION "330"
        #define GFX_GL_MINOR 0
    #endif // #else // #if defined(__APPLE__)
#elif defined(USE_GL2) // #if defined(USE_GL3)
    #define USE_GL
    #define USE_GLEW
    #define SOKOL_GLES2
    #define GL_SHADER_VERSION "220"
    #define GFX_GL_MAJOR 2
    #define GFX_GL_MINOR 2
#elif defined(USE_GLES2) // #if defined(USE_GL2)
    #define USE_GLES
    #define SOKOL_GLES2
    #define GL_SHADER_VERSION "100"
    #define GFX_GL_MAJOR 2
    #define GFX_GL_MINOR 1
    #define IMGUI_IMPL_OPENGL_ES2
    //#define IMGUI_IMPL_OPENGL_ES3
#elif defined(USE_DX11) // #elif defined(USE_GL2)
    #define SOKOL_D3D11
#elif defined(USE_METAL) // #elif defined(USE_DX11)
    // SDL+Metal: https://gist.github.com/slime73/12284a8299be857d2581
    #define SOKOL_METAL
#else // #elif defined(USE_METAL)
    #define SOKOL_DUMMY_BACKEND
#endif // USE_GL3

#define SOKOL_TRACE_HOOKS // sokol_gfx_imgui debugging helper

#if defined(USE_GLEW)
    #define IMGUI_IMPL_OPENGL_LOADER_GLEW
    #define NO_SDL_GLEXT
    #include "GL/glew.h"
    #if defined(_MSC_VER)
        #include "GL/wglew.h"
    #endif // #else // #if defined(_MSC_VER)
#elif defined(USE_GL3W) // #if defined(USE_GLEW)
    #define IMGUI_IMPL_OPENGL_LOADER_GL3W
    #include "GL/gl3w.h"
#endif // #elif defined(USE_GL3W) // #if defined(USE_GLEW)

#if defined(USE_GL)
    /*#if defined(__APPLE__)
        #define GL_SILENCE_DEPRECATION
        #include <OpenGL/gl.h>
    #elif defined(__linux__) // #if defined(__APPLE__)
        #include <GL/gl.h>
    #endif // #elif defined(__linux__) // #if defined(__APPLE__)*/
    #include <SDL_opengl.h>
#elif defined(USE_GLES) // #if defined(USE_GL)
    #if (USE_GLES2)
        //#define GL_GLEXT_PROTOTYPES
        //#include <GLES2/gl2.h>
        //#include <GLES2/gl2ext.h>
        #include <SDL_opengles2.h>
        #define __glew_h__ // HACK to get sokol_gfx use the right symbols
    #endif
#endif // #elif defined(USE_GLES) // #if defined(USE_GL)

#if defined(USE_GL) || defined(USE_GLES)
#define GL_CHECK_ATTRIB(x) \
    do { \
        if (x == -1) { \
            Error(Renderer, "Attib %s not found or removed from shader", TOSTRING(x)); \
            exit(-68); \
        } \
    } while(0)
#define GL_CHECK(statement) \
    do { \
        statement; \
        const auto e = glGetError(); \
        if (e != GL_NO_ERROR) { \
            Error(Renderer, TOSTRING(statement) " returned error: 0x%04x", e); \
            exit(-69); \
        } \
    } while(0)
#endif

#if defined(XUO_LOCAL_HEADERS)
    #include "SDL.h"
#else // #if defined(XUO_LOCAL_HEADERS)
    #include <SDL.h>
#endif // #else // #if defined(XUO_LOCAL_HEADERS)

#ifdef __cplusplus
extern "C" {
#endif

typedef struct win_context_
{
    const char *title;
    SDL_Window *window;
    void *sg_default_desc;
    void *sg_default_shader_desc;
    void *context;
    const char *inifile;
    int width;
    int height;
    int vsync;
    int debug;
} win_context;

int win_init(win_context *ctx);
void win_shutdown(win_context *ctx);
void win_flip(win_context *ctx);
void win_gfx_context_attrbutes(int enableDebug);

#ifdef __cplusplus
}
#endif

#endif // #ifndef EXTERNAL_GFX_H
