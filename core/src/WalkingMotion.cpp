/**
 * @class	WalkingMotion
 * @brief	WalkingMotion object
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */

#include "WalkingMotion.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include "MathUtils.hpp"

using namespace MathUtils;

WalkingMotion::WalkingMotion()
    : _forward(0.0f, 0.0f, -1.0f, 0.0f)
    , _up(0.0f, 1.0f, 0.0f, 0.0f)
    , _right(1.0f, 0.0f, 0.0f, 0.0f)
    , _yaw(0.0f)
    , _pitch(0.0f)
    , _forwardAmount(0.0f)
    , _rightAmount(0.0f)
    , _position(0.0f, 0.0f, 0.0f)
{
}

void WalkingMotion::applyTo(Object3D &obj)
{
    glm::quat qyaw = glm::angleAxis(_yaw, glm::vec3(_up));
    glm::quat qpitch = glm::angleAxis(_pitch, glm::vec3(_right));

    glm::mat4 orientation = glm::toMat4(qpitch) * glm::toMat4(qyaw);

    glm::vec4 forward = _forward * orientation;
    glm::vec4 right = _right * orientation;

    forward.y = 0.0f;
    right.y = 0.0f;

    _position += glm::vec3(_forwardAmount * forward + _rightAmount * right);

    obj.setOrientation(orientation);
    obj.setPosition(_position);

    _forwardAmount = _rightAmount = 0.0f;
}

void WalkingMotion::reset()
{
    _forwardAmount = 0.0;
    _rightAmount = 0.0;
    _yaw = 0.0;
    _pitch = 0.0;
}

void WalkingMotion::setPosition(const glm::vec3 &pos) { _position = pos; }
void WalkingMotion::forward(float amount) { _forwardAmount += amount; }
void WalkingMotion::right(float amount) { _rightAmount += amount; }
void WalkingMotion::rotatePitch(float angle) { _pitch = MathUtils::restrictAngle(_pitch + angle); }
void WalkingMotion::rotateYaw(float angle) { _yaw = MathUtils::restrictAngle(_yaw + angle); }
