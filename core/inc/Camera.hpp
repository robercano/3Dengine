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
    /**
     * Constructor
     */
    Camera() : _planesValid(false) {}

    /**
     * Checks if the given object is visible from the camera
     *
     * It determines if any part of the object is inside the camera's frustum
     *
     * @param object  Object to check against the camera's frustum
     *
     * @return true if the object is visible, false otherwise
     */
    bool isObjectVisible(Object3D &object);

  private:

    bool _planesValid; /**< Indicate if the frustum planes have been already calculated */
};
