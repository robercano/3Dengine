/**
 * @class	Camera
 * @brief	Camera object
 *
 * @author	Roberto Sosa Cano
 */
#ifndef __CAMERA_HPP__
#define __CAMERA_HPP__

#include <glm/glm.hpp>

class Camera
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

		/*
		 * Sets the projection for the camera
		 *
		 * @param fov	Angle for the field of view in degrees
		 * @param ratio	Aspect ratio of the viewport
		 * @param near	Near plane for the frustum
		 * @param far	Far plane for the frustum
		 */
		void setProjection(float fov, float ratio, float near, float far)
		{
			_fov   = fov;
			_ratio = ratio;
			_near  = near;
			_far   = far;

			_projectionValid = false;
		}

		/**
		 * Accesors to set each field of the projection matrix
		 *
		 * @see setProjection
		 */
		void setFov(float fov)     { _fov   = fov;   _projectionValid = false; }
		void setRatio(float ratio) { _ratio = ratio; _projectionValid = false; }
		void setNear(float near)   { _near  = near;  _projectionValid = false; }
		void setFar(float far)     { _far   = far;   _projectionValid = false; }

		/**
		 * Moves the camera in the required direction
		 */
		void advance(float amount);
		void stroll(float amount);

		/**
		 * Rotates camera along the X axis
		 */
		void rotatePitch(float angle);

		/**
		 * Rotates camera along the Y axis
		 */
		void rotateYaw(float angle);

		/**
		 * Rotates camera along the Z axis
		 */
		void rotateRoll(float angle);

		/**
		 * Gets the projection matrix
		 */
		const glm::mat4 &getProjection(void);

		/**
		 * Gets the view matrix
		 */
		const glm::mat4 &getView(void);

	private:

		/**
		 * Restricts the given angle to the 0..360 range
		 *
		 * @details This function assumes that an increment in angle will
		 *          never be more than +/- 360 degrees, so no modulo
		 *          operation is performed
		 *
		 * @param angle	Angle to be restricted
		 *
		 * @return	The restricted angle
		 */
		float _restrictAngle(float angle)
		{
			if (angle >= 360.0)   angle -= 360.0;
			else if (angle < 0.0) angle += 360.0;
			return angle;
		}

		/**
		 * Position of the camera in world coordinates
		 */
		glm::vec3 _position;

		/**
		 * Up vector
		 */
		glm::vec3 _up;

		/**
		 * Where is the camera looking at
		 */
		glm::vec3 _lookat;

		/**
		 * Field of view
		 */
		float _fov;

		/**
		 * Aspect ratio of this camera
		 */
		float _ratio;

		/**
		 * Near and far plane of the frustum
		 */
		float _near, _far;

		/**
		 * Rotation components for the camera
		 */
		float _yaw, _pitch, _roll;

		/**
		 * Projection matrix for the camera
		 */
		glm::mat4 _projection;

		/**
		 * Flag to control if the projection matrx mas be recalculated
		 */
		bool _projectionValid;

		/**
		 * View matrix for the camera
		 */
		glm::mat4 _view;

		/**
		 * Flag to control if the view matrx mas be recalculated
		 */
		bool _viewValid;

		float _advanceAmount, _strollAmount;


};

#endif
