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

		/**
		 * Sets the projection for the camera
		 */
		bool setProjection(float fov, float ratio, float near, float far);

		/**
		 * Sets the position of the camera in world space
		 */
		bool setPosition(float x, float y, float z);

		/**
		 * Sets the looking point of the camera in world space
		 */
		bool setLookAt(float x, float y, float z);

		/**
		 * Moves the camera forward/backward in the direction of the vector
		 * projected by the lookat vector over the XZ plane
		 *
		 * @param amount	Amount of movement
		 *
		 * @details amount can be negative
		 */
		bool forward(float amount);

		/**
		 * Moves the camera left/right in the direction of the perpendicular
		 * to the vector projected by the lookat vector over the XZ plane
		 *
		 * @param amount	Amount of movement
		 *
		 * @details amount can be nagative
		 */
		bool stroll(float amount);

		/**
		 * Moves the camera to the given position
		 */
		bool move(float x, float y, float z);

		/**
		 * Rotates camera along the X axis
		 */
		bool rotatePitch(float angle);

		/**
		 * Rotates camera along the Y axis
		 */
		bool rotateYaw(float angle);

		/**
		 * Rotates camera along the Z axis
		 */
		bool rotateRoll(float angle);

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
		 * Pitch, yaw, and roll of the camera
		 */
		float _pitch, _yaw, _roll;

		/**
		 * Amount of movement along the lookat vector
		 */
		float _forward;

		/**
		 * Amount of movement along the perpendicular to the lookat vector
		 */
		float _stroll;

		/**
		 * Invalidates the projection and view matrices
		 */
		bool _projectionInvalid;
		bool _viewInvalid;

		/**
		 * Projection matrix
		 */
		glm::mat4 _projection;

		/**
		 * View matrix
		 */
		glm::mat4 _view;
};

#endif
