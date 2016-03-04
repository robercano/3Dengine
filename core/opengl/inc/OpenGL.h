/**
 * Global OpenGL header that includes the necessary OpenGL
 * headers in the right order for the specific platform.
 *
 * It also provides some macros for OpenGL debugging
 *
 * @author Roberto Cano
 */
#pragma once

#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#if defined (__linux__)
#include <GL/gl.h>
#include <GL/glu.h>
#elif defined (__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
    #error "Platform not supported for OpenGL"
#endif

/* Macro for OpenGL debugging. It performs the GL call
 * and then checks the resulting error with glGetError().
 * This approach is over-killing but it helps narrow down
 * problems in OpenGL pipeline
 */
#ifdef DEBUG_OPENGL_PIPELINE
#define GL( call ) \
    { \
        glGetError(); \
        call; \
        GLuint error = glGetError(); \
        if (error != GL_NO_ERROR) { \
            fprintf(stderr, "ERROR 0x%x calling %s in context:\n\t%s (%s:%d)\n", \
                    error, #call, __PRETTY_FUNCTION__, __FILE__, __LINE__); \
        } \
    }
#else
#define GL( call ) call
#endif

#define GL_CHECKPOINT() \
    { \
        GLuint error = glGetError(); \
        if (error != GL_NO_ERROR) { \
            fprintf(stderr, "ERROR 0x%x in checkpoint for context:\n\t%s (%s:%d)\n", \
                    error, __PRETTY_FUNCTION__, __FILE__, __LINE__); \
        } \
    }
