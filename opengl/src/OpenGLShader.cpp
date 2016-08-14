/**
 * @class	OpenGL Shader
 * @brief	OpenGL shader implementation
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */

#include "OpenGL.h"
#include <stdio.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include "OpenGLShader.hpp"
#include "OpenGLShaderMaterial.hpp"

OpenGLShader::OpenGLShader(void) : _programID(0) {}
OpenGLShader::~OpenGLShader(void)
{
    _deleteShadersIDs();
    __(glDeleteProgram(_programID));
}

bool OpenGLShader::use(const std::string &path, std::string &error)
{
    std::string vertex = std::string("data/shaders/") + path + std::string(".vert");
    std::string geometry = std::string("data/shaders/") + path + std::string(".geo");
    std::string fragment = std::string("data/shaders/") + path + std::string(".frag");

    if (loadVertexShader(vertex, error) == false) {
        return false;
    }
    if (loadGeometryShader(geometry, error) == false) {
        return false;
    }

    if (loadFragmentShader(fragment, error) == false) {
        return false;
    }
    return linkProgram(error);
}

bool OpenGLShader::loadVertexShader(const std::string &filename, std::string &error)
{
    GLuint shaderObjectID;

    __(shaderObjectID = glCreateShader(GL_VERTEX_SHADER));

    return _loadShader(shaderObjectID, filename, error);
}

bool OpenGLShader::loadGeometryShader(const std::string &filename, std::string &error)
{
    GLuint shaderObjectID;

    __(shaderObjectID = glCreateShader(GL_GEOMETRY_SHADER));

    return _loadShader(shaderObjectID, filename, error, true);
}

bool OpenGLShader::loadFragmentShader(const std::string &filename, std::string &error)
{
    GLuint shaderObjectID;

    __(shaderObjectID = glCreateShader(GL_FRAGMENT_SHADER));

    return _loadShader(shaderObjectID, filename, error);
}

bool OpenGLShader::_loadShader(uint32_t shaderObjectID, const std::string &filename, std::string &error, bool optional)
{
    GLint result = GL_FALSE;

    /* Open file and read it */
    FILE *shader = fopen(filename.c_str(), "rb");

    if (shader == NULL) {
        if (optional) {
            return true;
        }
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
    char *shaderText = new char[size + 1];
    size_t count = fread(shaderText, sizeof(char), size, shader);
    if (count != size) {
        error = std::string("Failed when reading ") + filename;
        delete[] shaderText;
        fclose(shader);
        return false;
    }
    shaderText[size] = 0;
    fclose(shader);

    __(glShaderSource(shaderObjectID, 1, (const GLchar **)&shaderText, NULL));
    __(glCompileShader(shaderObjectID));

    delete[] shaderText;

    /* Get compilation status */
    __(glGetShaderiv(shaderObjectID, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE) {
        int32_t infoLogLength;
        __(glGetShaderiv(shaderObjectID, GL_INFO_LOG_LENGTH, &infoLogLength));

        error.resize(infoLogLength + 1);
        __(glGetShaderInfoLog(shaderObjectID, infoLogLength, NULL, const_cast<GLchar *>(error.c_str())));

        return false;
    }

    _shadersIDs.push_back(shaderObjectID);
    return true;
}

bool OpenGLShader::linkProgram(std::string &error)
{
    GLint result = GL_FALSE;

    __(_programID = glCreateProgram());

    std::vector<uint32_t>::iterator it;
    for (it = _shadersIDs.begin(); it != _shadersIDs.end(); ++it) {
        __(glAttachShader(_programID, *it));
    }

    /* Link the program */
    __(glLinkProgram(_programID));
    __(glGetProgramiv(_programID, GL_LINK_STATUS, &result));

    if (result == GL_FALSE) {
        int32_t infoLogLength;
        __(glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &infoLogLength));

        error.resize(infoLogLength + 1);
        __(glGetProgramInfoLog(_programID, infoLogLength, NULL, const_cast<GLchar *>(error.c_str())));

        return false;
    }

    _buildUniformsMap();

    /* Delete the shaders IDs */
    _deleteShadersIDs();

    return true;
}

bool OpenGLShader::attach(void)
{
    __(glUseProgram(_programID));
    return true;
}

bool OpenGLShader::detach(void)
{
    __(glUseProgram(0));
    return true;
}

const std::map<std::string, uint32_t> &OpenGLShader::getUniforms(void) { return _uniformNames; }
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
    __(*id = glGetAttribLocation(_programID, name.c_str()));
    return *id != -1 ? true : false;
}

