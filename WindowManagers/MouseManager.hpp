/**
 * @class	MouseManager
 * @brief	Interface for mouse management
 *
 * @author	Roberto Sosa Cano
 */
#ifndef __MOUSEMANAGER_HPP__
#define __MOUSEMANAGER_HPP__

#include <stdint.h>
#include <vector>


/**
 * Interface to be implemented by listeners of the key manager
 */
class MouseListener
{
	public:
		/**
		 * Method called when one of the registered keys is pressed/released
		 *
		 * @param	x	X coordinate of the mouse
		 * @param	y	Y coordinate of the mouse
		 */
		virtual void processMouse(int32_t x, int32_t y) = 0;
};

class MouseManager
{
	public:
		/**
		 * Registers a listener to receive mouse events
		 *
		 * @param	listener	Listener that will process the mouse
		 *
		 * @return true if the listener was correctly registered, false otherwise
		 */
		virtual bool registerListener(MouseListener &listener) = 0;
};

#endif
