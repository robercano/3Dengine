/**
 * @class	Camera
 * @brief	Camera object
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

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

		/**
		 * Sets the position of the camera
		 *
		 * @param position  The new position of the camera
		 */
		void setPosition(glm::vec4 &position);
        void lookAt(glm::vec4 &at);

		/**
		 * Sets the mask to restrict camera movement
		 *
		 * @param mask	The mask applied to restrict the movement
		 *
		 * @see _mask
		 */
		void setMask(glm::mat4 &mask);

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
		 *
		 * @param amount Amount of movement in the indicated direction
		 */
		void forward(float amount);
		void right(float amount);
		void up(float amount);

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
		virtual const glm::mat4 &getView(void) = 0;

		const glm::vec4 getPosition() { return _position; }
		float getPitch() { return _pitch; }
		float getRoll() { return _roll; }
		float getYaw() { return _yaw; }

	protected:

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
		glm::vec4 _position;
        glm::mat4 _orientation;

		/**
		 * Where is the camera looking at
		 */
		glm::vec4 _forward;

		/**
		 * Up vector
		 */
		glm::vec4 _up;

		/**
		 * Perpendicular to the forward/up plane
		 */
		glm::vec4 _right;

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
        glm::quat _qyaw, _qpitch, _qroll;

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

		/**
		 * Accumulators for the camera movement
		 */
		float _forwardAmount, _upAmount, _rightAmount;

		/**
		 * Mask to restrict camera movement. This matrix will be multiplied
		 * by the movement vector before adding it to the current position
		 */
		glm::mat4 _mask;
};
