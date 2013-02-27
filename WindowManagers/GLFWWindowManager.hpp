/**
 * @class	GLFWWindowManager
 * @brief	GLFW based window manager
 *
 * @author	Roberto Sosa Cano
 * @todo	Standarize the interface to make a generic Window Manager
 */
#ifndef __GLFWWINDOWMANAGER_HPP__
#define __GLFWWINDOWMANAGER_HPP__

#include <stdint.h>
#include <GL/glew.h>
#include <GL/glfw.h>
#include "WindowManager.hpp"
#include "GLFWKeyManager.hpp"

class GLFWWindowManager : public WindowManager
{
	public:
		/**
		 * @brief Constructor of the class
		 */
		GLFWWindowManager(void);

		/**
		 * Gets the associated key manager
		 *
		 * @return The key manager or NULL if no manager is available
		 */
		KeyManager *getKeyManager();

		/**
		 * @brief	Initializes the window manager
		 * @details	Prepares the window manager to be used. After calling
		 *          this method the rest of the methods can be called
		 *
		 * @return	true or false
		 */
		bool init(void);

		/**
		 * @brief	Creates a new window
		 *
		 * @param	name	Title of the window
		 * @param	width	Width of the window
		 * @param	height	Height of the window
		 *
		 * @return  true or false
		 */
		bool createWindow(std::string &name, uint16_t width, uint16_t height);

		/**
		 * Called when the window is resized
		 *
		 * @param	width	Width of the window
		 * @param	height	Height of the window
		 *
		 * @return true or false
		 */
		bool resize(uint16_t width, uint16_t height);

		/**
		 * @brief	Sets a new renderer to handle display requests
		 *
		 * @param	renderer	Renderer that will handle display/reshape requests
		 *
		 * @return	true or false
		 */
		bool setRenderer(Renderer *renderer);

		/**
		 * Enters main loop
		 */
		void loop(void);

	private:
		/**
		 * Static callback for resize of the window
		 */
		static void GLFWCALL handle_resize(int width,int height);

		/**
		 * Width of the created window
		 */
		uint16_t _width;

		/**
		 * Height of the created window
		 */
		uint16_t _height;

		/**
		 * Renderer associated to this manager
		 */
		Renderer *_renderer;

};

#endif
