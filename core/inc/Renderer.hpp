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
#include "Scene.hpp"
#include "NormalShadowMapShader.hpp"
#include "Viewport.hpp"
#include "Asset3D.hpp"

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
    virtual bool init(void);

    /**
     * Renderer introspection
     */
    virtual const char *getName() = 0;
    virtual const char *getVersion() = 0;
    virtual const char *getVendor() = 0;
    virtual const char *getShaderVersion() = 0;

    /**
     * Loads an asset3D resource prepared for the specific renderer API
     *
     * The returned asset can be deleted with a delete statement.
     *
     * @param assetName  Asset name to be loaded from the assets directory
     *
     * @return The prepared Asset3D pointer or NULL if an error happened
     */
    virtual Asset3D *loadAsset3D(const std::string &assetName) = 0;

    /**
     * Prepares the given Asset3D to be rendered by the underlaying
     * rendering API
     *
     * @param asset Asset already containing the data. It will be modified to
     *              containg the required data for the rendering API
     *
     * @return True if the preparation was succesful, false otherwise
     */
    virtual bool prepareAsset3D(Asset3D &model) = 0;

    /**
     * Renders a whole scene
     *
     * This method uses the input scene to render the final frame. It performs
     * some geometry occlusion calculations, and determines the best parameters
     * for light rendering using the camera frustum and the models bounding boxes
     *
     * @see Scene
     *
     * @param scene     The scene to be rendered
     * @param viewport  Viewport for the current window. If not passed the whole window
     *                  is used
     *
     * @return true or false
     */
    bool renderScene(Scene &scene, const Viewport &viewport);

    /**
     * Renders a model 3D from the given camera using the provided lighting shader and the
     * different types of light into the given renderTarget
     *
     * @param model         Model to be rendered
     * @param camera        Camera to use for the rendering
     * @param shader        Lighting shader to apply to the model
     * @param sun           Direct light to apply to the model
     * @param pointLights   Vector of point lights to use for the rendering
     * @param spotLights    Vector of spot lights to use for the rendering
     * @param ambientK      Precalculated ambient factor to use for the rendering. This is
     *                      typically calculated from the scene definition
     * @param renderTarget  Render target for rendering the frame
     * @param disableDepth  Disables the depth test
     *
     * @return true or false
     */
    virtual bool renderModel3D(Model3D &model, Camera &camera, LightingShader &shader, DirectLight *sun,
                               std::vector<PointLight *> &pointLights, std::vector<SpotLight *> &spotLights, float ambientK,
                               RenderTarget &renderTarget, bool disableDepth = false) = 0;

    /**
     * Renders the shadow map of the model using the given light and the given shader. The shadow
     * map is stored in the 'light' parameter to be used later on for shadow rendering
     *
     * @param model  Model to be rendered as a shadow map
     * @param light  Light to use for the shadow map rendering
     * @param shader Shadow map shader to use for the rendering
     *
     * @return true or false
     */
    virtual bool renderToShadowMap(Model3D &model3D, Light &light, NormalShadowMapShader &shader) = 0;

    /**
     * Adjusts the renderer's display size
     */
    virtual bool resize(uint16_t width, uint16_t height) = 0;

    /**
     * Flushes all the renderers pending commands
     */
    virtual void flush() = 0;

    /**
     * Clears the main framebuffer
     */
    virtual void clear() = 0;

    /**---------------------------
     * Debug rendering methods
     *----------------------------*/

    /**
     * Renders the position of the light as a billboard with the ambient component
     * of the light. Used for debugging purposes
     *
     * Mainly used from renderLights as a helper function
     *
     * @param light         Light to be rendered as a billboard
     * @param camera        Camera to use for the rendering
     * @param renderTarget  Render target for rendering the billboard
     * @param lightNumber   Number of light to offset the billboard rendering, this helps
     *                      that the billboards are not rendered at the same depth
     *
     * @return true or false
     */
    virtual bool renderLight(Light &light, Camera &camera, RenderTarget &renderTarget, uint32_t lightNumber) = 0;

    /**
     * Renders the position of the light in the input vector as a billboard with the ambient component
     * of the light. Used for debugging purposes
     *
     * @param lights        Vector of lights to be rendered as a billboard
     * @param camera        Camera to use for the rendering
     * @param renderTarget  Render target for rendering the billboard
     *
     * @return true or false
     */
    virtual bool renderLights(std::vector<Light*> &lights, Camera &camera, RenderTarget &renderTarget) = 0;

    /**
     * Renders the bounding box with the given color
     *
     * Helper method for renderModelBoundingBoxes
     *
     * @param box           Bounding box to be rendered
     * @param modelMatrix   Model matrix used to bring the bounding box to world coordinates. Typically
     *                      the matrix comes from the same Model3D than the BoundingBox
     * @param color         Color to use for the bounding box rendering
     * @param camera        Camera to use for the bounding box rendering
     * @param renderTarget  Render target for rendering the bounding box
     *
     * @return true or false
     */
    virtual bool renderBoundingBox(const BoundingBox &box, const glm::mat4 &modelMatrix, const glm::vec3 &color, Camera &camera, RenderTarget &renderTarget) = 0;
    /**
     *
     * Renders the bounding sphere with the given color
     *
     * Helper method for renderModelBoundingBoxes
     *
     * @param sphere        Bounding sphere to be rendered
     * @param center        World coordinates position for the center of the sphere. Typically the
     *                      position of the Model3D that contained the bounding sphere
     * @param color         Color to use for the bounding box
     * @param camera        Camera to use for the bounding box rendering
     * @param renderTarget  Render target for rendering the bounding box
     *
     * @return true or false
     */
    virtual bool renderBoundingSphere(const BoundingSphere &sphere, const glm::vec3 &center,
                                      const glm::vec3 &color, Camera &camera, RenderTarget &renderTarget) = 0;

    /**
     * Renders a model 3D bounding volumes (AABB, OOBB and Bounding Sphere)
     *
     * @param model         Model containing the bounding boxes to be rendered
     * @param camera        Camera to use for the rendering
     * @param renderTarget  Render target for rendering the bounding volumes
     * @param showSphere    Indicates whether to render the bounding sphere or not
     * @param showAABB      Indicates whether to render the AABB or not
     * @param showOOBB      Indicates whether to render the OOBB or not
     *
     * @return true or false
     */
    virtual bool renderModelBoundingVolumes(Model3D &model, Camera &camera, RenderTarget &renderTarget, bool showSphere = true, bool showAABB = true, bool showOOBB = true) = 0;

    /**
     * Renders a model 3D normals
     *
     * @param model         Model containing the normals to be rendered
     * @param camera        Camera to use for the rendering
     * @param renderTarget  Render target for rendering the normals
     * @param normalSize    Length of the normal in world units
     *
     * @return true or false
     */
    virtual bool renderModelNormals(Model3D &model3D, Camera &camera, RenderTarget &renderTarget, float normalSize) = 0;

    /**
     * Global settings for the renderer
     */
    void setWireframeMode(bool flag) { _wireframe = flag; }
    bool getWireframeMode() { return _wireframe; }
    void setRenderNormals(bool flag) { _renderNormals = flag; }
    bool getRenderNormals() { return _renderNormals; }
    void setRenderBoundingVolumes(bool flag)
    {
        _renderBoundingSphere = flag;
        _renderAABB = flag;
        _renderOOBB = flag;
    }
    void setRenderBoundingSphere(bool flag) { _renderBoundingSphere = flag; }
    void setRenderAABB(bool flag) { _renderAABB = flag; }
    void setRenderOOBB(bool flag) { _renderOOBB = flag; }

    bool getRenderBoundingSphere() { return _renderBoundingSphere; }
    bool getRenderAABB() { return _renderAABB; }
    bool getRenderOOBB() { return _renderOOBB; }
    void setRenderLightsMarkers(bool flag) { _renderLightsMarkers = flag; }
    bool getRenderLightsMarkers() { return _renderLightsMarkers; }

  protected:
    /**
     * Constructor
     */
    Renderer() : _wireframe(false),_renderNormals(false),
                 _renderBoundingSphere(false),_renderAABB(false),_renderOOBB(false),
                 _renderLightsMarkers(false), _shaderShadow(NULL) {}

  private:

    static Renderer *_renderer;   /**< Singleton instance */
    bool _wireframe;              /**< Enables/disables wireframe rendering */
    bool _renderNormals;          /**< Global flag to enable model normals rendering */
    bool _renderBoundingSphere;   /**< Global flag to enable model bounding sphere rendering */
    bool _renderAABB;             /**< Global flag to enable model AABB rendering */
    bool _renderOOBB;             /**< Global flag to enable model OOBB rendering */
    bool _renderLightsMarkers;    /**< Global flag to enable lights markers rendering */
    NormalShadowMapShader *_shaderShadow; /**< Preloaded shader to render shadow maps */
};
