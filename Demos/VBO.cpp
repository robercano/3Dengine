/*
 * Vertex Buffer Objects demo
 *
 * @author	Roberto Sosa Cano
 */

#include <stdio.h>
#include <string>
#include <vector>

#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>
#include <GL/glfw.h>
#include <OpenGL/gl.h>
#include "WindowManager.hpp"
#include "OpenGLRenderer.hpp"

class Triangle : public Object3D
{
	public:
		~Triangle() {}

		bool init()
		{
			glGenVertexArrays(1, &_VertexArrayID);
			glBindVertexArray(_VertexArrayID);

			// Create and compile our GLSL program from the shaders
			_programID = loadSimpleShaders();
			static const GLfloat g_vertex_buffer_data[] = { 
				-1.0f, -1.0f, 0.0f,
				1.0f, -1.0f, 0.0f,
				0.0f,  1.0f, 0.0f,
			};

			glGenBuffers(1, &_vertexbuffer);
			glBindBuffer(GL_ARRAY_BUFFER, _vertexbuffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
		}

		bool destroy()
		{
			glDeleteBuffers(1, &_vertexbuffer);
			glDeleteVertexArrays(1, &_VertexArrayID);
		}

		bool render()
		{
			glUseProgram(_programID);

			glEnableVertexAttribArray(0);

			glBindBuffer(GL_ARRAY_BUFFER, _vertexbuffer);
			glVertexAttribPointer(
					0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
					3,                  // size
					GL_FLOAT,           // type
					GL_FALSE,           // normalized?
					0,                  // stride
					(void*)0            // array buffer offset
					);

			// Draw the triangle !
			glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle

			glDisableVertexAttribArray(0);
		}

		GLuint loadSimpleShaders()
		{
			GLuint VertexShaderID   = glCreateShader(GL_VERTEX_SHADER);
			GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
			GLint Result = GL_FALSE;
			int InfoLogLength;

			const char * VertexSourcePointer = "#version 330 core\
				layout(location = 0) in vec3 vertexPosition_modelspace;\
				void main() {\
					gl_Position.xyz = vertexPosition_modelspace;\
					gl_Position.w = 1.0;\
				}\n";

			glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
			glCompileShader(VertexShaderID);
			glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
			glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
			if ( InfoLogLength > 0 ){
				std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
				glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
				printf("%s\n", &VertexShaderErrorMessage[0]);
			}

			char const * FragmentSourcePointer = "#version 330 core\
				out vec3 color;\
				void main() { color = vec3(1,0,0); }\n";
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

private:
		GLuint _VertexArrayID;
		GLuint _vertexbuffer;
		GLuint _programID;
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

	Triangle triangle;
	renderer->addObject(&triangle);

	wmanager->loop();
	return 0;
}
