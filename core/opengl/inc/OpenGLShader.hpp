/**
 * @class	OpenGLShader
 * @brief   OpenGL shader implementation
 *
 * @author	Roberto Sosa Cano
 */
#pragma once

#include <map>
#include <vector>
#include <string>
#include "Shader.hpp"

class OpenGLShader : public Shader
{
	public:
		/**
		 * Constructor
		 */
		OpenGLShader(void);

		/**
		 * Destructor
		 */
		~OpenGLShader(void);

		/**
		 * Loads a new vertex shader code and compiles it
		 *
		 * @param		filename  File name of the shader to be compiled
		 * @param[out]	error	  If compilation fails error will contain
		 *						  a description of the error
		 *
		 * @return true or false
		 */
		bool loadVertexShader(const std::string &filename, std::string &error);

		/**
		 * Loads a new fragment shader code and compiles it
		 *
		 * @param		filename  File name of the shader to be compiled
		 * @param[out]	error	  If compilation fails error will contain
		 *						  a description of the error
		 *
		 * @return true or false
		 */
		bool loadFragmentShader(const std::string &filename, std::string &error);

		/**
		 * Links the program together and prepares the shader to be used
		 *
		 * @param[out] error	If linking fails error will contain a
		 *						description of the error
		 *
		 * @return true or false
		 */
		bool linkProgram(std::string &error);

		/**
		 * Makes the shader active
		 */
		bool attach(void);

		/**
		 * Makes the shader inactive
		 */
		bool detach(void);

		/**
		 * Gets a list of all the shader uniforms
		 *
		 * @return A vector with all the available uniform names
		 */
		const std::map<std::string, uint32_t> & getUniforms(void);

        /**
         * Retrieves the ID of a specific uniform
         *
         * @param name  Name of the uniform
         * @param id    Output ID
         *
         */
        const bool getUniformID(const std::string &name, uint32_t *id);
        const bool getAttributeID(const std::string &name, uint32_t *id);

		/**
		 * Sets the value of a shader uniform as a mat4x4
		 *
		 * @param name  Name of the shader uniform
		 * @param value Value of the uniform to be set
		 *
		 * @return true if the value was set or false if the
		 *         attribute cannot be found
		 */
		bool setUniformMat4(const std::string &name, const glm::mat4 &value);
        bool setUniformTexture2D(const std::string &name, uint32_t unitID);
        bool setUniformFloat(const std::string &name, float value);
        bool setUniformVec4(const std::string &name, glm::vec4 &value);

	private:
		/**
		 * Internal function to load a shader code and compile it
		 *
		 * @param		shaderObjectID ID of the shader to be compiled
		 * @param		filename       File name of the shader to be compiled
		 * @param[out]	error	       If compilation fails error will contain
		 *						       a description of the error
		 *
		 * @return true or false
		 */
		bool _loadShader(uint32_t shaderObjectID, const std::string &filename, std::string &error);

		/**
		 * Deletes all the previously allocated shaders IDs
		 */
		void _deleteShadersIDs(void);

		/**
		 * Internal function to build up the association map for
		 * uniforms names/IDs
		 */
		void _buildUniformsMap(void);

		/**
		 * List of compiled shaders
		 */
		std::vector<uint32_t> _shadersIDs;

		/**
		 * Map with uniform names/location pairs
		 */
		std::map<std::string, uint32_t> _uniformNames;

		/**
		 * ID of the compiled program
		 */
		uint32_t _programID;
};
