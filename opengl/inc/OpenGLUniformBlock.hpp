/**
 * @class	OpenGLUniformBlock
 * @brief	Manages the access to a uniform block in a shader. It takes care
 *          of finding out the block size and offsets, then allows to set the
 *          values in the block and bind it
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include <string.h>
#include <map>
#include <string>
#include <vector>
#include "OpenGL.h"

class OpenGLUniformBlock
{
  public:
    OpenGLUniformBlock();
    ~OpenGLUniformBlock();

    void setBlockName(const std::string &name) { _blockName = name; }
    void setBlockArrayIndex(uint32_t blockArrayIndex) { _blockArrayIndex = blockArrayIndex; }
    void setBindingPoint(uint32_t bindingPoint) { _bindingPoint = bindingPoint; }
    void addParamName(const std::string &name);

    bool prepareForShader(GLuint programID);

    template <typename T>
    bool setParamValue(const std::string &name, const T &value)
    {
        // TODO: Check if size is correct
        memcpy(_paramsBuffer + _paramsOffsets[name], &value, sizeof value);
        return true;
    }

    void bind();

  private:
    bool _linkedToShader;

    std::string _blockName;
    int32_t _blockArrayIndex;
    GLuint _bindingPoint;
    std::map<std::string, GLint> _paramsOffsets;
    std::vector<std::string> _paramsFullName;

    GLuint _programID;
    GLuint _blockIndex;
    GLint _blockSize;
    GLubyte *_paramsBuffer;
    GLuint _uniformBufferObj;
};