bool OpenGLShader::setUniformMat4(const std::string &name, const glm::mat4 value[], uint32_t numItems)
{
    std::map<std::string, uint32_t>::iterator it = _uniformNames.find(name);

    if (it == _uniformNames.end()) {
        return false;
    }

    __(glUniformMatrix4fv(it->second, numItems, GL_FALSE, (GLfloat *)value));
    return true;
}

bool OpenGLShader::setUniformMat3(const std::string &name, const glm::mat3 value[], uint32_t numItems)
{
    std::map<std::string, uint32_t>::iterator it = _uniformNames.find(name);

    if (it == _uniformNames.end()) {
        return false;
    }

    __(glUniformMatrix3fv(it->second, numItems, GL_FALSE, (GLfloat *)value));
    return true;
}

bool OpenGLShader::setUniformTexture2D(const std::string &name, uint32_t unitID)
{
    std::map<std::string, uint32_t>::iterator it = _uniformNames.find(name);

    if (it == _uniformNames.end()) {
        return false;
    }

    __(glUniform1i(it->second, unitID));
    return true;
}

bool OpenGLShader::setUniformTexture2DArray(const std::string &name, uint32_t unitIDs[], uint32_t numItems)
{
    std::map<std::string, uint32_t>::iterator it = _uniformNames.find(name);

    if (it == _uniformNames.end()) {
        return false;
    }

    __(glUniform1iv(it->second, numItems, (GLint *)unitIDs));
    return true;
}

bool OpenGLShader::setUniformFloat(const std::string &name, float value)
{
    std::map<std::string, uint32_t>::iterator it = _uniformNames.find(name);

    if (it == _uniformNames.end()) {
        return false;
    }

    __(glUniform1f(it->second, value));
    return true;
}

bool OpenGLShader::setUniformUint(const std::string &name, uint32_t value)
{
    std::map<std::string, uint32_t>::iterator it = _uniformNames.find(name);

    if (it == _uniformNames.end()) {
        return false;
    }

    __(glUniform1ui(it->second, value));
    return true;
}

bool OpenGLShader::setUniformBool(const std::string &name, bool value)
{
    std::map<std::string, uint32_t>::iterator it = _uniformNames.find(name);

    if (it == _uniformNames.end()) {
        return false;
    }

    __(glUniform1i(it->second, value));
    return true;
}

bool OpenGLShader::setUniformVec4(const std::string &name, glm::vec4 &value)
{
    std::map<std::string, uint32_t>::iterator it = _uniformNames.find(name);

    if (it == _uniformNames.end()) {
        return false;
    }

    __(glUniform4fv(it->second, 1, &value[0]));
    return true;
}

bool OpenGLShader::setUniformVec3(const std::string &name, glm::vec3 &value)
{
    std::map<std::string, uint32_t>::iterator it = _uniformNames.find(name);

    if (it == _uniformNames.end()) {
        return false;
    }

    __(glUniform3fv(it->second, 1, &value[0]));
    return true;
}

bool OpenGLShader::setUniformVec2(const std::string &name, glm::vec2 &value)
{
    std::map<std::string, uint32_t>::iterator it = _uniformNames.find(name);

    if (it == _uniformNames.end()) {
        return false;
    }

    __(glUniform2fv(it->second, 1, &value[0]));
    return true;
}

void OpenGLShader::setCustomParams() { /* Empty on purpose */}
void OpenGLShader::_deleteShadersIDs(void)
{
    std::vector<uint32_t>::iterator it;
    for (it = _shadersIDs.begin(); it != _shadersIDs.end(); ++it) {
        __(glDeleteShader(*it));
    }
    _shadersIDs.clear();
}

void OpenGLShader::_buildUniformsMap(void)
{
    _uniformNames.clear();

    int32_t count, i;
    __(glGetProgramiv(_programID, GL_ACTIVE_UNIFORMS, &count));

    for (i = 0; i < count; ++i) {
        char uniformName[128];
        GLint size;
        GLenum type;

        /* Get uniform name */
        __(glGetActiveUniform(_programID, i, sizeof uniformName, NULL, &size, &type, uniformName));

        /* Get location */
        uint32_t uniformID;

        __(uniformID = glGetUniformLocation(_programID, uniformName));

        /* Save in map */
        _uniformNames[uniformName] = uniformID;
    }
}
