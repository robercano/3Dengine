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

Camera::Camera() : _fov(45.0f), _width(16.0f), _height(9.0f), _near(0.1f), _far(100.0f),
				   _projectionValid(false), _orthogonalValid(false)
{
}

Camera::~Camera()
{
}

const glm::mat4 & Camera::getProjectionMatrix(void)
{
	if (_projectionValid == false) {
		_projection = glm::perspective(_fov, _width/_height, _near, _far);
		_projectionValid = true;
	}
	return _projection;
}
const glm::mat4 & Camera::getOrthogonalMatrix(void)
{
	if (_orthogonalValid == false) {
		_orthogonal = glm::ortho(-_width/2.0f, _width/2.0f, -_height/2.0f, _height/2.0f, _near, _far);
		_orthogonalValid = true;
	}
	return _orthogonal;
}
