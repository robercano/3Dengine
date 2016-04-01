/**
 * @class	Object3D
 * @brief	Object3D object
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include "Camera.hpp"

Object3D::Object3D() : _position(0.0f, 0.0f, 10.0f, 1.0f),
				       _forward(0.0f, 0.0f, -1.0f, 0.0f), _up(0.0f, 1.0f, 0.0f, 0.0f), _right(1.0f, 0.0f, 0.0f, 0.0f),
				       _yaw(0.0f), _pitch(0.0f), _roll(0.0f),
				       _modelValid(false)
{
}

Object3D::~Object3D()
{
}

const glm::mat4 &Object3D::getModel(void)
{
	if (_modelValid == false) {
        _qyaw = glm::angleAxis(_yaw, glm::vec3(_up));
        _qpitch = glm::angleAxis(_pitch, glm::vec3(_right));
        _qroll = glm::angleAxis(_roll, glm::vec3(_forward));

        _orientation = glm::toMat4(_qpitch) * glm::toMat4(_qyaw) * glm::toMat4(_qroll) * _orientation;

        glm::mat4 translation = glm::translate(glm::mat4(), -glm::vec3(_position));
        _model = ( _orientation * translation );
        _modelValid = true;

        _yaw = _pitch = _roll = 0.0;
	}

	return _model;
}
