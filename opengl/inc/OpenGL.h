/**
 * Global OpenGL header that includes the necessary OpenGL
 * headers in the right order for the specific platform.
 *
 * It also provides some macros for OpenGL debugging
 *
 * @author Roberto Cano
 */
#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

#if defined(__linux__)
#include <GL/gl.h>
#include <GL/glew.h>
#include <GL/glu.h>
#elif defined(__APPLE__)
#include <GL/glew.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#elif defined(_WIN32) || defined(_WIN64)
#include <GL/glew.h>
#include <gl/GL.h>
#else
#error "Platform not supported for OpenGL"
#endif

/* Macro for OpenGL debugging. It performs the GL call
 * and then checks the resulting error with glGetError().
 * This approach is over-killing but it helps narrow down
 * problems in OpenGL pipeline
 */
#ifdef DEBUG_OPENGL_PIPELINE

#define GL_ERROR_TO_STR(error) \
		(error == 0x500)  ? "GL_INVALID_ENUM" :      \
        (error == 0x501)  ? "GL_INVALID_VALUE" :     \
		(error == 0x502)  ? "GL_INVALID_OPERATION" : \
		(error == 0x503)  ? "GL_STACK_OVERFLOW" :    \
		(error == 0x504)  ? "GL_STACK_UNDERFLOW" :   \
		(error == 0x505)  ? "GL_OUT_OF_MEMORY" :     \
		(error == 0x506)  ? "GL_INVALID_FRAMEBUFFER_OPERATION" : \
		(error == 0x507)  ? "GL_CONTEXT_LOST" :      \
		(error == 0x8031) ? "GL_TABLE_TOO_LARGE1" :  \
        "Unknown"

#define __(call)                                                                                                                         \
    {                                                                                                                                    \
        glGetError();                                                                                                                    \
        call;                                                                                                                            \
        GLuint error = glGetError();                                                                                                     \
        if (error != GL_NO_ERROR) {                                                                                                      \
            fprintf(stderr, "ERROR 0x%x (%s) calling:\n\t%s\nin context:\n\t%s (%s:%d)\n\n", error, GL_ERROR_TO_STR(error), #call, __PRETTY_FUNCTION__, __FILE__, __LINE__); \
        }                                                                                                                                \
    }
#else
#define __(call) call
#endif

#define GL_CHECKPOINT()                                                                                                               \
    {                                                                                                                                 \
        GLuint error = glGetError();                                                                                                  \
        if (error != GL_NO_ERROR) {                                                                                                   \
            fprintf(stderr, "ERROR 0x%x (%s) in checkpoint for context:\n\t%s (%s:%d)\n\n", error, GL_ERROR_TO_STR(error), __PRETTY_FUNCTION__, __FILE__, __LINE__); \
        }                                                                                                                             \
    }
