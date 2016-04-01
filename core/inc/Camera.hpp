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

        void lookAt(const glm::vec4 &at);
		void setMask(glm::mat4 &mask);
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

		void forward(float amount);
		void right(float amount);
		void up(float amount);

		const glm::mat4 &getProjection(void);

		virtual const glm::mat4 &getView(void) = 0;

	protected:

		glm::vec4 _forward;
		glm::vec4 _up;
		glm::vec4 _right;
		float _fov;
		float _ratio;
		float _near, _far;
		glm::mat4 _projection;
		bool _projectionValid;
		float _forwardAmount, _upAmount, _rightAmount;
		glm::mat4 _mask;
};
