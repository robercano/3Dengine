/**
 *                          Light &light,
 * @class	OpenGLRenderer
 * @brief	OpenGL based 3D renderer
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include <vector>
#include "OpenGLShader.hpp"
#include "OpenGLSolidColorShader.hpp"
#include "Renderer.hpp"

class OpenGLRenderer : public Renderer
{
  public:
    /**
     * Renderer methods
     */
    bool init(void);
    const char *getName();
    const char *getVersion();
    const char *getVendor();
    const char *getShaderVersion();
    Shader *newShader(void);
    Asset3D *loadAsset3D(const std::string &assetName);
    bool prepareAsset3D(Asset3D &model);
    bool renderModel3DWireframe(Model3D &model, const glm::vec4 &color, Camera &camera, RenderTarget &renderTarget);
    bool renderModel3D(Model3D &model, Camera &camera, LightingShader &shader, DirectLight *sun, std::vector<PointLight *> &pointLights,
                       std::vector<SpotLight *> &spotLights, float ambientK, RenderTarget &renderTarget, bool disableDepth = false);
    bool renderToShadowMap(Model3D &model3D, Light &light, NormalShadowMapShader &shader);
    bool renderLight(Light &light, Camera &camera, RenderTarget &renderTarget, uint32_t lightNumber);
    bool renderLights(std::vector<Light *> &lights, Camera &camera, RenderTarget &renderTarget);
    bool renderBoundingBox(const BoundingBox &box, const glm::mat4 &modelMatrix, const glm::vec3 &color, Camera &camera,
                           RenderTarget &renderTarget);
    bool renderBoundingSphere(const BoundingSphere &sphere, const glm::vec3 &center, const glm::vec3 &color, Camera &camera,
                              RenderTarget &renderTarget);
    bool renderBoundingVolumes(Object3D &object, Camera &camera, RenderTarget &renderTarget, bool showSphere = true, bool showAABB = true,
                               bool showOOBB = true);
    bool renderModelNormals(Model3D &model3D, Camera &camera, RenderTarget &renderTarget, float normalSize);
    bool resize(uint16_t width, uint16_t height);
    void flush();
    void clear();

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

    /**
     * Texture used when an object does not receive shadow
     */
    unsigned int _noshadowTexture;

    /**
     * Shader to render light billboards
     */
    OpenGLShader _renderLightShader;

    /**
     * Shader to render bounding volumes
     */
    OpenGLShader _renderBoundingBox;

    /**
     * Shader to render bounding spheres
     */
    OpenGLShader _renderBoundingSphere;

    /**
     * Shader to render a solid color, used for wireframe rendering
     */
    OpenGLSolidColorShader *_wireframeShader;

    /**
     * Shader to render model normals
     */
    OpenGLShader _renderNormals;
};
