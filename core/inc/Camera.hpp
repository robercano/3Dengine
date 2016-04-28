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

		void setProjection(float fov, float width, float height, float p_near, float p_far)
		{
			_fov    = fov;
			_width  = width;
			_height = height;
			_near   = p_near;
			_far    = p_far;

			_projectionValid = false;
		}
		void setOrthogonal(float width, float height, float p_near, float p_far)
		{
			_width  = width;
			_height = height;
			_near   = p_near;
			_far    = p_far;

			_orthogonalValid = false;
		}

		void setFov(float fov)     { _fov   = fov;   _projectionValid = false; }
		void setWidth(float width) { _width = width; _projectionValid = false; _orthogonalValid = false; }
		void setHeight(float height) { _height = height; _projectionValid = false; _orthogonalValid = false; }
		void setNear(float p_near)   { _near  = p_near; _projectionValid = false; _orthogonalValid = false; }
		void setFar(float p_far)     { _far   = p_far; _projectionValid = false; _orthogonalValid = false; }

		const glm::mat4 &getProjectionMatrix(void);
		const glm::mat4 &getOrthogonalMatrix(void);

	protected:

		float _fov;
		float _width, _height;
		float _near, _far;
		glm::mat4 _projection;
		glm::mat4 _orthogonal;
		bool _projectionValid;
		bool _orthogonalValid;
};
