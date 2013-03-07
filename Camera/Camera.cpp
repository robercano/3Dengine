/**
 * @class	Camera
 * @brief	Camera object
 *
 * @author	Roberto Sosa Cano
 */

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Camera.hpp"

Camera::Camera() : _position(0, 0, 0), _lookat(0, 0, 1), _fov(45.0), _up(0.0, 1.0, 0.0),
				   _ratio(16.0/9.0), _near(0.1), _far(100.0),
				   _pitch(0.0), _yaw(0.0), _roll(0.0),
				   _forward(0.0), _projectionInvalid(true), _viewInvalid(true)
{
}

Camera::~Camera()
{
}

bool Camera::setProjection(float fov, float ratio, float near, float far)
{
	_fov   = fov;
	_ratio = ratio;
	_near  = near;
	_far   = far;

	_projectionInvalid = true;
}

bool Camera::setPosition(float x, float y, float z)
{
	_position.x = x;
	_position.y = y;
	_position.z = z;

	_viewInvalid = true;
}

bool Camera::setLookAt(float x, float y, float z)
{
	_lookat.x = x;
	_lookat.y = y;
	_lookat.z = z;

	_viewInvalid = true;
}

bool Camera::forward(float amount)
{
	_forward += amount;
	_viewInvalid = true;
}

bool Camera::stroll(float amount)
{
	_stroll += amount;
	_viewInvalid = true;
}

bool Camera::move(float x, float y, float z)
{
	_position.x += x;
	_position.y += y;
	_position.z += z;

	_viewInvalid = true;
}

bool Camera::rotatePitch(float angle)
{
	_pitch += angle;
	_viewInvalid = true;
}

bool Camera::rotateYaw(float angle)
{
	_yaw += angle;
	_viewInvalid = true;
}

bool Camera::rotateRoll(float angle)
{
	_roll += angle;
	_viewInvalid = true;
}

const glm::mat4 &Camera::getProjection(void)
{
	if (_projectionInvalid) {
		_projection = glm::perspective(_fov, _ratio, _near, _far);
		_projectionInvalid = false;
	}
	return _projection;
}

const glm::mat4 &Camera::getView(void)
{
	if (_viewInvalid) {
		/* Calculate the next position */
		glm::vec3 direction = glm::normalize(_lookat - _position);
		_position.x += direction.x*_forward;
		_position.z += direction.z*_forward;

		_position.x += -direction.z*_stroll;
		_position.z += direction.x*_stroll;

		_forward = 0.0;
		_stroll  = 0.0;

		_view =	glm::lookAt(_position, _lookat, _up);

		glm::mat4 rotation(1.0); /* Identity */
		rotation = glm::rotate(rotation,   _yaw, glm::vec3(0.0, 1.0, 0.0));
		rotation = glm::rotate(rotation, _pitch, glm::vec3(1.0, 0.0, 0.0));
		//_view = glm::rotate(_view, _roll,  glm::vec3(0.0, 0.0, 1.0));
		_view = rotation * _view;

		// TODO: rotate Up vector here!
		_viewInvalid = false;
	}
	return _view;
}
