/**
 * @class	Camera
 * @brief	Camera object
 *
 * @author	Roberto Sosa Cano
 */

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Camera.hpp"

Camera::Camera() : _position(0.0, 0.0, -10.0), _lookat(0.0, 0.0, 1.0),
				   _fov(45.0), _up(0.0, 1.0, 0.0),
				   _ratio(16.0/9.0), _near(0.1), _far(100.0),
				   _yaw(0.0), _pitch(0.0), _roll(0.0),
				   _projectionValid(false), _viewValid(false)
{
}

Camera::~Camera()
{
}

/*bool Camera::lookAt(glm::vec3 &at)
{
	_lookat = at;
	_lookat = glm::normalize(_lookat);
}
*/
void Camera::advance(float amount)
{
	//_position.x += _lookat.x*amount;
	//_position.z += _lookat.z*amount;

	_advanceAmount += amount;
	_viewValid = false;
}

void Camera::stroll(float amount)
{
	//_position.x += -_lookat.z*amount;
	//_position.z +=  _lookat.x*amount;

	_strollAmount += amount;
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
		glm::vec4 lookat(_lookat, 0.0);
		lookat = lookat*glm::rotate(glm::mat4(1.0), _pitch, glm::vec3(1.0, 0.0, 0.0));
		lookat = lookat*glm::rotate(glm::mat4(1.0), _yaw, glm::vec3(0.0, 1.0, 0.0));
		lookat = lookat*glm::rotate(glm::mat4(1.0), _roll, glm::vec3(0.0, 0.0, 1.0));

		_position.x += _advanceAmount*lookat.x;
		_position.z += _advanceAmount*lookat.z;

		_position.x += -_strollAmount*lookat.z;
		_position.z += _strollAmount*lookat.x;

		_strollAmount = _advanceAmount = 0;
		_view = glm::lookAt(_position, _position + glm::vec3(lookat), _up);
		_viewValid = true;
	}

	return _view;
}
