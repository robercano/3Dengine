/**
 * @class	GLFWTimeManager
 * @brief	GLFW based time manager
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 * @todo	Standarize the interface to make a generic Time Manager
 */
#pragma once

#include <stdint.h>
#include "OpenGL.h"
#include "TimeManager.hpp"

class GLFWTimeManager : public TimeManager
{
  public:
    /**
     * Destructor of the class
     */
    ~GLFWTimeManager(void) {}
    /**
     * Retrieves the elapsed milliseconds
     */
    double getElapsedMs() { return glfwGetTime() * 1000.0; }
};
