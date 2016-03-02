/**
 * @class	OpenGLMaterial
 * @brief	OpenGL material implemented as a block uniform to be used in
 *          a shader. Contains ambient, diffuse, specular, alpha and shininess
 *          components
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include "Material.hpp"
#include "OpenGL.h"

class OpenGLMaterial : public Material
{
	public:
        OpenGLMaterial() : _paramsBuffer(NULL) {}
        ~OpenGLMaterial();

        bool bindToShader(GLuint programID);
        bool setValues(glm::vec3 &ambient, glm::vec3 &diffuse, glm::vec3 &specular, float alpha, float shininess);

    private:
        GLuint _uniformBO;
        GLubyte *_paramsBuffer;
        GLint _paramsOffset[5];
        GLuint _blockIndex;
        GLint _blockSize;
};
