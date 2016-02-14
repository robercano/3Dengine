/**
 * @class	Object3D
 * @brief	Basic object 3D representation
 *
 * @author	Roberto Sosa Cano
 */
#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>

#include "Object3D.hpp"

/**
 * Adds a new shader to the object to be used for rendering
 *
 * @shader   The shader to be added
 *
 * @returns true if the shader was added correctly, false if not
 */
bool Object3D::addShader(Shader *shader)
{
    /* TODO: allow for a list of shaders */
    _shader = shader;
    return true;
}

bool Object3D::render(const glm::mat4 &projection, const glm::mat4 &view)
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
	glBindVertexArray(getVertexArrayIndex());
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, NULL);
	glBindVertexArray(0);

	_shader->detach();
    return true;
};
