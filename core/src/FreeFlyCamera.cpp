/**
 * @class	FreeFlyCamera
 * @brief	FreeFlyCamera object
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include "FreeFlyCamera.hpp"

const glm::mat4 & FreeFlyCamera::getView(void)
{
	if (_viewValid == false) {
        _qyaw = glm::angleAxis(_yaw, glm::vec3(_up));
        _qpitch = glm::angleAxis(_pitch, glm::vec3(_right));
        _qroll = glm::angleAxis(_roll, glm::vec3(_forward));

        _orientation = glm::toMat4(_qpitch) * glm::toMat4(_qyaw) * glm::toMat4(_qroll) * _orientation;

		glm::vec4 forward = _forward * _orientation;
		glm::vec4 up      =      _up * _orientation;
		glm::vec4 right   =   _right * _orientation;

		_position += _mask*(_forwardAmount*forward + _rightAmount*right + _upAmount*up);

        glm::mat4 translation = glm::translate(glm::mat4(), -glm::vec3(_position));
        _view = ( _orientation * translation );
        _viewValid = true;

		_forwardAmount = _upAmount = _rightAmount = 0;
        _yaw = _pitch = _roll = 0.0;
	}

	return _view;
}
