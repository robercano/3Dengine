/**
 * @class	Scene
 * @brief	Represents a scene with all the elements needed to render the scene. This acts
 *          as a container for models, lights and any other element that may be needed to
 *          render the scene. Each added element will have an associated name that can be
 *          used to access the element to modify it, or to remove it from the scene
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include <glm/glm.hpp>
#include <string>
#include <map>
#include "Model3D.hpp"
#include "PointLight.hpp"
#include "SpotLight.hpp"
#include "DirectLight.hpp"
#include "Camera.hpp"
#include "RenderTarget.hpp"

class Scene
{
  public:
      /**
       * Constructor
       */
      Scene() {}

      /**
       * Methods to add different elements to the scene by name
       *
       * The elements are added to the scene and can be later retrieved
       * using the 'get' method using the same name.
       *
       * The name must be unique across all elements of the same type. The name
       * can be duplicated between elements of different types, although it is not
       * recommended
       *
       * When adding the first camera or render target, that element will be set as
       * the active camera or render target for the scene until it is changed using
       * setActiveCamera or setActiveRenderTarget
       *
       * @param name  Name of the element in the scene
       * @param elem  Element to be added to the scene
       *
       * @return true if the element was added or false if
       *              the name already exists
       */
      bool add(const std::string &name, Model3D *elem);
      bool add(const std::string &name, PointLight *elem);
      bool add(const std::string &name, SpotLight *elem);
      bool add(const std::string &name, DirectLight *elem);
      bool add(const std::string &name, Camera *elem);
      bool add(const std::string &name, RenderTarget *elem);

      /**
       * Methods to retrieve an element from the scene by name
       *
       * @param name  Name of the element in the scene
       *
       * @return The requested element
       */
      Model3D      *getModel(const std::string &name);
      PointLight   *getPointLight(const std::string &name);
      SpotLight    *getSpotLight(const std::string &name);
      DirectLight  *getDirectLight(const std::string &name);
      Camera       *getCamera(const std::string &name);
      RenderTarget *getRenderTarget(const std::string &name);

      /**
       * Retrieves the internal maps of elements
       *
       * @return The map of elements including the names and the elements
       */
      std::map<std::string, Model3D*> &getModels(void) { return _models; }
      std::map<std::string, PointLight*> &getPointLights(void) { return _pointLights; }
      std::map<std::string, SpotLight*> &getSpotLights(void) { return _spotLights; }
      std::map<std::string, DirectLight*> &getDirectLights(void) { return _directLights; }
      std::map<std::string, Camera*> &getCameras(void) { return _cameras; }
      std::map<std::string, RenderTarget*> &getRenderTargets(void) { return _renderTargets; }

      /**
       * Sets the active camera to be used for rendering
       *
       * TODO: Add viewport support to support multiple renderings.
       *       When viewports are supported a new function to associate
       *       a camera with a viewport will be needed
       *
       * @param name  Name of the camera to be set as active
       *
       * @return true if the camera was set as active or false if the
       *         name does not exist
       */
      bool setActiveCamera(const std::string &name);

      /**
       * Sets the active render target where the rendered image will be
       * written to
       *
       * TODO: Add viewport support to support multiple renderings.
       *       When viewports are supported a new function to associate
       *       a render target with a viewport will be needed
       *
       * @param name  Name of the render target to be set as active
       *
       * @return true if the render target was set as active or false if the
       *         name does not exist
       */
      bool setActiveRenderTarget(const std::string &name);

  private:
    std::map<std::string, Model3D *> _models;            /**< Contains all models in the scene associated by name */
    std::map<std::string, PointLight*> _pointLights;     /**< Contains all point lights in the scene associated by name */
    std::map<std::string, SpotLight*> _spotLights;       /**< Contains all spot lights in the scene associated by name */
    std::map<std::string, DirectLight*> _directLights;   /**< Contains all direct lights in the scene associated by name */
    std::map<std::string, Camera*> _cameras;             /**< Contains all cameras in the scene associated by name */
    std::map<std::string, RenderTarget*> _renderTargets; /**< Contains all render targets lights in the scene associated by name */

    Camera *_activeCamera;                /**< The current active camera */
    RenderTarget *_activeRenderTarget;    /**< The current active render target */
};
