/**
 * @class	Scene
 * @brief	Represents a scene with all the elements needed to render the scene. This acts
 *          as a container for models, lights and any other element that may be needed to
 *          render the scene. Each added element will have an associated name that can be
 *          used to access the element to modify it, or to remove it from the scene
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#include "Scene.hpp"

bool Scene::add(const std::string &name, Model3D *elem)
{
    if (getModel(name) != NULL) {
        return false;
    }
    _modelsNames[name] = elem;
    _models.push_back(elem);
    return true;
}

bool Scene::add(const std::string &name, PointLight *elem)
{
    if (getPointLight(name) != NULL) {
        return false;
    }
    _pointLightsNames[name] = elem;
    _pointLights.push_back(elem);
    return true;
}

bool Scene::add(const std::string &name, SpotLight *elem)
{
    if (getSpotLight(name) != NULL) {
        return false;
    }
    _spotLightsNames[name] = elem;
    _spotLights.push_back(elem);
    return true;
}

bool Scene::add(const std::string &name, DirectLight *elem)
{
    if (getDirectLight(name) != NULL) {
        return false;
    }
    _directLightsNames[name] = elem;
    _directLights.push_back(elem);
    return true;
}

bool Scene::add(const std::string &name, Camera *elem)
{
    if (getCamera(name) != NULL) {
        return false;
    }
    _camerasNames[name] = elem;
    _cameras.push_back(elem);

    if (_activeCamera == NULL) {
        _activeCamera = elem;
    }
    return true;
}

bool Scene::add(const std::string &name, RenderTarget *elem)
{
    if (getRenderTarget(name) != NULL) {
        return false;
    }
    _renderTargetsNames[name] = elem;
    _renderTargets.push_back(elem);

    if (_activeRenderTarget == NULL) {
        _activeRenderTarget = elem;
    }
    return true;
}

Model3D *Scene::getModel(const std::string &name)
{
    std::map<std::string, Model3D *>::iterator it = _modelsNames.find(name);
    if (it == _modelsNames.end()) {
        return NULL;
    }
    return it->second;
}

PointLight *Scene::getPointLight(const std::string &name)
{
    std::map<std::string, PointLight *>::iterator it = _pointLightsNames.find(name);
    if (it == _pointLightsNames.end()) {
        return NULL;
    }
    return it->second;
}

SpotLight *Scene::getSpotLight(const std::string &name)
{
    std::map<std::string, SpotLight *>::iterator it = _spotLightsNames.find(name);
    if (it == _spotLightsNames.end()) {
        return NULL;
    }
    return it->second;
}

DirectLight *Scene::getDirectLight(const std::string &name)
{
    std::map<std::string, DirectLight *>::iterator it = _directLightsNames.find(name);
    if (it == _directLightsNames.end()) {
        return NULL;
    }
    return it->second;
}

Camera *Scene::getCamera(const std::string &name)
{
    std::map<std::string, Camera *>::iterator it = _camerasNames.find(name);
    if (it == _camerasNames.end()) {
        return NULL;
    }
    return it->second;
}

RenderTarget *Scene::getRenderTarget(const std::string &name)
{
    std::map<std::string, RenderTarget *>::iterator it = _renderTargetsNames.find(name);
    if (it == _renderTargetsNames.end()) {
        return NULL;
    }
    return it->second;
}

bool Scene::setActiveCamera(const std::string &name)
{
    Camera *camera = getCamera(name);
    if (camera == NULL) {
        return false;
    }
    _activeCamera = camera;
    return true;
}

bool Scene::setActiveRenderTarget(const std::string &name)
{
    RenderTarget *target = getRenderTarget(name);
    if (target == NULL) {
        return false;
    }
    _activeRenderTarget = target;
    return true;
}
