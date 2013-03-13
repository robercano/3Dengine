#include <string.h>
#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>
using namespace glm;
#include <string>

#include "Icosahedron.hpp"
#include "Renderer.hpp"

bool Icosahedron::init()
{
	// To angles are used to explore the icosahedron: theta and phi
	//
	// phi is the angle of rotation in the plane XZ
	// theta is the angle of rotation in the plane YZ
	//
	// Starting with theta = phi = 0
	//    Generate vertex (0)
	//
	// Rotate phi 60 degrees
	//    Generate a vertex (1, 2, 3, 4, 5)
	//    Rotate theta by 72 degrees
	//    Repeat until theta is 0 again
	//
	// Rotate phi another 60 degrees and set theta to 36 degrees
	//    Generate a vertex (6, 7, 8, 9, 10)
	//    Rotate theta by 72 degrees
	//    Repeat until theta is 36 again
	//
	// Rotate phi another 60 degree and set theta to 0
	//    Generate a vertex (11)
	//
	// Vertex 0 forms a fan with vertices 1, 2, 3, 4 and 5
	// Vertex 11 forms a fan with vertices 6, 7, 8, 9 and 10
	// Vertices 1, 6, 2, 7, 3, 8, 4, 9, 5, 10 and 1 again forms a triangle strip


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
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

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
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

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
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

bool Icosahedron::destroy()
{
	glDeleteBuffers(1, &_colorsVBO);
	glDeleteBuffers(1, &_verticesVBO);
	glDeleteVertexArrays(1, &_gVAO);
}

bool Icosahedron::render(const glm::mat4 &projection, const glm::mat4 &view)
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
}
