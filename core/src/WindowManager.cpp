/**
 * @class	WindowManager
 * @brief	Interface for window manager management
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */

#include "WindowManager.hpp"
#include "GLFWWindowManager.hpp"

WindowManager *WindowManager::_windowManager = NULL;

WindowManager *WindowManager::GetInstance()
{
    if (_windowManager == NULL) {
        _windowManager = new GLFWWindowManager();
    }
    return _windowManager;
}

void WindowManager::DisposeInstance(void)
{
    delete _windowManager;
    _windowManager = NULL;
}
