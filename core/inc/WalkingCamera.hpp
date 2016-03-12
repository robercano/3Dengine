/**
 * @class	WalkingCamera
 * @brief	Camera object with waking movement. This camera only uses yaw and pitch, and
 *          doesn't move on the y axis
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "Camera.hpp"

class WalkingCamera : public Camera
{
	public:
		/**
		 * Gets the view matrix
		 */
		const glm::mat4 &getView(void);
};
