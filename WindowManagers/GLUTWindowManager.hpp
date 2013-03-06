/**
 * @class	GLUTWindowManager
 * @brief	GLUT based window manager
 *
 * @author	Roberto Sosa Cano
 * @todo	Standarize the interface to make a generic Window Manager
 */
#ifndef __GLUTWINDOWMANAGER_HPP__
#define __GLUTWINDOWMANAGER_HPP__

#include <stdint.h>
#include "WindowManager.hpp"

class GLUTWindowManager : public WindowManager
{
	public:
		/**
		 * @brief Constructor of the class
		 */
		GLUTWindowManager(void);

		/**
		 * Gets the associated key manager
		 *
		 * @return The key manager or NULL if no manager is available
		 */
		KeyManager *getKeyManager();

		/**
		 * Gets the associated mouse manager
		 *
		 * @return The mouse manager or NULL if no manager is available
		 */
		MouseManager *getMouseManager();

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
		 * @param	name		Title of the window
		 * @param	width		Width of the window
		 * @param	height		Height of the window
		 * @param	fullscreen	Set the window to fullscreen
		 *
		 * @return  true or false
		 */
		bool createWindow(std::string &name, uint16_t width, uint16_t height, bool fullscreen);

		/**
		 * Called when the window is resized
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
		 * Swaps the double buffer
		 */
		void swapBuffers(void);

		/**
		 * Polls for input events
		 */
		void poll(void);

	private:
		/**
		 * Internal callback for GLUT C interface render function
		 */
		static void display_callback(void);

		/**
		 * Calls the registered render to display the scene
		 */
		void display(void);

		/**
		 * Internal callback for GLUT C interface resize function
		 */
		static void resize_callback(int width, int height);

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
