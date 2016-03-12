/**
 * @class	FlyCamera
 * @brief	Camera object with fly movement. This camera uses only yaw and pitch and
 *          the rotations are always aligned to the world axes, not the camera own axes.
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "Camera.hpp"

class FlyCamera : public Camera
{
	public:
		/**
		 * Gets the view matrix
		 */
		const glm::mat4 &getView(void);
};
