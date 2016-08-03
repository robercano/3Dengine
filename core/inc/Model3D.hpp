/**
 * @class	Model3D
 * @brief	A Model3D represents a 3D asset in the engine including the geometry,
 *          normals, textures and texture coordinates, and vertices indices for
 *          indexed geometry rendering. This class can be expanded to support any
 *          3D model definition that can be needed in the engine, like displacement
 *          maps. This class represents an static model and does not take into
 *          account animations or tessellation of skin models.
 *
 *          Objects of this class cannot be instantiated directly and cannot only
 *          be obtained through model loaders.
 *
 *          The data of the 3D model is organized as follows:
 *
 *              * List of vertices of the model including position, normal and UV coordinate.
 *                If a vertex belongs to 2 different polygons with different textures or surface
 *                directions, the same vertex position can appear more than once with different
 *                normal and/or different UV coordinate
 *              * List of indices for triangle rendering lists. The list of indices is always
 *                a multiple of 3 and it specifies how to group 3 vertices to form a triangle,
 *                including the face direction (counter clock-wise)
 *              * The following vector define the rendering lists. Each entry of the folowing vectors
 *                define 1 display list of triangles with it's corresponding material and texture. All
 *                the following vectors must have the same length:
 *
 *                  * List of textures
 *                  * List of materials
 *                  * List of offsets into the indices list for the start of the rendering list
 *                  * List with the number of indices that belong to the rendering list starting
 *                    at the indicated offset
 *
 *          The former data allows to render a model3D as a group of rendering lists, with each list
 *          defining a material and a texture, and a list of indices into the raw vertex data. This
 *          minimizes the bandwidth required from the GPU to render the model.
 *
 *          TODO: This class is likely to be refactored when animations are introduced
 *          in the engine
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include <stdint.h>
#include <glm/glm.hpp>
#include <vector>
#include "Object3D.hpp"
#include "Asset3D.hpp"
#include "Material.hpp"
#include "Texture.hpp"
#include "ProceduralUtils.hpp"
#include "LightingShader.hpp"

class Model3D : public Object3D
{
  public:
    /**
     * Friend classes
     */
    friend class ModelLoaders;
    friend class ModelTransform;

    /**
     * Constructor
     */
    Model3D() : _lightingShader(NULL), _renderNormals(false), _isShadowCaster(true)
    {
        _asset = Asset3D::New();
    }
    Model3D(Asset3D *asset) : _asset(asset), _lightingShader(NULL), _renderNormals(false), _isShadowCaster(true) {}

    /**
     * Destructor
     */
    virtual ~Model3D() {}

    /**
     * Retrieves the model asset 3D
     *
     * @return The associated Asset3D
     */
    Asset3D *getAsset3D() { return _asset; }
    const Asset3D *getAsset3D() const { return _asset; }
    operator Asset3D *() { return _asset; }
    operator Asset3D &() { return *_asset; }

    /**
     * Sets the lighting shader used to render this model
     *
     * @param shader  Lighting shader used to render this model
     */
    void setLightingShader(LightingShader *shader) { _lightingShader = shader; }

    /**
     * Gets the lighting shader used to render this model
     *
     * @return Lighting shader used to render this model
     */
    LightingShader *getLightingShader(void) { return _lightingShader; }

    /**
     * Enables/disables this model as a shadow caster
     *
     * If disabled the model is not taken into account when generating the
     * shadow map for a specific light
     *
     * @param flag  true (casts a shadow) or false (does not cast a shadow)
     */
    void setShadowCaster(bool flag) { _isShadowCaster = flag; }

    /**
     * Indicates if this model casts a shadow or not
     *
     * @see setShadowCaster
     *
     * @return true (casts a shadow) or false (does not cast a shadow)
     */
    bool isShadowCaster(void) { return _isShadowCaster; }

    /**
     * Debug information
     */
    void setRenderNormals(bool flag) { _renderNormals = flag; }
    bool getRenderNormals(void) { return _renderNormals; }

  protected:
    /**
     * Methods from Object3D class
     */
    void _calculateBoundingVolumes();

    Asset3D *_asset;            /**< Asset containing the geometry and textures */
    bool _renderNormals;        /**< Enables normal rendering for this model */
    bool _isShadowCaster;       /**< Indicates if this model is a shadow caster */

    LightingShader *_lightingShader; /** Lighting shader used to render this model */
};
