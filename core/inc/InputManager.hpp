/**
 * @class	InputManager
 * @brief	Manager to handle different inputs
 *
 * @todo	The first version is just a basic keyboard and mouse handler
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include "KeyManager.hpp"
#include "MouseManager.hpp"

class InputManager : public KeyListener, public MouseListener
{
	public:
		/**
		 * Constructor
		 */
		InputManager(void);

		/**
		 * Method called when one of the registered keys is pressed/released
		 *
		 * @param	key		ID of the processed key
		 * @param	state	State of the key, pressed or released
		 */
		void processKey(uint32_t key, bool state);

		/**
		 * Method called when one of the registered keys is pressed/released
		 *
		 * @param	x	X coordinate of the mouse
		 * @param	y	Y coordinate of the mouse
		 */
		void processMouse(int32_t x, int32_t y);

		/**
		 * Array with the state for all the keys
		 */
		bool _keys[1024];

		/**
		 * Last position read from the mouse
		 */
		int32_t _xMouse, _yMouse;
};
