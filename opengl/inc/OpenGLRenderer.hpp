/**
 *                          Light &light,
 * @class	OpenGLRenderer
 * @brief	OpenGL based 3D renderer
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include <vector>
#include "Renderer.hpp"

class OpenGLRenderer : public Renderer
{
  public:
    /**
     * Renderer methods
     */
    void init(void);
    const char *getName();
    const char *getVersion();
    const char *getVendor();
    const char *getShaderVersion();
    Shader *newShader(void);
    Model3D *loadModelOBJ(const std::string &modelName);
    Model3D *prepareModel(const Model3D &model);
    bool renderModel3D(Model3D &model, Camera &camera, LightingShader &shader, DirectLight *sun, std::vector<PointLight *> &pointLights,
                       std::vector<SpotLight *> &spotLights, float ambientK, RenderTarget &renderTarget, bool disableDepth = false);
    bool renderToShadowMap(Model3D &model3D, Light &light, NormalShadowMapShader &shader);
    bool renderLight(Light &light, Camera &camera, RenderTarget &renderTarget);
    bool renderBoundingBox(const BoundingBox &box, const glm::mat4 &modelMatrix, const glm::vec3 &color, Camera &camera, RenderTarget &renderTarget);
    bool renderModelBoundingBoxes(Model3D &model, Camera &camera, RenderTarget &renderTarget, bool showSphere = true, bool showAABB = true, bool showOOBB = true);
    bool renderModelNormals(Model3D &model3D, Camera &camera, RenderTarget &renderTarget);
    bool resize(uint16_t width, uint16_t height);
    void flush();

  private:
    /**
     * Width of the display
     */
    uint16_t _width;

    /**
     * Height of the display
     */
    uint16_t _height;

    /**
     * Dummy texture for some GLSL 3.30 workaround
     */
    unsigned int _dummyTexture;
};
