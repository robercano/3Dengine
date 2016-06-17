/**
 * @class	FlyMotion
 * @brief	Motion for fly movement. This motion uses yaw and pitch and
 *          the rotations are always aligned to the world axes, not the
 *          object own axes.
 *
 *          Typical motion for a 3D modeling software fly camera, where the
 *          user can freely navigate the world but the floor is always down
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "Motion.hpp"

class FlyMotion : public Motion
{
  public:
    /**
     * Constructor
     */
    FlyMotion();

    /**
     * Inherited from Motion class
     */
    void applyTo(Object3D &obj);
    void reset();

    /**
     * Sets the position of the motion
     *
     *  The position will be directly transferred to the given
     *  Object3D when using applyTo()
     *
     *  @param pos  The new position for the motion
     */
    void setPosition(const glm::vec3 &pos);

    /**
     * Moves the position of the motion along the forward vector
     *
     * The forward vector is the (0, 0, -1) vector in local coordinates
     *
     * @param amount  Amount of movement along the forward vector. It can
     *                be positive or negative, in which case it moves backwards
     */
    void forward(float amount);

    /**
     * Moves the position of the motion along the right vector
     *
     * The right vector is the (1, 0, 0) vector in local coordinates
     *
     * @param amount  Amount of movement along the right vector. It can
     *                be positive or negative, in which case it moves left
     */
    void right(float amount);

    /**
     * Rotates the motion along the right vector
     *
     * @see forward
     *
     * @param angle  Angle in degrees for the rotation around the right vector
     */
    void rotatePitch(float angle);

    /**
     * Rotates the motion along the up vector
     *
     * The up vector is the (0, 1, 0) vector in local coordinates
     *
     * @param angle  Angle in degrees for the rotation around the up vector
     */
    void rotateYaw(float angle);

  private:
    glm::vec4 _forward;   /**< Forward vector in local coordinates */
    glm::vec4 _up;        /**< Up vector in local coordinates */
    glm::vec4 _right;     /**< Right vector in local coordinates */
    float _yaw;           /**< Current yaw angle in degrees */
    float _pitch;         /**< Current pitch angle in degrees */
    float _forwardAmount; /**< Current accumulated forward amount. This is reset
                               after reset() or applyTo() are called */
    float _rightAmount;   /**< Current accumulated right amount. This is reset
                               after reset() or applyTo() are called */
    glm::vec3 _position;  /**< Current position after the forward and right amounts
                               are added */
};
