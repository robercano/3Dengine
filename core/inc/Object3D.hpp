/**
 * @class	Object3D
 * @brief	Meta-class that represents any object in the 3D world that has a
 *          position, an orientation and a scale factor. Both the model
 *          and the view matrix can be obtained from any Object3D instance
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include "BoundingBox.hpp"
#include "BoundingSphere.hpp"
#include "Logging.hpp"

using namespace Logging;

class Object3D
{
  public:
    /**
     * Constructor
     */
    Object3D()
        : _position(0.0f, 0.0f, 0.0f)
        , _orientation(glm::mat4(1.0f))
        , _scale(glm::vec3(1.0))
        , _model(glm::mat4(1.0f))
        , _modelValid(true)
        , _viewValid(true)
        , _oobbValid(false)
        , _boundingVolumesValid(false)
    {
    }

    /**
     * Moves the postition of the 3D object according to the given amount for each axis
     *
     * Forces recalculation of the model and view matrices
     *
     * @param amount  Amount of movement on each axis as a vec3
     */
    void move(const glm::vec3 &amount)
    {
        _position += amount;
        _modelValid = false;
        _viewValid = false;
    }

    /**
     * Rotates the current orientation according to the given rotation as a quaternion
     *
     * Forces recalculation of the model and view matrices
     *
     * @param rotation  Rotation to apply to the current orientation as a mat4 quaternion
     */
    void rotate(const glm::mat4 &rotation)
    {
        _orientation = rotation * _orientation;
        _modelValid = false;
        _viewValid = false;
    }

    /**
     * Multiplies the current scale factor by the given scale factor and sets
     * the result as the new scale factor
     *
     * Forces recalculation of the model and view matrices
     *
     * @param factor  Scale factor to apply to the 3D object current scale factor
     */
    void scale(const glm::vec3 &factor)
    {
        _scale *= factor;
        _modelValid = false;
        _viewValid = false;
    }

    /**
     * Recalculates the view and model matrices so the object's forward vector
     * goes from the object's position to the given 'at' position
     *
     * Forces recalculation of the model and view matrices
     *
     * @param at  Position in space where the forward vector will point to
     */
    void lookAt(const glm::vec3 &at)
    {
        glm::vec3 up(0.0f, 1.0f, 0.0f);

        // TODO: possible problem with lights not rendering shadow map correctly
        // when view vector is aligned with up vector
        _view = glm::lookAt(_position, at, up);
        _model = glm::inverse(_view);

        _modelValid = true;
        _viewValid = true;
        _boundingVolumesValid = false;
    }

    /**
     * Setters
     *
     * All setters force recalculation of the model and view matrices
     */
    void setPosition(const glm::vec3 &position)
    {
        _position = position;
        _modelValid = false;
        _viewValid = false;

    }
    void setOrientation(const glm::mat4 &orientation)
    {
        _orientation = orientation;
        _modelValid = false;
        _viewValid = false;

    }
    void setScaleFactor(const glm::vec3 &factor)
    {
        _scale = factor;
        _modelValid = false;
        _viewValid = false;
    }

    /**
     * Getters
     */
    const glm::vec3 &getPosition() { return _position; }
    const glm::mat4 &getOrientation() { return _orientation; }
    const glm::vec3 &getScaleFactor() { return _scale; }
    /**
     * Returns the current direction of the model, which is the forward vector
     *
     * @return A vec3 with the forward vector
     */
    glm::vec3 getDirection() { return -glm::vec3(glm::row(getViewMatrix(), 2)); }
    /**
     * Returns the current model matrix of the object
     *
     * The matrix is cached and only recalculated if any of the setters or model
     * matrix modifiers are called
     *
     * @return A mat4 with the current model matrix
     */
    const glm::mat4 &getModelMatrix(void)
    {
        if (_modelValid == false) {
            _model = glm::scale(glm::translate(glm::mat4(), _position) * _orientation, _scale);
            _modelValid = true;
            _boundingVolumesValid = false;
        }
        return _model;
    }

    /**
     * Returns the current view matrix of the object
     *
     * The matrix is cached and only recalculated if any of the setters or model
     * matrix modifiers are called
     *
     * @return A mat4 with the current model matrix
     */
    const glm::mat4 &getViewMatrix(void)
    {
        if (_viewValid == false) {
            _view = glm::translate(glm::scale(glm::mat4(), _scale) * _orientation, -_position);
            _viewValid = true;
        }
        return _view;
    }

    BoundingSphere &getBoundingSphere()
    {
        if (_oobbValid == false) {
            _calculateBoundingVolumes();
            _oobbValid = true;
        }
        if (_boundingVolumesValid == false || _modelValid == false) {
            _updateBoundingVolumes();
            _boundingVolumesValid = true;
        }
        return _boundingSphere;
    }

    const BoundingBox &getAABB()
    {
        if (_oobbValid == false) {
            _calculateBoundingVolumes();
            _oobbValid = true;
        }
        if (_boundingVolumesValid == false || _modelValid == false) {
            _updateBoundingVolumes();
            _boundingVolumesValid = true;
        }
        return _aabb;
    }
    const BoundingBox &getOOBB()
    {
        if (_oobbValid == false) {
            _calculateBoundingVolumes();
            _oobbValid = true;
        }
        if (_boundingVolumesValid == false || _modelValid == false) {
            _updateBoundingVolumes();
            _boundingVolumesValid = true;
        }
        return _oobb;
    }

  protected:
    /**
     * Calculates the bounding volumes for a Model3D
     *
     * The bounding volumes include the BoundingSphere, the OOBB and the AABB.
     * This performs a full recalculation of all the volumes assuming no prior
     * information can be reused
     *
     * This method must be overriden in inheriting classes to calculate the
     * proper bounding volumes based on the inheriting class data
     */
    virtual void _calculateBoundingVolumes() { /* empty */}

    /**
     * Updates the bounding volumes of a model. In this case the AABB
     * is used to speed up the calculation of the bounding sphere and the
     * OOBB
     *
     * Fast calculation idea copied from
     *    http://zeuxcg.org/2010/10/17/aabb-from-obb-with-component-wise-abs/
     */
    void _updateBoundingVolumes()
    {
        glm::mat4 model = getModelMatrix();
        glm::mat4 absModel(glm::abs(model[0][0]), glm::abs(model[0][1]), glm::abs(model[0][2]), glm::abs(model[0][3]),
                           glm::abs(model[1][0]), glm::abs(model[1][1]), glm::abs(model[1][2]), glm::abs(model[1][3]),
                           glm::abs(model[2][0]), glm::abs(model[2][1]), glm::abs(model[2][2]), glm::abs(model[2][3]),
                           glm::abs(model[3][0]), glm::abs(model[3][1]), glm::abs(model[3][2]), glm::abs(model[3][3]));

        glm::vec3 center = (_oobb.getMin() + _oobb.getMax()) / 2.0f;
        glm::vec3 extent = (_oobb.getMax() - _oobb.getMin()) / 2.0f;

        glm::vec3 newCenter = glm::vec3(model * glm::vec4(center, 1.0f));
        glm::vec3 newExtent = glm::vec3(absModel * glm::vec4(extent, 0.0f));

        glm::vec3 min = newCenter - newExtent;
        glm::vec3 max = newCenter + newExtent;

        _aabb.setMin(newCenter - newExtent);
        _aabb.setMax(newCenter + newExtent);

        _boundingSphere.setRadius(glm::length(_maxLengthVertex * getScaleFactor()));
    }

    glm::vec3 _position;    /**< Position of the object in world coordinates */
    glm::mat4 _orientation; /**< Orientation of the object as a quaternion */
    glm::vec3 _scale;       /**< Scale factors for each axis XYZ */
    glm::mat4 _model;       /**< Current model matrix of the object */
    glm::mat4 _view;        /**< Current view matrix of the object */
    bool _modelValid;       /**< If true, current model matrix is cached and does not need recalculation */
    bool _viewValid;        /**< If true, current view matrix is cached and does not need recalculation */

    glm::vec3 _maxLengthVertex;     /**< Maximum length among the vertices set, used to calculate _boundingSphere */
    BoundingSphere _boundingSphere; /**< Bounding sphere containing all model's vertices */
    BoundingBox _oobb;              /**< Object-oriented bounding box containing all model's vertices */
    BoundingBox _aabb;              /**< Axis-aligned bounding box containing all model's vertices */
    bool _oobbValid;                /**< Indicates if the cached information for the base OOBB is still valid */
    bool _boundingVolumesValid;     /**< Indicates if the cached information for the OOBB and AABB is still valid */
};
