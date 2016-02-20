/**
 * @class	Camera
 * @brief	Camera object
 *
 * @author	Roberto Sosa Cano
 */

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Camera.hpp"

Camera::Camera() : _position(0.0, 0.0, 10.0, 1.0),
				   _forward(0.0, 0.0, -1.0, 0.0), _up(0.0, 1.0, 0.0, 0.0), _right(1.0, 0.0, 0.0, 0.0),
				   _fov(45.0), _ratio(16.0/9.0), _near(0.1), _far(100.0),
				   _yaw(0.0), _pitch(0.0), _roll(0.0),
				   _forwardAmount(0.0), _upAmount(0.0), _rightAmount(0.0),
				   _mask(1.0),
				   _projectionValid(false), _viewValid(false)
{
}

Camera::~Camera()
{
}

void Camera::setPosition(glm::vec4 &position)
{
	_position = position;
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
	_pitch += angle;
	_restrictAngle(_pitch);

	_viewValid = false;
}

void Camera::rotateYaw(float angle)
{
	_yaw += angle;
	_restrictAngle(_yaw);

	_viewValid = false;
}

void Camera::rotateRoll(float angle)
{
	_roll += angle;
	_restrictAngle(_roll);

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

const glm::mat4 & Camera::getView(void)
{
	if (_viewValid == false) {
		glm::mat4 rotation = glm::rotate(glm::mat4(1.0), _pitch, glm::vec3(_right)) *
			                 glm::rotate(glm::mat4(1.0),   _yaw, glm::vec3(_up)) *
							 glm::rotate(glm::mat4(1.0),  _roll, glm::vec3(_forward));

		glm::vec4 forward = _forward * rotation;
		glm::vec4 up      =      _up * rotation;
		glm::vec4 right   =   _right * rotation;

		_position += _mask*(_forwardAmount*forward + _rightAmount*right + _upAmount*up);

		_forwardAmount = _upAmount = _rightAmount = 0;

		_view = glm::lookAt(glm::vec3(_position), glm::vec3(_position + forward), glm::vec3(_up));
		_viewValid = true;
	}

	return _view;
}
