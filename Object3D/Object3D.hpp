/**
 * @class	Object3D
 * @brief	Basic object 3D representation
 *
 * @author	Roberto Sosa Cano
 */
#pragma once

#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>
#include "Shader.hpp"
#include "RenderTarget.hpp"

class Object3D
{
	public:
		/**
		 * Destructor
		 */
		virtual ~Object3D() {}

        /**
         * Returns the vertices array information
         */
        virtual const GLfloat *getVerticesArray() = 0;
        virtual uint32_t getVerticesArrayLen() = 0;

        /**
         * Returns the colors array information
         */
        virtual const GLfloat *getColorsArray() = 0;
        virtual uint32_t getColorsArrayLen() = 0;

        /**
         * Returns the normals array information
         */
        virtual const GLfloat *getNormalsArray() = 0;
        virtual uint32_t getNormalsArrayLen() = 0;

        /**
         * Returns the colors array information
         */
        virtual const GLuint *getIndicesArray() = 0;
        virtual uint32_t getIndicesArrayLen() = 0;

		/**
		 * Initialises the 3D object
		 *
		 * @returns	true if the object was initialised or false otherwise
		 */
		virtual bool init();

		/**
		 * Destroys the object by deinitilising it
		 *
		 * @returns	true if the object was destroyed or false otherwise
		 */
		virtual bool destroy();

        /**
         * Adds a new shader to the object to be used for rendering
         *
         * @shader   The shader to be added
         *
         * @returns true if the shader was added correctly, false if not
         */
        virtual bool addShader(Shader *shader);

		/**
		 * Renders the object by using graphic API commands
		 *
		 * @param projection	Projection matrix
		 * @param view			View matrix
         * @param renderTarget  Render target
		 *
		 * @returns	true if the object was rendered or false otherwise
		 */
		virtual bool render(const glm::mat4 &projection, const glm::mat4 &view, RenderTarget &renderTarget);

    private:
        /**
         * Shader to use for the rendering of this object
         */
        Shader *_shader;

        /**
         * Vertex array object ID
         */
        uint32_t _gVAO;

        /**
         * Vertex buffer object for vertices
         */
        uint32_t _verticesVBO;

        /**
         * Vertex buffer object for colors
         */
        uint32_t _colorsVBO;

        /**
         * Vertex buffer object for normals
         */
        uint32_t _normalsVBO;

        /**
         * Vertex buffer object for indices
         */
        uint32_t _indicesVBO;
};
