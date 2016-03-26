/**
 * @class	OpenGLUniformBlock
 * @brief	Manages the access to a uniform block in a shader. It takes care
 *          of finding out the block size and offsets, then allows to set the
 *          values in the block and bind it
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#include <string.h>
#include "OpenGLUniformBlock.hpp"

OpenGLUniformBlock::OpenGLUniformBlock() :
    _programID(0), _bindingPoint(0), _linkedToShader(false), _blockArrayIndex(-1),
    _blockIndex(0), _blockSize(0), _paramsBuffer(NULL), _uniformBufferObj(0)
{
}

OpenGLUniformBlock::~OpenGLUniformBlock()
{
    if (_linkedToShader) {
        glDeleteBuffers(1, &_uniformBufferObj);
        delete[] _paramsBuffer;
    }
}

void OpenGLUniformBlock::addParamName(const std::string &name)
{
    _paramsOffsets[name] = -1;
    _paramsFullName.push_back(_blockName + std::string(".") + name);
}

bool OpenGLUniformBlock::prepareForShader(GLuint programID)
{
	const GLchar **names = NULL;
	GLuint *indices = NULL;
	GLint *offsets = NULL;
    std::map<std::string, GLint>::iterator it;
    std::string accessName;
	bool ret = true;

	names = new const GLchar*[_paramsFullName.size()];
	indices = new GLuint[_paramsFullName.size()];
	offsets = new GLint[_paramsFullName.size()];

    /* Prepare array of names pointers */
    for (size_t i=0; i<_paramsFullName.size(); ++i) {
        names[i] = _paramsFullName[i].c_str();
    }

    if (_blockArrayIndex == -1) {
        accessName = _blockName;
    } else {
        accessName = _blockName + std::string("[") + std::to_string(_blockArrayIndex) + std::string("]");
    }

    GL( _blockIndex = glGetUniformBlockIndex(programID, accessName.c_str()) );
    if (_blockIndex == GL_INVALID_INDEX) {
        fprintf(stderr, "ERROR OpenGLUniformBlock bad block index for block: %s\n", accessName.c_str());
        ret = false;
		goto error;
    }


    GL( glGetActiveUniformBlockiv(programID, _blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &_blockSize) );
    GL( glGetUniformIndices(programID, _paramsFullName.size(), names, indices) );
    for (size_t i=0; i<_paramsOffsets.size(); ++i) {
        if (indices[i] == GL_INVALID_INDEX) {
            fprintf(stderr, "ERROR OpenGLUniformBlock could not get all indices\n");
			ret = false;
			goto error;
        }
    }

    /* Allocate the offsets and the data buffer */
    _paramsBuffer = new GLubyte[_blockSize];

    /* Retrieve the offsets */
    GL( glGetActiveUniformsiv(programID, _paramsFullName.size(), indices, GL_UNIFORM_OFFSET, offsets) );

    /* Associate them with their names */
    for (size_t i=0; i<_paramsFullName.size(); ++i) {
        std::string baseName = _paramsFullName[i].substr(_blockName.length() + 1);
        _paramsOffsets[baseName] = offsets[i];
    }

    GL( glGenBuffers(1, &_uniformBufferObj) );
    GL( glBindBuffer(GL_UNIFORM_BUFFER, _uniformBufferObj) );
    GL( glBufferData(GL_UNIFORM_BUFFER, _blockSize, NULL, GL_DYNAMIC_DRAW) );
    GL( glBindBufferBase(GL_UNIFORM_BUFFER, _bindingPoint, _uniformBufferObj) );

    _linkedToShader = true;
    _programID = programID;

error:
	delete[] names;
	delete[] indices;
	delete[] offsets;

    return ret;
}

void OpenGLUniformBlock::bind()
{
    GL( glBindBuffer(GL_UNIFORM_BUFFER, _uniformBufferObj) );
    GL( glBufferSubData(GL_UNIFORM_BUFFER, 0, _blockSize, _paramsBuffer) );
    GL( glBindBuffer(GL_UNIFORM_BUFFER, 0) );
    GL( glUniformBlockBinding(_programID, _blockIndex, _bindingPoint) );
}
