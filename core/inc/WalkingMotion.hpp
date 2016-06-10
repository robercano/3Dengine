/**
 * @class	WalkingMotion
 * @brief	Motion for walking movement. This motion uses yaw and pitch and
 *          the rotations are always aligned to the world axes, not the
 *          object own axes. The y position of the motion is constant
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "Motion.hpp"

class WalkingMotion : public Motion
{
  public:
    WalkingMotion();

    void applyTo(Object3D &obj);
    void reset();

    void setPosition(const glm::vec3 &pos);
    void forward(float amount);
    void right(float amount);
    void rotatePitch(float angle);
    void rotateYaw(float angle);

  private:
    glm::vec4 _forward;
    glm::vec4 _up;
    glm::vec4 _right;
    float _yaw, _pitch;
    float _forwardAmount, _rightAmount;
    glm::vec3 _position;
};
