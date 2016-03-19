/**
 * @class	OpenGL Shader
 * @brief	OpenGL shader implementation
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */

#include <iostream>
#include <fstream>
#include <stdio.h>
#include "OpenGL.h"
#include "OpenGLShader.hpp"
#include "OpenGLShaderMaterial.hpp"

OpenGLShader::OpenGLShader(void) : _programID(0), _material(NULL)
{
}

OpenGLShader::~OpenGLShader(void)
{
	_deleteShadersIDs();
	GL( glDeleteProgram(_programID) );
}

bool OpenGLShader::loadVertexShader(const std::string &filename, std::string &error)
{
	GLuint shaderObjectID;

    GL( shaderObjectID = glCreateShader(GL_VERTEX_SHADER) );

	return _loadShader(shaderObjectID, filename, error);
}

bool OpenGLShader::loadFragmentShader(const std::string &filename, std::string &error)
{
	GLuint shaderObjectID;

    GL( shaderObjectID = glCreateShader(GL_FRAGMENT_SHADER) );

	return _loadShader(shaderObjectID, filename, error);
}

bool OpenGLShader::_loadShader(uint32_t shaderObjectID, const std::string &filename, std::string &error)
{
	GLint result = GL_FALSE;

	/* Open file and read it */
	FILE *shader = fopen(filename.c_str(), "r");

	if (shader == NULL) {
		error = std::string("File ") + filename + std::string(" couldn't be opened");
		return false;
	}

	fseek(shader, 0, SEEK_END);
	long size = ftell(shader);
	fseek(shader, 0, SEEK_SET);

	if (size == -1) {
		error = std::string("Failed when getting the size of ") + filename;
		fclose(shader);
		return false;
	}
	char *shaderText = new char[size+1];
	if (fread(shaderText, sizeof(char), size, shader) != size) {
		error = std::string("Failed when reading ") + filename;
		delete shaderText;
		fclose(shader);
		return false;
	}
	shaderText[size] = 0;
	fclose(shader);

	GL( glShaderSource(shaderObjectID, 1, (const GLchar **)&shaderText, NULL) );
	GL( glCompileShader(shaderObjectID) );

	delete shaderText;

	/* Get compilation status */
	GL( glGetShaderiv(shaderObjectID, GL_COMPILE_STATUS, &result) );
	if (result == GL_FALSE) {
		int32_t infoLogLength;
		GL( glGetShaderiv(shaderObjectID, GL_INFO_LOG_LENGTH, &infoLogLength) );

		char errorMessage[infoLogLength + 1];
		GL( glGetShaderInfoLog(shaderObjectID, infoLogLength, NULL, errorMessage) );

		error = errorMessage;
		return false;
	}

	_shadersIDs.push_back(shaderObjectID);
	return true;
}

bool OpenGLShader::linkProgram(std::string &error)
{
	GLint result = GL_FALSE;

	GL( _programID = glCreateProgram() );

	std::vector<uint32_t>::iterator it;
	for (it=_shadersIDs.begin(); it != _shadersIDs.end(); ++it) {
		GL( glAttachShader(_programID, *it) );
	}

	/* Link the program */
	GL( glLinkProgram(_programID) );
	GL( glGetProgramiv(_programID, GL_LINK_STATUS, &result) );

	if (result == GL_FALSE) {
		int32_t infoLogLength;
		GL( glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &infoLogLength) );

		char errorMessage[infoLogLength + 1];
		GL( glGetProgramInfoLog(_programID, infoLogLength, NULL, errorMessage) );

		error = errorMessage;
		return false;
	}

	_buildUniformsMap();

	/* Delete the shaders IDs */
	_deleteShadersIDs();

	return true;
}

ShaderMaterial *OpenGLShader::getMaterial()
{
    if (_material != NULL) {
        return _material;
    }

    _material = new OpenGLShaderMaterial();

    if (_material->prepareForShader(_programID) == false) {
        delete _material;
        _material = NULL;
    }

    return _material;
}

ShaderLight *OpenGLShader::getLight()
{
    if (_light != NULL) {
        return _light;
    }

    _light = new OpenGLShaderLight();

    if (_light->prepareForShader(_programID) == false) {
        delete _light;
        _light = NULL;
    }

    return _light;
}

bool OpenGLShader::attach(void)
{
	GL( glUseProgram(_programID) );
    return true;
}

bool OpenGLShader::detach(void)
{
	GL( glUseProgram(0) );
    return true;
}

const std::map<std::string, uint32_t> & OpenGLShader::getUniforms(void)
{
	return _uniformNames;
}

const bool OpenGLShader::getUniformID(const std::string &name, uint32_t *id)
{
	std::map<std::string, uint32_t>::iterator it = _uniformNames.find(name);

	if (it == _uniformNames.end()) {
		return false;
	}

    *id = it->second;
    return true;
}

const bool OpenGLShader::getAttributeID(const std::string &name, uint32_t *id)
{
    GL( *id = glGetAttribLocation( _programID, name.c_str()) );
    return *id != -1 ? true : false;
}

bool OpenGLShader::setUniformMat4(const std::string &name, const glm::mat4 &value)
{
	std::map<std::string, uint32_t>::iterator it = _uniformNames.find(name);

	if (it == _uniformNames.end()) {
		return false;
	}

	GL( glUniformMatrix4fv(it->second, 1, GL_FALSE, &value[0][0]) );
	return true;
}

bool OpenGLShader::setUniformTexture2D(const std::string &name, uint32_t unitID)
{
	std::map<std::string, uint32_t>::iterator it = _uniformNames.find(name);

	if (it == _uniformNames.end()) {
		return false;
	}

    GL( glUniform1i(it->second, unitID) );
	return true;
}

bool OpenGLShader::setUniformFloat(const std::string &name, float value)
{
	std::map<std::string, uint32_t>::iterator it = _uniformNames.find(name);

	if (it == _uniformNames.end()) {
		return false;
	}

    GL( glUniform1f(it->second, value) );
    return true;
}

bool OpenGLShader::setUniformVec2(const std::string &name, glm::vec2 &value)
{
	std::map<std::string, uint32_t>::iterator it = _uniformNames.find(name);

	if (it == _uniformNames.end()) {
		return false;
	}

    GL( glUniform2fv(it->second, 1, &value[0]) );
    return true;
}

bool OpenGLShader::setUniformVec4(const std::string &name, glm::vec4 &value)
{
	std::map<std::string, uint32_t>::iterator it = _uniformNames.find(name);

	if (it == _uniformNames.end()) {
		return false;
	}

    GL( glUniform4fv(it->second, 1, &value[0]) );
    return true;
}

void OpenGLShader::_deleteShadersIDs(void)
{
	std::vector<uint32_t>::iterator it;
	for (it=_shadersIDs.begin(); it != _shadersIDs.end(); ++it) {
		GL( glDeleteShader(*it) );
	}
	_shadersIDs.clear();
}

void OpenGLShader::_buildUniformsMap(void)
{
	_uniformNames.clear();

	int32_t count, i;
	GL( glGetProgramiv(_programID, GL_ACTIVE_UNIFORMS, &count) );

	for (i=0; i<count; ++i) {
		char uniformName[128];
        GLsizei length;
        GLint size;
        GLenum type;

		/* Get uniform name */
		GL( glGetActiveUniform(_programID, i, sizeof uniformName, NULL, &size, &type, uniformName) );

		/* Get location */
		uint32_t uniformID;

        GL( uniformID = glGetUniformLocation(_programID, uniformName) );

		/* Save in map */
		_uniformNames[uniformName] = uniformID; }
}
