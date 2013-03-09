/**
 * @class	Camera
 * @brief	Camera object
 *
 * @author	Roberto Sosa Cano
 */

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Camera.hpp"

Camera::Camera() : _position(0.0, 0.0, -10.0), _lookat(0.0, 0.0, 1.0, 0.0),
				   _fov(45.0), _up(0.0, 1.0, 0.0),
				   _ratio(16.0/9.0), _near(0.1), _far(100.0),
				   _yaw(0.0), _pitch(0.0), _roll(0.0)
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
}

bool Camera::advance(float amount)
{
	_position.x += _lookat.x*amount;
	_position.z += _lookat.z*amount;
}

bool Camera::stroll(float amount)
{
	_position.x += -_lookat.z*amount;
	_position.z +=  _lookat.x*amount;
}

bool Camera::rotatePitch(float angle)
{
	_pitch += angle;
}

bool Camera::rotateYaw(float angle)
{
	_yaw += angle;
}

bool Camera::rotateRoll(float angle)
{
	_roll += angle;
}

glm::mat4 Camera::getProjection(void)
{
	return glm::perspective(_fov, _ratio, _near, _far);
}

glm::mat4 Camera::getView(void)
{
	_lookat = _lookat*glm::rotate(glm::mat4(1.0), _pitch, glm::vec3(1.0, 0.0, 0.0));
	_lookat = _lookat*glm::rotate(glm::mat4(1.0), _yaw, glm::vec3(0.0, 1.0, 0.0));
	_lookat = _lookat*glm::rotate(glm::mat4(1.0), _roll, glm::vec3(0.0, 0.0, 1.0));
	_yaw = _pitch = _roll = 0.0;
	return glm::lookAt(_position, _position + glm::vec3(_lookat), _up);
}
