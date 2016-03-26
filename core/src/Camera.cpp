/**
 * @class	Camera
 * @brief	Camera object
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include "Camera.hpp"

Camera::Camera() : _position(0.0f, 0.0f, 10.0f, 1.0f),
				   _forward(0.0f, 0.0f, -1.0f, 0.0f), _up(0.0f, 1.0f, 0.0f, 0.0f), _right(1.0f, 0.0f, 0.0f, 0.0f),
				   _fov(45.0f), _ratio(16.0f/9.0f), _near(0.1f), _far(100.0f),
				   _yaw(0.0f), _pitch(0.0f), _roll(0.0f),
				   _forwardAmount(0.0f), _upAmount(0.0f), _rightAmount(0.0f),
				   _mask(1.0f),
				   _projectionValid(false), _viewValid(false)
{
}

Camera::~Camera()
{
}

void Camera::setPosition(const glm::vec4 &position)
{
	_position = position;
}

void Camera::lookAt(const glm::vec4 &at)
{
}

void Camera::setMask(glm::mat4 &mask)
{
	_mask = mask;
}

void Camera::forward(float amount)
{
	_forwardAmount += amount;
	_viewValid = false;
}

void Camera::up(float amount)
{
	_upAmount += amount;
	_viewValid = false;
}

void Camera::right(float amount)
{
	_rightAmount += amount;
	_viewValid = false;
}

void Camera::rotatePitch(float angle)
{
	_pitch = _restrictAngle(_pitch + angle);

	_viewValid = false;
}

void Camera::rotateYaw(float angle)
{
	_yaw = _restrictAngle(_yaw + angle);

	_viewValid = false;
}

void Camera::rotateRoll(float angle)
{
	_roll = _restrictAngle(_roll + angle);

	_viewValid = false;
}

const glm::mat4 & Camera::getProjection(void)
{
	if (_projectionValid == false) {
		_projection = glm::perspective(_fov, _ratio, _near, _far);
		_projectionValid = true;
	}
	return _projection;
}
