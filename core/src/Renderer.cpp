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

bool Renderer::renderScene(Scene &scene)
{
    std::vector<Light *> lightsMarkers;
    DirectLight *sun = NULL;

    if (scene.getActiveCamera() == NULL || scene.getActiveRenderTarget() == NULL) {
        return false;
    }

    scene.getActiveRenderTarget()->clear();

    /* Render the point lights shadows */
    for (std::vector<PointLight *>::iterator pointLight = scene.getPointLights().begin(); pointLight != scene.getPointLights().end();
         ++pointLight) {
        /* TODO: lookAt the center of the calculated bounding box, but for now this is enough */
        (*pointLight)->getShadowMap()->clear();
        (*pointLight)->lookAt(glm::vec3(0.0f, 0.0f, 0.0f));

        /* Render the shadow maps for all models */
        for (std::vector<Model3D *>::iterator model = scene.getModels().begin(); model != scene.getModels().end(); ++model) {
            renderToShadowMap(**model, **pointLight, *_shaderShadow);
        }

        /* Check if we need to render this light billboard */
        if ((*pointLight)->getRenderMarker() == true) {
            lightsMarkers.push_back(*pointLight);
        }
    }

    /* TODO: We only support one direct light for now */
    if (scene.getDirectLights().size() > 0) {
        sun = scene.getDirectLights()[0];

        /* TODO: lookAt in this case must be fixed to go along the direct light direction */
        sun->lookAt(glm::vec3(0.0f, 0.0f, 0.0f));
        sun->getShadowMap()->clear();

        /* Render the shadow maps for all models */
        for (std::vector<Model3D *>::iterator model = scene.getModels().begin(); model != scene.getModels().end(); ++model) {
            renderToShadowMap(**model, *sun, *_shaderShadow);
        }
    }

    /* Render the spot lights shadows */
    for (std::vector<SpotLight *>::iterator spotLight = scene.getSpotLights().begin(); spotLight != scene.getSpotLights().end();
         ++spotLight) {
        /* TODO: lookAt the center of the calculated bounding box, but for * now this is enough */
        (*spotLight)->lookAt(glm::vec3(0.0f, 0.0f, 0.0f));
        (*spotLight)->getShadowMap()->clear();

        /* Render the shadow maps for all models */
        for (std::vector<Model3D *>::iterator model = scene.getModels().begin(); model != scene.getModels().end(); ++model) {
            renderToShadowMap(**model, **spotLight, *_shaderShadow);
        }

        /* Check if we need to render this light billboard */
        if ((*spotLight)->getRenderMarker() == true) {
            lightsMarkers.push_back(*spotLight);
        }
    }

    /* Render all objects */
    for (std::vector<Model3D *>::iterator model = scene.getModels().begin(); model != scene.getModels().end(); ++model) {
        if ((*model)->getLightingShader() == NULL) {
            log("ERROR model has no lighting shader associated to it\n");
            continue;
        }

        renderModel3D(**model, *scene.getActiveCamera(), *(*model)->getLightingShader(), sun, scene.getPointLights(), scene.getSpotLights(),
                      0.5f, /* TODO: calculate the global ambient light */
                      *scene.getActiveRenderTarget());

        /* Render normals information */
        if ((*model)->getRenderNormals() == true) {
            /* TODO: Calculate proper normal size by analyzing all objects in the scene and taking
             * a proper average */
            renderModelNormals(**model, *scene.getActiveCamera(), *scene.getActiveRenderTarget(),
                               (*model)->getBoundingSphere().getRadius() * 0.0002);
        }
        /* Render bounding volumes information */
        if ((*model)->getRenderNormals() == true) {
            /* TODO: Calculate proper normal size by analyzing all objects in the scene and taking
             * a proper average */
            renderModelBoundingVolumes(**model, *scene.getActiveCamera(), *scene.getActiveRenderTarget());
        }
    }

    /* Render the required light markers */
    renderLights(lightsMarkers, *scene.getActiveCamera(), *scene.getActiveRenderTarget());

    /* Blit the render target to the screen */
    scene.getActiveRenderTarget()->blit();

    return true;
}
