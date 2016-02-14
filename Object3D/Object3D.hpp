/**
 * @class	Object3D
 * @brief	Basic object 3D representation
 *
 * @author	Roberto Sosa Cano
 */
#pragma once

#include <glm/glm.hpp>
#include "Shader.hpp"

class Object3D
{
	public:
		/**
		 * Destructor
		 */
		virtual ~Object3D() {}

		/**
		 * Initialises the 3D object
		 *
		 * @returns	true if the object was initialised or false otherwise
		 */
		virtual bool init() = 0;

		/**
		 * Destroys the object by deinitilising it
		 *
		 * @returns	true if the object was destroyed or false otherwise
		 */
		virtual bool destroy() = 0;

        /**
         * Retrieves the vertex array index to be used for the rendering
         *
         * @returns the vertex array index
         */
        virtual uint32_t getVertexArrayIndex() = 0;

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
		 *
		 * @returns	true if the object was rendered or false otherwise
		 */
		virtual bool render(const glm::mat4 &projection, const glm::mat4 &view);

    private:
        /**
         * Shader to use for the rendering of this object
         */
        Shader *_shader;
};
