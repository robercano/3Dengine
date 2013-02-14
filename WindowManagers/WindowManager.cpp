/**
 * @class	WindowManager
 * @brief	Interface for window manager management
 *
 * @author	Roberto Sosa Cano
 */

#include "WindowManager.hpp"
#include "GLUTWindowManager.hpp"

WindowManager *WindowManager::NewWindowManager(WindowManagerType type)
{
	GLUTWindowManager *glutWindowManager = NULL;

	switch (type)
	{
		case WINDOW_MANAGER_GLUT:
			/* Take care of creating only one of this, as it is using
			 * static callbacks */
			if (glutWindowManager == NULL) {
				glutWindowManager = new GLUTWindowManager();
			}
			return glutWindowManager;
		default:
			return NULL;
	}
}

void WindowManager::DeleteWindowManager(WindowManager *wmanager)
{
	delete wmanager;
}
