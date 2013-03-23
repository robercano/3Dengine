#include <string.h>
#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>
using namespace glm;
#include <string>
#include <stddef.h>

#include "Plane.hpp"
#include "Procedural.hpp"
#include "Renderer.hpp"

using namespace std;

static uint32_t numIndices = 0;
bool Plane::init()
{
	/* Generate the procedural plane */
	procedural::Plane plane;
	glm::vec4 xaxis[] = {
		glm::vec4(0.0, 0.0, -1.0, 0.0),
		glm::vec4(1.0, 0.0, 0.0, 0.0),
		glm::vec4(0.0, 0.0, 1.0, 0.0),
		glm::vec4(-1.0, 0.0, 0.0, 0.0),
		glm::vec4(0.0, 1.0, 0.0, 0.0),
		glm::vec4(0.0, -1.0, 0.0, 0.0) };
	glm::vec4 yaxis[] = {
		glm::vec4(0.0, 1.0, 0.0, 0.0),
		glm::vec4(0.0, 1.0, 0.0, 0.0),
		glm::vec4(0.0, 1.0, 0.0, 0.0),
		glm::vec4(0.0, 1.0, 0.0, 0.0),
		glm::vec4(-1.0, 0.0, 0.0, 0.0),
		glm::vec4(1.0, 0.0, 0.0, 0.0) };
	glm::vec4 zaxis[] = {
		glm::vec4(1.0, 0.0, 0.0, 0.0),
		glm::vec4(0.0, 0.0, 1.0, 0.0),
		glm::vec4(-1.0, 0.0, 0.0, 0.0),
		glm::vec4(0.0, 0.0, -1.0, 0.0),
		glm::vec4(0.0, 0.0, 1.0, 0.0),
		glm::vec4(0.0, 0.0, 1.0, 0.0) };

	for (int i=0; i<6; ++i) {
		plane.addPlane(100, 100, 1.0, 1.0, 1.0, xaxis[i], yaxis[i], zaxis[i]);
	}

	const vector<procedural::pack> *packs = plane.getGeometry();
	const vector<uint32_t>         *index = plane.getIndices();

	const void *vertices = &(*packs)[0];
	const void *indices  = &(*index)[0];

	uint32_t vertices_size = packs->size()*sizeof(procedural::pack);
	numIndices = index->size();
	uint32_t indices_size  = index->size()*sizeof(uint32_t);

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
	glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices, GL_STATIC_DRAW);

	/* Specify vertex attribute */
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			4,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			sizeof(procedural::pack),                  // stride
			(void*)0            // array buffer offset
			);

	/* Link the data with the second shader attribute */
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
			1,                  // attribute
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			sizeof(procedural::pack),// stride
			(void*)sizeof(glm::vec4)            // array buffer offset
			);

	/* Generate the buffer for the indices */
	glGenBuffers(1, &_indicesVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indicesVBO);

	/* Upload the data */
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size, indices, GL_STATIC_DRAW);
}

bool Plane::destroy()
{
	glDeleteBuffers(1, &_verticesVBO);
	glDeleteVertexArrays(1, &_gVAO);
}

bool Plane::render(const glm::mat4 &projection, const glm::mat4 &view)
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
	glDrawElements(GL_TRIANGLE_STRIP, numIndices, GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);

	_shader->detach();
}
