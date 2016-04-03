/**
 * @class	FlyMotion
 * @brief	FlyMotion object
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include "FlyMotion.hpp"
#include "MathUtils.h"

FlyMotion::FlyMotion() : _forward(0.0f, 0.0f, -1.0f, 0.0f), _up(0.0f, 1.0f, 0.0f, 0.0f), _right(1.0f, 0.0f, 0.0f, 0.0f),
				         _yaw(0.0f), _pitch(0.0f),
				         _forwardAmount(0.0f), _rightAmount(0.0f),
						 _position(0.0f, 0.0f, 0.0f)
{
}

void FlyMotion::applyTo(Object3D &obj)
{
	glm::quat qyaw = glm::angleAxis(_yaw, glm::vec3(_up));
	glm::quat qpitch = glm::angleAxis(_pitch, glm::vec3(_right));

	glm::mat4 orientation = glm::toMat4(qpitch) * glm::toMat4(qyaw);

	glm::vec4 forward = _forward * orientation;
	glm::vec4 right   =   _right * orientation;

	_position += glm::vec3(_forwardAmount*forward + _rightAmount*right);

	obj.setOrientation(orientation);
	obj.setPosition(_position);

	_forwardAmount = _rightAmount = 0.0f;
}

void FlyMotion::reset()
{
	_forwardAmount = 0.0;
	_rightAmount = 0.0;
	_yaw = 0.0;
	_pitch = 0.0;
}

void FlyMotion::setPosition(const glm::vec3 &pos)
{
	_position = pos;
}

void FlyMotion::forward(float amount)
{
	_forwardAmount += amount;
}

void FlyMotion::right(float amount)
{
	_rightAmount += amount;
}

void FlyMotion::rotatePitch(float angle)
{
	_pitch = restrictAngle(_pitch + angle);
}

void FlyMotion::rotateYaw(float angle)
{
	_yaw = restrictAngle(_yaw + angle);
}
