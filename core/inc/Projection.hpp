/**
 * @class	Projection
 * @brief	Orthogonal or perspective projection for any object. This can be typically
 *          inherited by cameras to add projection functionality to the camera, or to
 *          lights to calculate a shadow map
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include "Object3D.hpp"

class Projection
{
  public:
    Projection() {}
    ~Projection() {}
    /**
     * Configures the projection values. This method is used to configure both perspective
     * and orthogonal projection. For orthogonal the fov parameter can be ommited
     *
     * @param width  Width in world units of the axis aligned frustum. Used to
     *               calculate the aspect ratio in perspective projections
     * @param height Height in world units of the axis aligned frustum. Used to
     *               calculate the aspect ratio in perspective projection
     * @param near   Near plane. Distance in world units of the near plane perpendicular
     *               to the view direction
     * @param far    Far plane. Distance in world units of the far plane perpendicular
     *               to the view direction
     * @param fov    Field of view, in degrees, from the tip of the projection
     *               frustum
     */
    void setProjection(float width, float height, float pNear, float pFar, float fov = 45.0f)
    {
        _fov = fov;
        _width = width;
        _height = height;
        _near = pNear;
        _far = pFar;

        _valid = false;
    }

    /**
     * Setters for the projections fields
     */
    void setFov(float fov)
    {
        _fov = fov;
        _valid = false;
    }
    void setWidth(float width)
    {
        _width = width;
        _valid = false;
    }
    void setHeight(float height)
    {
        _height = height;
        _valid = false;
    }
    void setNear(float pNear)
    {
        _near = pNear;
        _valid = false;
    }
    void setFar(float pFar)
    {
        _far = pFar;
        _valid = false;
    }

    /**
     * Getters for the projections fields
     */
    float getFov() const
    {
        return _fov;
    }
    float getWidth() const
    {
        return _width;
    }
    float getHeight() const
    {
        return _height;
    }
    float getNear() const
    {
        return _near;
    }
    float getFar() const
    {
        return _far;
    }

    /**
     * Retrieves the perspective projection matrix
     *
     * It is important to note that the projection class assumes only one
     * projection type will be used along the life of the projection object,
     * so the _valid attribute is used for both orthogonal and perspective
     * projections. To switch to other projection type the forceRecalculate
     * parameter can be set to true to retrieve the new matrix
     *
     * @param forceRecalculate  Forces recalculation of the perspective matrix
     *                          regardless of the internal cache
     *
     * @return The perspective projection matrix
     */
    const glm::mat4 &getPerspectiveMatrix(bool forceRecalculate = false)
    {
        if (_valid == false || forceRecalculate == true) {
            _perspective = glm::perspective(_fov, _width / _height, _near, _far);
            _valid = true;
        }
        return _perspective;
    }

    /**
     * Retrieves the orthogonal projection matrix
     *
     * It is important to note that the projection class assumes only one
     * projection type will be used along the life of the projection object,
     * so the _valid attribute is used for both orthogonal and perspective
     * projections. To switch to other projection type the forceRecalculate
     * parameter can be set to true to retrieve the new matrix
     *
     * @param forceRecalculate  Forces recalculation of the orthogonal matrix
     *                          regardless of the internal cache
     *
     * @return The perspective orthogonal matrix
     */
    const glm::mat4 &getOrthogonalMatrix(bool forceRecalculate = false)
    {
        if (_valid == false || forceRecalculate == true) {
            _orthogonal = glm::ortho(-_width / 2.0f, _width / 2.0f, -_height / 2.0f, _height / 2.0f, _near, _far);
            _valid = true;
        }
        return _orthogonal;
    }

  protected:
    float _fov;             /**< Field of view angle in degrees */
    float _width, _height;  /**< Width and height in world coordinates of the frustum */
    float _near, _far;      /**< Distance in world units of the near and far frustum planes */
    glm::mat4 _perspective; /**< Cached perspective projection matrix */
    glm::mat4 _orthogonal;  /**< Cached orthogonal projection matrix */
    bool _valid;            /**< Flag to indicate if a cached matrix is available */
};
