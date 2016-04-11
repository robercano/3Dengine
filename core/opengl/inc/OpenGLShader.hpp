/**
 * @class	OpenGLShader
 * @brief   OpenGL shader implementation
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include <map>
#include <vector>
#include <string>
#include "Shader.hpp"
#include "OpenGLShaderMaterial.hpp"
#include "OpenGLShaderLight.hpp"

class OpenGLShader : public virtual Shader
{
	public:
		OpenGLShader(void);
		~OpenGLShader(void);
		bool init() { return true; }
		bool use(const std::string &path, std::string &error);
		bool loadVertexShader(const std::string &filename, std::string &error);
		bool loadFragmentShader(const std::string &filename, std::string &error);
		bool linkProgram(std::string &error);
		bool attach(void);
		bool detach(void);
		const std::map<std::string, uint32_t> & getUniforms(void);
        const bool getUniformID(const std::string &name, uint32_t *id);
        const bool getAttributeID(const std::string &name, uint32_t *id);
		bool setUniformMat4(const std::string &name, const glm::mat4 &value);
		bool setUniformMat3(const std::string &name, const glm::mat3 &value);
        bool setUniformTexture2D(const std::string &name, uint32_t unitID);
        bool setUniformFloat(const std::string &name, float value);
        bool setUniformUint(const std::string &name, uint32_t value);
        bool setUniformVec4(const std::string &name, glm::vec4 &value);
        bool setUniformVec3(const std::string &name, glm::vec3 &value);
        bool setUniformVec2(const std::string &name, glm::vec2 &value);
	protected:
		bool _loadShader(uint32_t shaderObjectID, const std::string &filename, std::string &error);
		void _deleteShadersIDs(void);
		void _buildUniformsMap(void);
		std::vector<uint32_t> _shadersIDs;
		std::map<std::string, uint32_t> _uniformNames;
		uint32_t _programID;
};
