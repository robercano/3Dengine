/**
 * @class	FreeFlyMotion
 * @brief	Motion for free fly movement. This motion uses yaw, pitch and roll and
 *          the rotations are always aligned to the motion object own axes.
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "Motion.hpp"

class FreeFlyMotion : public Motion
{
	public:
		FreeFlyMotion();

		void applyTo(Object3D &obj);
		void reset();

		void setPosition(const glm::vec3 &pos);
		void forward(float amount);
		void right(float amount);
		void up(float amount);
		void rotatePitch(float angle);
		void rotateYaw(float angle);
		void rotateRoll(float angle);

	private:
		glm::vec4 _forward;
		glm::vec4 _up;
		glm::vec4 _right;
		float _yaw, _pitch, _roll;
		float _forwardAmount, _rightAmount, _upAmount;
		glm::vec3 _position;
		bool _recalculate;
};
