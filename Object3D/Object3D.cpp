/**
 * @class	Object3D
 * @brief	Basic object 3D representation
 *
 * @author	Roberto Sosa Cano
 */
#include "Object3D.hpp"

bool Object3D::init()
{
	/* Generate a vertex array to reference the attributes */
	glGenVertexArrays(1, &_gVAO);
	glBindVertexArray(_gVAO);

	/* Generate a buffer object for the vertices positions */
	glGenBuffers(1, &_verticesVBO);
	glBindBuffer(GL_ARRAY_BUFFER, _verticesVBO);

	/* Upload the data for this buffer */
	glBufferData(GL_ARRAY_BUFFER, getVerticesArrayLen() * sizeof(GLfloat), getVerticesArray(), GL_STATIC_DRAW);

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
	glBufferData(GL_ARRAY_BUFFER, getColorsArrayLen() * sizeof (GLfloat), getColorsArray(), GL_STATIC_DRAW);

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

	/* Generate a buffer for the vertices colors */
	glGenBuffers(1, &_normalsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, _normalsVBO);

	/* Upload the data for this buffer */
	glBufferData(GL_ARRAY_BUFFER, getNormalsArrayLen() * sizeof (GLfloat), getNormalsArray(), GL_STATIC_DRAW);

	/* Link the data with the second shader attribute */
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(
			2,                  // attribute
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
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, getIndicesArrayLen() * sizeof(GLuint), getIndicesArray(), GL_STATIC_DRAW);
    return true;
}

bool Object3D::destroy()
{
	glDeleteBuffers(1, &_colorsVBO);
	glDeleteBuffers(1, &_verticesVBO);
	glDeleteBuffers(1, &_normalsVBO);
	glDeleteVertexArrays(1, &_gVAO);
    return true;
}

bool Object3D::addShader(Shader *shader)
{
    /* TODO: allow for a list of shaders */
    _shader = shader;
    return true;
}

bool Object3D::render(const glm::mat4 &projection, const glm::mat4 &view, RenderTarget &renderTarget)
{
	/* Model matrix : an identity matrix (model will be at the origin) */
	glm::mat4 model      = glm::mat4(1.0f);

	/* Our ModelViewProjection : multiplication of our 3 matrices */
	glm::mat4 MVP = projection * view * model; // Remember, matrix multiplication is the other way around

    /* Bind the render target */
    glBindFramebuffer(GL_FRAMEBUFFER, renderTarget.getID());

	/* Bind program to upload the uniform */
	_shader->attach();

	/* Send our transformation to the currently bound shader, in the "MVP" uniform */
	_shader->setUniform("MVP", MVP);

	/* Draw the object */
	glBindVertexArray(_gVAO);
	glDrawElements(GL_TRIANGLES, getIndicesArrayLen(), GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);

    /* Unbind */
	_shader->detach();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return true;
};
