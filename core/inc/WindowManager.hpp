/**
 * @class	WindowManager
 * @brief	Interface for window manager management
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include <stdint.h>
#include <string>
#include "KeyManager.hpp"
#include "MouseManager.hpp"
#include "Renderer.hpp"

class WindowManager
{
  public:
    /**
     * Window Manager factory
     *
     * @return Pointer to a the requested window manager or NULL
     */
    static WindowManager *GetInstance(void);

    /**
     * Window Manager disposal
     *
     * @param	wmanager	Window manager to be disposed
     */
    static void DisposeInstance();

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
	 * If either width or height are 0, then the manager will detect the current
	 * desktop resolution and set that value for width and height
     *
     * @param	name		Title of the window
     * @param	width		Width of the window
     * @param	height		Height of the window
     * @param	fullscreen	Set the window to fullscreen
     *
     * @return  true or false
     */
    virtual bool createWindow(std::string &name, uint16_t width = 0, uint16_t height = 0, bool fullscreen = true) = 0;

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
     * Retrieves the actual size of the window
     *
     * @param width  Output width of the current window
     * @param height Output height of the current window
     *
     * @return true or false
     */
    virtual bool getWindowSize(uint32_t *width, uint32_t *height) = 0;

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
