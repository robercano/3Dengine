/**
 * @class	Camera
 * @brief	Camera object. It can be used to project objects onto it through the
 *          Projection interface, allowing for perspective and orthogonal projections. It
 *          inherits from Object3D to allow movement of the camera through interpolators
 *          or Movement objects
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include "Object3D.hpp"
#include "Projection.hpp"

class Camera : public Projection, public Object3D
{
  public:
    Camera() {}
    ~Camera() {}
};
