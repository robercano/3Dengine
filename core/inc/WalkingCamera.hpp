/**
 * @class	WalkingCamera
 * @brief	Specialization of the camera object for a walking camera
 *			(Y coord is always the same)
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include "Camera.hpp"

class WalkingCamera : public Camera
{
	public:
		/**
		 * Constructor
		 */
		WalkingCamera()
		{
			glm::mat4 mask(1.0, 0.0, 0.0, 0.0,
					       0.0, 0.0, 0.0, 0.0, /* Restrict Y coordinate */
						   0.0, 0.0, 1.0, 0.0,
						   0.0, 0.0, 0.0, 1.0);
			setMask(mask);
		}
};
