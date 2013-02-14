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
		 * @brief	Sets a new renderer to handle display requests
		 *
		 * @param	renderer	Renderer that will handle display/reshape requests
		 *
		 * @return	true or false
		 */
		bool setRenderer(Renderer *renderer);

		/**
		 * Enters GLUT main loop
		 */
		void loop(void);

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
		 * Calls the registered render to resize the scene
		 */
		void resize(uint16_t width, uint16_t height);

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
