/**
 * @class	InputManager
 * @brief	Manager to handle different inputs
 *
 * @todo	The first version is just a basic keyboard and mouse handler
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#include <string.h>
#include "InputManager.hpp"

InputManager::InputManager()
{
	memset(_keys, false, sizeof _keys);
}

void InputManager::processKey(uint32_t key, bool state)
{
	_keys[key] = state;
}

void InputManager::processMouse(int32_t x, int32_t y)
{
	_xMouse = x;
	_yMouse = y;
}
