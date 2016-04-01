/**
 * @class	FlyCamera
 * @brief	FlyCamera object
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include "FlyCamera.hpp"

const glm::mat4 &FlyCamera::getView(void)
{
	if (_viewValid == false) {
        glm::quat qyaw = glm::angleAxis(_yaw, glm::vec3(_up));
        glm::quat qpitch = glm::angleAxis(_pitch, glm::vec3(_right));

        glm::mat4 orientation = glm::toMat4(qpitch) * glm::toMat4(qyaw);

        setOrientation(orientation);

		glm::vec4 forward = _forward * _orientation;
		glm::vec4 up      =      _up * _orientation;
		glm::vec4 right   =   _right * _orientation;

        move(_mask*(_forwardAmount*forward + _rightAmount*right + _upAmount*up));

		_forwardAmount = _upAmount = _rightAmount = 0;
	}

	return _view;
}
