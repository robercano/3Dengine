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

Camera::Camera() : _fov(45.0f), _ratio(16.0f/9.0f), _near(0.1f), _far(100.0f),
				   _projectionValid(false)
{
}

Camera::~Camera()
{
}

void Camera::lookAt(const glm::vec4 &at)
{
}

const glm::mat4 & Camera::getProjectionMatrix(void)
{
	if (_projectionValid == false) {
		_projection = glm::perspective(_fov, _ratio, _near, _far);
		_projectionValid = true;
	}
	return _projection;
}
