/**
 * @class	FreeFlyCamera
 * @brief	Camera object with free fly movement. This camera yaw, pitch and roll are always
 *          aligned to the camera own axis, not the world axis.
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "Camera.hpp"

class FreeFlyCamera : public Camera
{
	public:
		/**
		 * Gets the view matrix
		 */
		const glm::mat4 &getView(void);
};
