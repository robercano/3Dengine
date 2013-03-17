#include <string.h>
#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>
using namespace glm;
#include <string>

#include "Sphere.hpp"
#include "Renderer.hpp"

#if 0
	GLfloat vertices[][3] = {
		{  1.0,  1.0,  1.0 },
		{  1.0, -1.0,  1.0 } ,
		{  1.0,  1.0, -1.0 },
		{  1.0, -1.0, -1.0 },
		{ -1.0,  1.0, -1.0 },
		{ -1.0, -1.0, -1.0 },
		{ -1.0,  1.0,  1.0 },
		{ -1.0, -1.0,  1.0 }};

	GLfloat colors[][3] = {
		{  1.0,  1.0,  1.0 },
		{  1.0,  1.0,  1.0 },
		{  1.0,  1.0,  1.0 },
		{  1.0,  1.0,  1.0 },
		{  1.0,  1.0,  1.0 },
		{  1.0,  1.0,  1.0 },
		{  1.0,  1.0,  1.0 },
		{  1.0,  1.0,  1.0 } };

	GLubyte indices[] = {
		0, 1, 2, 3, 4, 5, 6, 7, 0, 1,
		0,
		0, 6, 2, 4,
		4, 5,
		5, 7, 3, 1
	};
#endif

static uint32_t numIndices = 0;
bool Sphere::init()
{
	/**
	 * Subdivision of a cube into more triangles t use it for a sphere creation
	 *
	 * For a subdivision level of N, the number of vertices for the main body
	 * strip are:
	 *
	 *     - H = 2ˆ(N+2)
	 *     - V = 2^N + 1
	 *     - TotalVerts = H*V
	 *
	 *     At level 0 we get H = 4, V = 2
	 */

	/* Preallocate vertices and color */
	typedef struct {
		GLfloat x, y, z;
	} vertex;

	typedef struct {
		GLfloat r, g, b;
	} color;

	uint8_t level = 7; /* Beware! max 8 for now! */
	uint32_t H = (1<<level) + 1;
	uint32_t V = (1<<level) + 1;
	uint32_t horizontal = (H-1) * 4; /* 4 sides */
	uint32_t vertical   = V;

	vertex *vertices = new vertex[horizontal*vertical];
	color *colors    = new color[horizontal*vertical];

	uint32_t i, j;

	for (i=0; i < vertical; ++i) {
		for(j=0; j < horizontal; ++j) {
			if (j < H-1) { /* z=0 */
				vertices[i*horizontal + j].x =       j/(H - 1.0);
				vertices[i*horizontal + j].y = 1.0 - (i/(V - 1.0));
				vertices[i*horizontal + j].z = 0.0;
			} else if (j < 2*(H-1)) { /* x=1 */
				vertices[i*horizontal + j].x = 1.0;
				vertices[i*horizontal + j].y = 1.0 - (i/(V - 1.0));
				vertices[i*horizontal + j].z = 0.0 - ((j-(H-1))/(H - 1.0));
			} else if (j < 3*(H-1)) { /* z=-1 */
				vertices[i*horizontal + j].x = 1.0 - ((j-2*(H-1))/(H - 1.0));
				vertices[i*horizontal + j].y = 1.0 - (i/(V - 1.0));
				vertices[i*horizontal + j].z = -1.0;
			} else if (j < 4*(H-1)) { /* x=0.0 */
				vertices[i*horizontal + j].x = 0.0;
				vertices[i*horizontal + j].y = 1.0  - (i/(V - 1.0));
				vertices[i*horizontal + j].z = ((j-3*(H-1))/(H - 1.0)) - 1.0;
			}
			colors[i*horizontal + j].r = 1.0;
			colors[i*horizontal + j].g = 1.0;
			colors[i*horizontal + j].b = 1.0;
		}
	}

	/* Generate indices */
	numIndices = 2*(horizontal+1)*(vertical-1);
	GLushort *indices = new GLushort[numIndices];
	int count = 0;
	for (i=0; i<vertical-1; i++) {
		for (j=0; j<horizontal; j++) {
			indices[count++] =  i   *horizontal + j;
			indices[count++] = (i+1)*horizontal + j;
		}
		/* Repeat the first ones */
		indices[count++] =  i   *horizontal;
		indices[count++] = (i+1)*horizontal;
	}
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
	glBufferData(GL_ARRAY_BUFFER, horizontal*vertical*sizeof(vertex), vertices, GL_STATIC_DRAW);

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
	glBufferData(GL_ARRAY_BUFFER, horizontal*vertical*sizeof(color), colors, GL_STATIC_DRAW);

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
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices*sizeof(GLushort), indices, GL_STATIC_DRAW);
}

bool Sphere::destroy()
{
	glDeleteBuffers(1, &_colorsVBO);
	glDeleteBuffers(1, &_verticesVBO);
	glDeleteVertexArrays(1, &_gVAO);
}

bool Sphere::render(const glm::mat4 &projection, const glm::mat4 &view)
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
	glDrawElements(GL_TRIANGLE_STRIP, numIndices, GL_UNSIGNED_SHORT, NULL);
	glBindVertexArray(0);

	_shader->detach();
}
