#include <string.h>
#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>
using namespace glm;
#include <string>

#include "ColorCube.hpp"
#include "OpenGLRenderer.hpp"

ColorCube::~ColorCube()
{
}

bool ColorCube::init()
{
	glClearColor(0.0f, 0.0f, 0.4f, 1.0f);

	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glGenVertexArrays(1, &_VertexArrayID);
	glBindVertexArray(_VertexArrayID);

	// Create and compile our GLSL program from the shaders
	std::string vertexShader("#version 330 core\
			layout(location = 0) in vec3 vertexPosition_modelspace;\
			layout(location = 1) in vec3 vertexColor;\
			out vec3 fragmentColor;\
			uniform mat4 MVP;\
			void main(){\
			gl_Position =  MVP * vec4(vertexPosition_modelspace,1);\
			fragmentColor = vertexColor; }\n");

	std::string fragmentShader("#version 330 core\
			in vec3 fragmentColor;\
			out vec3 color;\
			void main(){ color = fragmentColor;}\n");
	_programID = OpenGLRenderer::loadShaders(vertexShader, fragmentShader);

	// Get a handle for our "MVP" uniform
	_MatrixID = glGetUniformLocation(_programID, "MVP");

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	glm::mat4 View       =	glm::lookAt(
			glm::vec3(4,3,3), // Camera is at (4,3,-3), in World Space
			glm::vec3(0,0,0), // and looks at the origin
			glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
			);
	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 Model      = glm::mat4(1.0f);
	// Our ModelViewProjection : multiplication of our 3 matrices
	_MVP        = Projection * View * Model; // Remember, matrix multiplication is the other way around

	// Our vertices. Tree consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
	// A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
	static const GLfloat g_vertex_buffer_data[] = {
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f,-1.0f, 1.0f
	};

	// One color for each vertex. They were generated randomly.
	static GLfloat g_color_buffer_data[36*3];
	for (int i=0;i<36; i++) {
		g_color_buffer_data[3*i]   = g_vertex_buffer_data[3*i+2] < 0 ? 0.0f : 1.0f;
		g_color_buffer_data[3*i+1] = g_vertex_buffer_data[3*i]   < 0 ? 0.0f : 1.0f;
		g_color_buffer_data[3*i+2] = g_vertex_buffer_data[3*i+1] < 0 ? 0.0f : 1.0f;
	}

	glGenBuffers(1, &_vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	glGenBuffers(1, &_colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, _colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);
}

bool ColorCube::destroy()
{
	// Cleanup VBO and shader
	glDeleteBuffers(1, &_vertexbuffer);
	glDeleteBuffers(1, &_colorbuffer);
	glDeleteProgram(_programID);
	glDeleteVertexArrays(1, &_VertexArrayID);
}

bool ColorCube::render()
{
	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(_programID);

	// Send our transformation to the currently bound shader,
	// in the "MVP" uniform
	glUniformMatrix4fv(_MatrixID, 1, GL_FALSE, &_MVP[0][0]);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexbuffer);
	glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
			);

	// 2nd attribute buffer : colors
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, _colorbuffer);
	glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
			);

	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, 12*3); // 12*3 indices starting at 0 -> 12 triangles

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

