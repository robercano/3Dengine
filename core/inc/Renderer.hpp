/**
 * @class	Renderer
 * @brief	Interface for software/hardware renderers
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "BoundingBox.hpp"
#include "Camera.hpp"
#include "DirectLight.hpp"
#include "LightingShader.hpp"
#include "Model3D.hpp"
#include "NormalShadowMapShader.hpp"
#include "PointLight.hpp"
#include "RenderTarget.hpp"
#include "SpotLight.hpp"

class Renderer
{
  public:
    /**
     * Singleton
     */
    static Renderer *GetInstance(void);
    static void DisposeInstance(void);

    /**
     * Destructor
     */
    virtual ~Renderer() {}
    /**
     * Initializes the renderer
     */
    virtual void init(void) = 0;

    /**
     * Renderer introspection
     */
    virtual const char *getName() = 0;
    virtual const char *getVersion() = 0;
    virtual const char *getVendor() = 0;
    virtual const char *getShaderVersion() = 0;

    /**
     * Loads a model3D resource prepared for the specific renderer API
     *
     * The returned model can be deleted with a delete statement.
     *
     * @param modelName  Model name to be loaded from the assets directory
     *
     * @return The prepared Model3D pointer or NULL if an error happened
     */
    virtual Model3D *loadModelOBJ(const std::string &modelName) = 0;

    /**
     * Loads a model3D resource prepared for the specific renderer API from
     * a simple Model3D set of data. The data is copied and then the specific
     * rendering API structures prepared
     *
     * The returned model can be deleted with a delete statement.
     *
     * @param model  Model already containing the data
     *
     * @return The prepared Model3D pointer or NULL if an error happened
     */
    virtual Model3D *prepareModel(const Model3D &model) = 0;

    /**
     * Renders the internal list of models to a render target
     *
     * @param projection	Projection matrix
     * @param view			View matrix
     */
    virtual bool renderModel3D(Model3D &model, Camera &camera, LightingShader &shader, DirectLight *sun,
                               std::vector<PointLight *> &pointLights, std::vector<SpotLight *> &spotLights, float ambientK,
                               RenderTarget &renderTarget, bool disableDepth = false) = 0;
    virtual bool renderToShadowMap(Model3D &model3D, Light &camera, NormalShadowMapShader &shader) = 0;
    virtual bool renderLight(Light &light, Camera &camera, RenderTarget &renderTarget) = 0;
    virtual bool renderBoundingBox(const BoundingBox &box, const glm::vec3 &color, Camera &camera, RenderTarget &renderTarget) = 0;
    virtual bool renderModelBoundingBoxes(Model3D &model, Camera &camera, RenderTarget &renderTarget) = 0;

    /**
     * Adjusts the renderer's display size
     */
    virtual bool resize(uint16_t width, uint16_t height) = 0;
    virtual void flush() = 0;

  private:
    /**
     * Singleton instance
     */
    static Renderer *_renderer;
};
