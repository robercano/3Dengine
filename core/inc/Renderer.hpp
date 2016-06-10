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
#include "Camera.hpp"
#include "DirectLight.hpp"
#include "LightingShader.hpp"
#include "Model3D.hpp"
#include "NormalShadowMapShader.hpp"
#include "PointLight.hpp"
#include "RenderTarget.hpp"
#include "RendererModel3D.hpp"
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
     * Wraps an model3D with a renderer specific model 3D,
     * peparing all the structures needed to quickly render
     * the model in the renderer
     *
     * The returned model can be deleted through a delete
     * statement. The input Model3D has no linkage with the
     * output RenderModel3D and hence they can have different
     * lif cycles
     *
     * @param model  Object to prepare in the renderer and used to
     *                generate the RendererModel3D
     *
     * @return The prepared RendererModel3D pointer or NULL if an
     *         error happened
     */
    virtual RendererModel3D *prepareModel3D(const Model3D &model) = 0;

    /**
     * Renders the internal list of models to a render target
     *
     * @param projection	Projection matrix
     * @param view			View matrix
     */
    virtual bool renderModel3D(RendererModel3D &model, Camera &camera, LightingShader &shader, DirectLight *sun,
                               std::vector<PointLight *> &pointLights, std::vector<SpotLight *> &spotLights, float ambientK,
                               RenderTarget &renderTarget, bool disableDepth = false) = 0;
    virtual bool renderToShadowMap(RendererModel3D &model3D, Light &camera, NormalShadowMapShader &shader) = 0;
    virtual bool renderLight(Light &light, Camera &camera, RenderTarget &renderTarget) = 0;

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
