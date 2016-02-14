#include <string.h>
#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>
using namespace glm;
#include <string>

#include "Cube.hpp"
#include "Renderer.hpp"

using namespace procedural;

bool Cube::init()
{
	GLfloat cubeVerts[][3] = {
		{ -1.0, -1.0, -1.0 },
		{ -1.0, -1.0,  1.0 },
		{ -1.0,  1.0, -1.0 },
		{ -1.0,  1.0,  1.0 },
		{  1.0, -1.0, -1.0 },
		{  1.0, -1.0,  1.0 },
		{  1.0,  1.0, -1.0 },
		{  1.0,  1.0,  1.0 } };

	GLfloat cubeColors[][3] = {
		{  0.0,  0.0,  0.0 },
		{  0.0,  0.0,  1.0 },
		{  0.0,  1.0,  0.0 },
		{  0.0,  1.0,  1.0 },
		{  1.0,  0.0,  0.0 },
		{  1.0,  0.0,  0.0 },
		{  1.0,  1.0,  0.0 },
		{  1.0,  1.0,  1.0 } };

	GLubyte cubeIndices[] = {
		0, 1, 2,
		2, 1, 3,
		0, 2, 4,
		4, 2, 6,
		4, 6, 5,
		5, 6, 7,
		2, 3, 6,
		6, 3, 7,
		5, 7, 3,
		5, 3, 1,
		1, 4, 5,
		4, 1, 0
	};

	/* Request a new shader */
	_shader = Renderer::GetRenderer()->getShader();

	/* Basic shaders with only position and color attributes, with no camera */
	std::string error;
	if (_shader->loadVertexShader("Shaders/mvp.vert", error) == false) {
		printf("ERROR compiling vertex shader: %s\n", error.c_str());
		return false;
	}
	if (_shader->loadFragmentShader("Shaders/color.frag", error) == false) {
		printf("ERROR compiling fragment shader: %s\n", error.c_str());
		return false;
	}
	if (_shader->linkProgram(error) == false) {
		printf("ERROR linking shader: %s\n", error.c_str());
		return false;
	}

	/* Generate a vertex array to reference the attributes */
	glGenVertexArrays(1, &_gVAO);
	glBindVertexArray(_gVAO);

	/* Generate a buffer object for the vertices positions */
	glGenBuffers(1, &_verticesVBO);
	glBindBuffer(GL_ARRAY_BUFFER, _verticesVBO);

	/* Upload the data for this buffer */
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVerts), cubeVerts, GL_STATIC_DRAW);

	/* Link the data with the first shader attribute */
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
			);

	/* Generate a buffer for the vertices colors */
	glGenBuffers(1, &_colorsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, _colorsVBO);

	/* Upload the data for this buffer */
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeColors), cubeColors, GL_STATIC_DRAW);

	/* Link the data with the second shader attribute */
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
			1,                  // attribute
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
			);

	/* Generate the buffer for the indices */
	glGenBuffers(1, &_indicesVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indicesVBO);

	/* Upload the data */
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);
    return true;
}

bool Cube::destroy()
{
	glDeleteBuffers(1, &_colorsVBO);
	glDeleteBuffers(1, &_verticesVBO);
	glDeleteVertexArrays(1, &_gVAO);
    return true;
}

bool Cube::render(const glm::mat4 &projection, const glm::mat4 &view)
{
	/* Model matrix : an identity matrix (model will be at the origin) */
	glm::mat4 model      = glm::mat4(1.0f);

	/* Our ModelViewProjection : multiplication of our 3 matrices */
	glm::mat4 MVP = projection * view * model; // Remember, matrix multiplication is the other way around

	/* Bind program to upload the uniform */
	_shader->attach();

	/* Send our transformation to the currently bound shader, in the "MVP" uniform */
	_shader->setUniform("MVP", MVP);

	/* Clear the buffer */
	glBindVertexArray(_gVAO);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, NULL);
	glBindVertexArray(0);

	_shader->detach();
    return true;
}
