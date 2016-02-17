/**
 * @class	OpenGL Shader
 * @brief	OpenGL shader implementation
 *
 * @author	Roberto Sosa Cano
 */

#include <iostream>
#include <fstream>
#include <stdio.h>
#ifdef __linux
#include <GL/glew.h>
#else
#include <OpenGL/gl.h>
#endif
#include "OpenGLShader.hpp"

OpenGLShader::OpenGLShader(void) : _programID(0)
{
}

OpenGLShader::~OpenGLShader(void)
{
	_deleteShadersIDs();
	glDeleteProgram(_programID);
}

bool OpenGLShader::loadVertexShader(const std::string &filename, std::string &error)
{
	GLuint shaderObjectID   = glCreateShader(GL_VERTEX_SHADER);

	return _loadShader(shaderObjectID, filename, error);
}

bool OpenGLShader::loadFragmentShader(const std::string &filename, std::string &error)
{
	GLuint shaderObjectID   = glCreateShader(GL_FRAGMENT_SHADER);

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

	glShaderSource(shaderObjectID, 1, (const GLchar **)&shaderText, NULL);
	glCompileShader(shaderObjectID);

	delete shaderText;

	/* Get compilation status */
	glGetShaderiv(shaderObjectID, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int32_t infoLogLength;
		glGetShaderiv(shaderObjectID, GL_INFO_LOG_LENGTH, &infoLogLength);

		char errorMessage[infoLogLength + 1];
		glGetShaderInfoLog(shaderObjectID, infoLogLength, NULL, errorMessage);

		error = errorMessage;
		return false;
	}

	_shadersIDs.push_back(shaderObjectID);
	return true;
}

bool OpenGLShader::linkProgram(std::string &error)
{
	GLint result = GL_FALSE;

	_programID = glCreateProgram();

	std::vector<uint32_t>::iterator it;
	for (it=_shadersIDs.begin(); it != _shadersIDs.end(); ++it) {
		glAttachShader(_programID, *it);
	}

	/* Link the program */
	glLinkProgram(_programID);
	glGetProgramiv(_programID, GL_LINK_STATUS, &result);

	if (result == GL_FALSE) {
		int32_t infoLogLength;
		glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &infoLogLength);

		char errorMessage[infoLogLength + 1];
		glGetProgramInfoLog(_programID, infoLogLength, NULL, errorMessage);

		error = errorMessage;
		return false;
	}

	_buildUniformsMap();

	/* Delete the shaders IDs */
	_deleteShadersIDs();

	return true;
}

bool OpenGLShader::attach(void)
{
	glUseProgram(_programID);
    return true;
}

bool OpenGLShader::detach(void)
{
	glUseProgram(0);
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

bool OpenGLShader::setUniform(const std::string &name, glm::mat4 &value)
{
	std::map<std::string, uint32_t>::iterator it = _uniformNames.find(name);

	if (it == _uniformNames.end()) {
		return false;
	}

	glUniformMatrix4fv(it->second, 1, GL_FALSE, &value[0][0]);
	return true;
}

bool OpenGLShader::setUniformTexture2D(const std::string &name, GLuint unitID)
{
	std::map<std::string, uint32_t>::iterator it = _uniformNames.find(name);

	if (it == _uniformNames.end()) {
		return false;
	}

    glUniform1i(it->second, GL_TEXTURE0 + unitID);
	return true;
}

void OpenGLShader::_deleteShadersIDs(void)
{
	std::vector<uint32_t>::iterator it;
	for (it=_shadersIDs.begin(); it != _shadersIDs.end(); ++it) {
		glDeleteShader(*it);
	}
	_shadersIDs.clear();
}

void OpenGLShader::_buildUniformsMap(void)
{
	_uniformNames.clear();

	int32_t count, i;
	glGetProgramiv(_programID, GL_ACTIVE_UNIFORMS, &count);

	for (i=0; i<count; ++i) {
		char uniformName[128];

		/* Get uniform name */
		glGetActiveUniform(_programID, i, sizeof uniformName, NULL, NULL, NULL, uniformName);

		/* Get location */
		uint32_t uniformID = glGetUniformLocation(_programID, uniformName);

		/* Save in map */
		_uniformNames[uniformName] = uniformID;
	}
}
