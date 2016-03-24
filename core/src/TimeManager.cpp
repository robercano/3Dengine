/**
 * @class	TimeManager
 * @brief	Interface for time manager management
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */

#include "TimeManager.hpp"
#include "GLFWTimeManager.hpp"

TimeManager *TimeManager::_timeManager = NULL;

TimeManager *TimeManager::GetInstance()
{
    if (_timeManager == NULL) {
        _timeManager = new GLFWTimeManager();
    }
    return _timeManager;
}

void TimeManager::DisposeInstance(void)
{
    delete _timeManager;
    _timeManager = NULL;
}
