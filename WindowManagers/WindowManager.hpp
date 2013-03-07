/**
 * @class	WindowManager
 * @brief	Interface for window manager management
 *
 * @author	Roberto Sosa Cano
 */
#ifndef __WINDOWMANAGER_HPP__
#define __WINDOWMANAGER_HPP__

#include <string>
#include <stdint.h>
#include "KeyManager.hpp"
#include "MouseManager.hpp"
#include "Renderer.hpp"

class WindowManager
{
	public:
		/**
		 * Supported window managers
		 */
		enum WindowManagerType {
			WINDOW_MANAGER_GLFW
		};

		/**
		 * Window Manager factory
		 *
		 * @param	type		Type of window manager to be created
		 *
		 * @return Pointer to a the requested window manager or NULL
		 */
		static WindowManager *GetWindowManager(WindowManagerType type);

		/**
		 * Window Manager disposal
		 *
		 * @param	wmanager	Window manager to be disposed
		 */
		static void DisposeWindowManager(WindowManager *wmanager);

		/**
		 * Gets the current manager
		 *
		 * @return The current manager or NULL if no manager is available
		 */
		static WindowManager *GetCurrentManager();

		/**
		 * Gets the associated key manager
		 *
		 * @return The key manager or NULL if no manager is available
		 */
		virtual KeyManager *getKeyManager() = 0;

		/**
		 * Gets the associated mouse manager
		 *
		 * @return The mouse manager or NULL if no manager is available
		 */
		virtual MouseManager *getMouseManager() = 0;

		/**
		 * Destructor of the class
		 */
		virtual ~WindowManager(void) {}

		/**
		 * Initializes the window manager
		 * @details	Prepares the window manager to be used. After calling
		 *          this method the rest of the methods can be called
		 *
		 * @return	true or false
		 */
		virtual bool init(void) = 0;

		/**
		 * Creates a new window
		 *
		 * @param	name		Title of the window
		 * @param	width		Width of the window
		 * @param	height		Height of the window
		 * @param	fullscreen	Set the window to fullscreen
		 *
		 * @return  true or false
		 */
		virtual bool createWindow(std::string &name, uint16_t width, uint16_t height, bool fullscreen = false) = 0;

		/**
		 * Called when the window must be resized
		 *
		 * @param	width	Width of the window
		 * @param	height	Height of the window
		 *
		 * @return  true or false
		 */
		virtual bool resize(uint16_t width, uint16_t height) = 0;

		/**
		 * Sets a new renderer to handle display requests
		 *
		 * @param	renderer	Renderer that will handle display/reshape requests
		 *
		 * @return	true or false
		 */
		virtual bool setRenderer(Renderer *renderer) = 0;

		/**
		 * Swaps the double buffer
		 */
		virtual void swapBuffers(void) = 0;

		/**
		 * Polls for input events
		 */
		virtual void poll(void) = 0;

	private:
		/**
		 * Current window manager
		 */
		static WindowManager *_windowManager;
};

#endif
