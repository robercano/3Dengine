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
		 */
		bool setProjection(float fov, float ratio, float near, float far);

		/**
		 * Moves the camera in the required direction
		 */
		bool advance(float amount);
		bool stroll(float amount);

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
		glm::mat4 getProjection(void);

		/**
		 * Gets the view matrix
		 */
		glm::mat4 getView(void);

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
		glm::vec4 _lookat;

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

		float _yaw, _pitch, _roll;
};

#endif
