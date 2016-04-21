/**
 * @class	Camera
 * @brief	Camera object
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include "Object3D.hpp"

class Camera : public Object3D
{
	public:
		/**
		 * Constructor
		 */
		Camera();

		/**
		 * Destructor
		 */
		~Camera();

		void setProjection(float fov, float ratio, float p_near, float p_far)
		{
			_fov   = fov;
			_ratio = ratio;
			_near  = p_near;
			_far   = p_far;

			_projectionValid = false;
		}

		void setFov(float fov)     { _fov   = fov;   _projectionValid = false; }
		void setRatio(float ratio) { _ratio = ratio; _projectionValid = false; }
		void setNear(float p_near)   { _near  = p_near;  _projectionValid = false; }
		void setFar(float p_far)     { _far   = p_far;   _projectionValid = false; }

		const glm::mat4 &getProjectionMatrix(void);

	protected:

		float _fov;
		float _ratio;
		float _near, _far;
		glm::mat4 _projection;
		bool _projectionValid;
};
