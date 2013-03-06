/*
 * Vertex Buffer Objects demo
 *
 * @author	Roberto Sosa Cano
 */
#include <string.h>
#include <GL/glew.h>
#include <GL/glfw.h>
#include <string>
#include <stdio.h>
#include "WindowManager.hpp"
#include "ObjectLib.hpp"
#include "OpenGLRenderer.hpp"

class Simple : public Object3D
{
	public:
		bool init()
		{
			static const GLfloat cubeVerts[] = {
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

			/* Colors for the vertices */
			static GLfloat cubeColors[36*3];
			for (int i=0;i<36; i++) {
				cubeColors[3*i]   = cubeVerts[3*i+2] < 0 ? 0.0f : 1.0f;
				cubeColors[3*i+1] = cubeVerts[3*i]   < 0 ? 0.0f : 1.0f;
				cubeColors[3*i+2] = cubeVerts[3*i+1] < 0 ? 0.0f : 1.0f;
			}

			/* Background color */
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

			/* Basic shaders with only position and color attributes, with no camera */
			std::string vertexShader("#version 330 core\
					layout(location = 0) in vec3 position;\
					layout(location = 1) in vec3 color;\
					out vec3 fragment_color;\
					void main(){\
						gl_Position    = vec4(position, 3);\
						fragment_color = color; }\n");

			std::string fragmentShader("#version 330 core\
					in vec3 fragment_color;\
					out vec3 color;\
					void main(){ color = fragment_color.bgr;}\n");
			_programID = OpenGLRenderer::loadShaders(vertexShader, fragmentShader);

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
					1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
					3,                                // size
					GL_FLOAT,                         // type
					GL_FALSE,                         // normalized?
					0,                                // stride
					(void*)0                          // array buffer offset
					);
		}

		bool destroy()
		{
			/* Delete VAO, VBOs and shaders */
			glDeleteBuffers(1, &_verticesVBO);
			glDeleteBuffers(1, &_colorsVBO);
			glDeleteProgram(_programID);
			glDeleteVertexArrays(1, &_gVAO);
		}


		bool render()
		{
			/* Clear the screen */
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			/* Bind the shader program */
			glUseProgram(_programID);

			/* Bind the geometry */
			glBindVertexArray(_gVAO);

			// Draw the triangle !
			glDrawArrays(GL_TRIANGLES, 0, 12*3); // 12*3 indices starting at 0 -> 12 triangles

			/* Unbind the geometry */
			glBindVertexArray(0);

			/* Unbind the shader */
			glUseProgram(0);
		}

	private:
		uint32_t _gVAO;
		uint32_t _verticesVBO;
		uint32_t _colorsVBO;
		uint32_t _programID;
};

int main(int argc, char** argv)
{
	WindowManager *wmanager = WindowManager::GetWindowManager(WindowManager::WINDOW_MANAGER_GLFW);
	if (!wmanager) {
		fprintf(stderr, "ERROR creating new window manager\n");
		exit(1);
	}

	Renderer *renderer = new OpenGLRenderer();
	if (!renderer) {
		fprintf(stderr, "ERROR allocating renderer\n");
		WindowManager::DisposeWindowManager(wmanager);
		exit(1);
	}

	/* Init the window manager and the render*/
	wmanager->init();

	/* Set the window size */
	std::string windowName("OpenGL Test");
	wmanager->createWindow(windowName, 1024, 768);

	renderer->init();	// only after creating the window
	wmanager->setRenderer(renderer);

	Simple simple;
	renderer->addObject(&simple);

	wmanager->loop();
	return 0;
}
