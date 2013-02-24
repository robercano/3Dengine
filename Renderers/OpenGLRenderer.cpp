/**
 * @class	OpenGLRenderer
 * @brief	OpenGL based 3D renderer
 *
 * @author	Roberto Sosa Cano
 */

#include "OpenGLRenderer.hpp"
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

OpenGLRenderer::~OpenGLRenderer()
{
	std::vector<Object3D*>::iterator it = _objects.begin();
	for (; it != _objects.end(); ++it) {
		(*it)->destroy();
	}
}

uint32_t OpenGLRenderer::loadShaders(std::string &vertexShader, std::string &fragmentShader)
{
	GLuint VertexShaderID   = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	GLint Result = GL_FALSE;
	int InfoLogLength;

	const char * VertexSourcePointer = vertexShader.c_str();

	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	glCompileShader(VertexShaderID);
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	char const * FragmentSourcePointer = fragmentShader.c_str();

	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);

	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}
void OpenGLRenderer::init()
{
/*	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);*/
}

bool OpenGLRenderer::render()
{
	std::vector<Object3D*>::iterator it = _objects.begin();
	for (; it != _objects.end(); ++it) {
		(*it)->render();
	}
}

bool OpenGLRenderer::resize(uint16_t width, uint16_t height)
{
	_width  = width;
	_height = height;
}

bool OpenGLRenderer::addObject(Object3D *object)
{
	object->init();
	_objects.push_back(object);
}
