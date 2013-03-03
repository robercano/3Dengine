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

WindowManager *wmanager = NULL;
double angle = 0.0;
double far = 8.0;

class MyKeyListener : public KeyListener
{
	public:
		void processKey(uint32_t key, uint32_t flags)
		{
			switch (key) {
				case GLFW_KEY_LEFT:
					angle -= 3.0;
					break;
				case GLFW_KEY_RIGHT:
					angle += 3.0;
					break;
				case GLFW_KEY_UP:
					far *= 0.95;
					break;
				case GLFW_KEY_DOWN:
					far *= 1.05;
					break;
				case GLFW_KEY_ESC:
					wmanager->stop();
					break;
				default:
					break;
			}
		}
};

class MyMouseListener : public MouseListener
{
	public:
		void processMouse(int32_t x, int32_t y)
		{
			printf("-------- (%d, %d)\n", x, y);
		}
};

class Simple : public Object3D
{
	public:
		bool init()
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

			/* Background color */
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

			/* Basic shaders with only position and color attributes, with no camera */
			std::string vertexShader("#version 330 core\
					layout(location = 0) in vec3 position;\
					layout(location = 1) in vec3 color;\
					uniform mat4 MVP;\
					out vec3 fragment_color;\
					void main(){\
					gl_Position    = MVP * vec4(position, 1);\
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

			/* Get the location of the MVP attribute in the shader */
			_matrixID = glGetUniformLocation(_programID, "MVP");

			glEnable(GL_DEPTH_TEST);
		}

		bool destroy()
		{
			glDeleteBuffers(1, &_colorsVBO);
			glDeleteBuffers(1, &_verticesVBO);
			glDeleteVertexArrays(1, &_gVAO);
			glDeleteProgram(_programID);
		}

		bool render()
		{

			const double speed = 2.0;

			double x = far*sin(angle*M_PI/180.0);
			double z = far*cos(angle*M_PI/180.0);

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			/* Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units */
			glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);

			/* Camera matrix */
			glm::mat4 View       =	glm::lookAt(
					glm::vec3(x,far,z), // Camera is at (4,3,-3), in World Space
					glm::vec3(0,0,0), // and looks at the origin
					glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
					);
			/* Model matrix : an identity matrix (model will be at the origin) */
			glm::mat4 Model      = glm::mat4(1.0f);

			/* Our ModelViewProjection : multiplication of our 3 matrices */
			_MVP        = Projection * View * Model; // Remember, matrix multiplication is the other way around

			/* Bind program to upload the uniform */
			glUseProgram(_programID);

			/* Send our transformation to the currently bound shader, in the "MVP" uniform */
			glUniformMatrix4fv(_matrixID, 1, GL_FALSE, &_MVP[0][0]);

			/* Clear the buffer */
			glBindVertexArray(_gVAO);
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, NULL);
			glBindVertexArray(0);
			glUseProgram(0);
		}

	private:
		glm::mat4 _MVP;
		uint32_t _programID;
		uint32_t _gVAO;
		uint32_t _verticesVBO;
		uint32_t _colorsVBO;
		uint32_t _indicesVBO;
		uint32_t _matrixID;
};

int main(int argc, char** argv)
{
	wmanager = WindowManager::GetWindowManager(WindowManager::WINDOW_MANAGER_GLFW);
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
	wmanager->createWindow(windowName, 1440, 900);

	renderer->init();	// only after creating the window
	wmanager->setRenderer(renderer);

	MyKeyListener keyhandler;
	std::vector<uint32_t> keys;

	keys.push_back(GLFW_KEY_UP);
	keys.push_back(GLFW_KEY_DOWN);
	keys.push_back(GLFW_KEY_LEFT);
	keys.push_back(GLFW_KEY_RIGHT);
	keys.push_back(GLFW_KEY_ESC);

	wmanager->getKeyManager()->registerListener(keyhandler, keys);

	MyMouseListener mousehandler;
	//wmanager->getMouseManager()->registerListener(mousehandler);
	wmanager->getMouseManager();

	Simple simple;
	renderer->addObject(&simple);

	wmanager->loop();

	WindowManager::DisposeWindowManager(wmanager);
	delete renderer;

	return 0;
}
