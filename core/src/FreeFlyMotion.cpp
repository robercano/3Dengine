/**
 * @class	FreeFlyMotion
 * @brief	FreeFlyMotion object
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */

#include "FreeFlyMotion.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include "MathUtils.h"

FreeFlyMotion::FreeFlyMotion()
    : _forward(0.0f, 0.0f, -1.0f, 0.0f)
    , _up(0.0f, 1.0f, 0.0f, 0.0f)
    , _right(1.0f, 0.0f, 0.0f, 0.0f)
    , _yaw(0.0f)
    , _pitch(0.0f)
    , _roll(0.0f)
    , _forwardAmount(0.0f)
    , _rightAmount(0.0f)
    , _upAmount(0.0f)
    , _position(0.0f, 0.0f, 0.0f)
    , _recalculate(false)
{
}

void FreeFlyMotion::applyTo(Object3D &obj)
{
    if (_recalculate) {
        glm::quat qyaw = glm::angleAxis(_yaw, glm::vec3(_up));
        glm::quat qpitch = glm::angleAxis(_pitch, glm::vec3(_right));
        glm::quat qroll = glm::angleAxis(_roll, glm::vec3(_forward));

        glm::mat4 orientation = glm::toMat4(qpitch) * glm::toMat4(qyaw) * glm::toMat4(qroll) * obj.getOrientation();

        glm::vec4 forward = _forward * orientation;
        glm::vec4 right = _right * orientation;
        glm::vec4 up = _up * orientation;

        _position += glm::vec3(_forwardAmount * forward + _rightAmount * right + _upAmount * up);

        obj.setOrientation(orientation);
        obj.setPosition(_position);

        _forwardAmount = _rightAmount = _upAmount = 0.0f;
        _yaw = _pitch = _roll = 0.0;
    }
}

void FreeFlyMotion::reset()
{
    _forwardAmount = 0.0f;
    _rightAmount = 0.0f;
    _upAmount = 0.0f;
    _yaw = 0.0f;
    _pitch = 0.0f;
    _roll = 0.0f;
    _recalculate = true;
}

void FreeFlyMotion::setPosition(const glm::vec3 &pos)
{
    _position = pos;
    _recalculate = true;
}

void FreeFlyMotion::forward(float amount)
{
    _forwardAmount += amount;
    _recalculate = true;
}

void FreeFlyMotion::right(float amount)
{
    _rightAmount += amount;
    _recalculate = true;
}

void FreeFlyMotion::up(float amount)
{
    _upAmount += amount;
    _recalculate = true;
}

void FreeFlyMotion::rotatePitch(float angle)
{
    _pitch = MathUtils::restrictAngle(_pitch + angle);
    _recalculate = true;
}

void FreeFlyMotion::rotateYaw(float angle)
{
    _yaw = MathUtils::restrictAngle(_yaw + angle);
    _recalculate = true;
}

void FreeFlyMotion::rotateRoll(float angle)
{
    _roll = MathUtils::restrictAngle(_roll + angle);
    _recalculate = true;
}
