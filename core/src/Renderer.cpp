/**
 * @class	Renderer
 * @brief	Interface for software/hardware renderers
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */

#include "Renderer.hpp"
#include "Logging.hpp"
#include "OpenGLRenderer.hpp"

using namespace Logging;

Renderer *Renderer::_renderer = NULL;

Renderer *Renderer::GetInstance(void)
{
    if (_renderer == NULL) {
        _renderer = new OpenGLRenderer();
    }
    return _renderer;
}

void Renderer::DisposeInstance(void)
{
    delete _renderer;
    _renderer = NULL;
}

bool Renderer::init()
{
    _shaderShadow = NormalShadowMapShader::New();
    if (_shaderShadow->init() == false) {
        log("ERROR initializing shadow map shader\n");
        return false;
    }

    return true;
}

bool Renderer::renderScene(Scene &scene, const Viewport &viewport, bool doBlit)
{
    float avgRadius = 0.0f;
    std::vector<Light *> lightsMarkers;
    DirectLight *sun = NULL;
    std::vector<Model3D *> visibleModels;
    std::vector<PointLight *> visiblePointLights;
    std::vector<SpotLight *> visibleSpotLights;

    if (scene.getActiveCamera() == NULL || scene.getActiveRenderTarget() == NULL) {
        return false;
    }

    scene.getActiveRenderTarget()->clear();

    /* Reserve enough space for the list of visible models */
    visibleModels.reserve(scene.getModels().size());

    /* Force frustum planes calculation */
    scene.getActiveCamera()->recalculateFrustum();

    /* Determine the models visibility */
    for (std::vector<Model3D *>::iterator model = scene.getModels().begin(); model != scene.getModels().end(); ++model) {
        if ((*model)->isEnabled() && scene.getActiveCamera()->isObjectVisible(**model)) {
            visibleModels.push_back(*model);
        }
    }

    /* Determine the lights visibility */
    for (std::vector<PointLight *>::iterator pointLight = scene.getPointLights().begin(); pointLight != scene.getPointLights().end();
         ++pointLight) {
        if ((*pointLight)->isEnabled() && scene.getActiveCamera()->isObjectVisible(**pointLight)) {
            visiblePointLights.push_back(*pointLight);
        }
    }
    for (std::vector<SpotLight *>::iterator spotLight = scene.getSpotLights().begin(); spotLight != scene.getSpotLights().end();
         ++spotLight) {
        if ((*spotLight)->isEnabled() && scene.getActiveCamera()->isObjectVisible(**spotLight)) {
            visibleSpotLights.push_back(*spotLight);
        }
    }

    /* TODO: We only support one direct light for now */
    if (scene.getDirectLights().size() > 0 && scene.getDirectLights()[0]->isEnabled()) {
        sun = scene.getDirectLights()[0];

        /* TODO: lookAt in this case must be fixed to go along the direct light direction */
        sun->lookAt(glm::vec3(0.0f, 0.0f, 0.0f));
        sun->getShadowMap()->clear();

        /* Render the shadow maps for all models */
        for (std::vector<Model3D *>::iterator model = visibleModels.begin(); model != visibleModels.end(); ++model) {
            if ((*model)->isShadowCaster()) {
                renderToShadowMap(**model, *sun, *_shaderShadow);
            }
        }
    }

    /* Render the point lights shadows */
    for (std::vector<PointLight *>::iterator pointLight = visiblePointLights.begin(); pointLight != visiblePointLights.end();
         ++pointLight) {
        /* TODO: lookAt the center of the calculated bounding box, but for now this is enough */
        (*pointLight)->getShadowMap()->clear();
        (*pointLight)->lookAt(glm::vec3(0.0f, 0.0f, 0.0f));

        /* Render the shadow maps for all models */
        for (std::vector<Model3D *>::iterator model = visibleModels.begin(); model != visibleModels.end(); ++model) {
            if ((*model)->isShadowCaster()) {
                renderToShadowMap(**model, **pointLight, *_shaderShadow);
            }
        }

        /* Check if we need to render this light billboard */
        if ((*pointLight)->getRenderMarker() == true || this->getRenderLightsMarkers()) {
            lightsMarkers.push_back(*pointLight);
        }

        /* Check if we want to render the bounding volumes */
        renderBoundingVolumes(**pointLight, *scene.getActiveCamera(), *scene.getActiveRenderTarget(),
                              (*pointLight)->getRenderBoundingSphere() || this->getRenderBoundingSphere(),
                              (*pointLight)->getRenderAABB() || this->getRenderAABB(),
                              (*pointLight)->getRenderOOBB() || this->getRenderOOBB());
    }

    /* Render the spot lights shadows */
    for (std::vector<SpotLight *>::iterator spotLight = visibleSpotLights.begin(); spotLight != visibleSpotLights.end(); ++spotLight) {
        /* TODO: lookAt the center of the calculated bounding box, but for now this is enough */
        (*spotLight)->getShadowMap()->clear();

        /* Render the shadow maps for all models */
        for (std::vector<Model3D *>::iterator model = visibleModels.begin(); model != visibleModels.end(); ++model) {
            if ((*model)->isShadowCaster()) {
                renderToShadowMap(**model, **spotLight, *_shaderShadow);
            }
        }

        /* Check if we need to render this light billboard */
        if ((*spotLight)->getRenderMarker() == true || this->getRenderLightsMarkers()) {
            lightsMarkers.push_back(*spotLight);
        }
    }

    /* Render all objects */
    avgRadius = 0.0f;
    for (std::vector<Model3D *>::iterator model = visibleModels.begin(); model != visibleModels.end(); ++model) {
        if ((*model)->getLightingShader() == NULL) {
            log("ERROR model has no lighting shader associated to it\n");
            continue;
        }

        renderModel3D(**model, *scene.getActiveCamera(), *(*model)->getLightingShader(), sun, visiblePointLights, visibleSpotLights,
                      0.4f, /* TODO: calculate the global ambient light */
                      *scene.getActiveRenderTarget());

        /* Render overlay wireframe if requested */
        if (getWireframeMode() == Renderer::RENDER_WIREFRAME_OVERLAY) {
            renderModel3DWireframe(**model, glm::vec4(1.0f, 0.0f, 1.0f, 1.0f), *scene.getActiveCamera(), *scene.getActiveRenderTarget());
        }

        avgRadius += (*model)->getBoundingSphere().getRadius() / glm::length((*model)->getScaleFactor());
    }

    /* Calculate the average radius */
    avgRadius /= scene.getModels().size();

    /* Render the required debug info for the models */
    for (std::vector<Model3D *>::iterator model = visibleModels.begin(); model != visibleModels.end(); ++model) {
        /* Render normals information */
        if ((*model)->getRenderNormals() == true || this->getRenderNormals()) {
            renderModelNormals(**model, *scene.getActiveCamera(), *scene.getActiveRenderTarget(), avgRadius * 0.02f);
        }
        /* Render bounding volumes information */
        renderBoundingVolumes(**model, *scene.getActiveCamera(), *scene.getActiveRenderTarget(),
                              (*model)->getRenderBoundingSphere() || this->getRenderBoundingSphere(),
                              (*model)->getRenderAABB() || this->getRenderAABB(), (*model)->getRenderOOBB() || this->getRenderOOBB());
    }

    /* Render the required light markers */
    renderLights(lightsMarkers, *scene.getActiveCamera(), *scene.getActiveRenderTarget());

    if (doBlit) {
        scene.getActiveRenderTarget()->blit(viewport.getX(), viewport.getY(), viewport.getWidth(), viewport.getHeight());
    }

    return true;
}
