#include <stdio.h>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include "OpenGL.h"
#include "Camera.hpp"
#include "DirectLight.hpp"
#include "FlyMotion.hpp"
#include "Game.hpp"
#include "HDRRenderTarget.hpp"
#include "NOAARenderTarget.hpp"
#include "BlinnPhongShader.hpp"
#include "LightEmitShader.hpp"
#include "GaussianBlurRenderTarget.hpp"
#include "Plane.hpp"
#include "Sphere.hpp"
#include "PointLight.hpp"
#include "Logging.hpp"
#include "Scene.hpp"

using namespace Logging;

#define PI 3.14159265358979323846

class Demo : public GameHandler
{
  public:
    Demo()
    {
        _MouseSensibility = 10.0f;
        _KeyboardSensibility = 0.1f;
        _InvertMouse = 1.0;
        _prevX = 0xFFFFFF;
        _prevY = 0xFFFFFF;
        _keyPlusPressed = false;
        _keyMinusPressed = false;
        _gaussianBlurH = NULL;
        _gaussianBlurV = NULL;
        _enableBloom = false;
    }

    bool handleInit(Game *game)
    {
        /* Register the key and mouse listener */
        std::vector<uint32_t> keys;  // The keys should be read from a config file

        keys.push_back('W');
        keys.push_back('S');
        keys.push_back('A');
        keys.push_back('D');
        keys.push_back('R');
        keys.push_back('1');
        keys.push_back('2');
        keys.push_back('3');
        keys.push_back('Y');
        keys.push_back('H');
        keys.push_back(GLFW_KEY_ESCAPE);

        game->getWindowManager()->getKeyManager()->registerListener(_inputManager, keys);
        game->getWindowManager()->getMouseManager()->registerListener(_inputManager);

        /* Get window size */
        game->getWindowManager()->getWindowSize(&_width, &_height);

        /* Create the viewport */
        _viewport = new Viewport(0, 0, _width, _height);

        /* Create the HDR render target */
        _scene.add("RT_HDR", HDRRenderTarget::New());
        _scene.getRenderTarget("RT_HDR")->init(_width, _height, 0, 2);
        _scene.getRenderTarget("RT_HDR")->setClearColor(0.0, 0.0, 0.0, 0.0);

        /* Create the normal render target */
        _scene.add("RT_noaa", NOAARenderTarget::New());
        _scene.getRenderTarget("RT_noaa")->init(_width, _height);
        _scene.getRenderTarget("RT_noaa")->setClearColor(0.0, 0.0, 0.0, 1.0);

        /* Create the gaussian blur render target */
        _gaussianBlurH = GaussianBlurRenderTarget::New();
        _gaussianBlurH->init(_width, _height);
        _gaussianBlurH->setClearColor(0.0, 0.0, 0.0, 1.0);
        _gaussianBlurH->setHorizontal(true);

        _gaussianBlurV = GaussianBlurRenderTarget::New();
        _gaussianBlurV->init(_width, _height);
        _gaussianBlurV->setClearColor(0.0, 0.0, 0.0, 1.0);
        _gaussianBlurV->setHorizontal(false);

        _scene.setActiveRenderTarget("RT_noaa");

        /* Create a Blinn-phong shader for the geometry */
        BlinnPhongShader *shaderBlinnLight = BlinnPhongShader::New();
        if (shaderBlinnLight->init() == false) {
            log("ERROR initializing blinn-phong lighting shader\n");
            return false;
        }

        /* Create a Flat shader for the geometry */
        LightEmitShader *shaderLightEmit = LightEmitShader::New();
        if (shaderLightEmit->init() == false) {
            log("ERROR initializing flat lighting shader\n");
            return false;
        }

        /* Point light */
        _scene.add("SL_light1", new PointLight(glm::vec3(1.0f, 1.0f, 0.8f), glm::vec3(1.0f, 1.0f, 0.8f), glm::vec3(1.0f, 1.0f, 0.8f),
                                           glm::vec3(0.0f, 150.0f, 150.0f), 0.0000099999f, 240.0f));
        _scene.add("SL_light2", new PointLight(glm::vec3(10.0f, 10.0f, 8.0f), glm::vec3(10.0f, 10.0f, 8.0f), glm::vec3(10.0f, 10.0f, 8.0f),
                                           glm::vec3(160.0f, 150.0f, -100.0f), 0.0000099999f, 240.0f));
        _scene.add("SL_light3", new PointLight(glm::vec3(50.0f, 50.0f, 40.0f), glm::vec3(50.0f, 50.0f, 40.0f), glm::vec3(50.0f, 50.0f, 40.0f),
                                           glm::vec3(-160.0f, 150.0f, -100.0f), 0.0000099999f, 240.0f));

        _scene.getPointLight("SL_light1")->setProjection((float)_width / 4.0f, (float)_height / 4.0f, 0.1f, 10000.0f);
        _scene.getPointLight("SL_light1")->getShadowMap()->init(_width, _height);
        _scene.getPointLight("SL_light1")->lookAt(glm::vec3(0.0f, 0.0f, 150.0f));
        _scene.getPointLight("SL_light2")->setProjection((float)_width / 4.0f, (float)_height / 4.0f, 0.1f, 10000.0f);
        _scene.getPointLight("SL_light2")->getShadowMap()->init(_width, _height);
        _scene.getPointLight("SL_light2")->lookAt(glm::vec3(120.0f, 0.0f, -100.0f));
        _scene.getPointLight("SL_light3")->setProjection((float)_width / 4.0f, (float)_height / 4.0f, 0.1f, 10000.0f);
        _scene.getPointLight("SL_light3")->getShadowMap()->init(_width, _height);
        _scene.getPointLight("SL_light3")->lookAt(glm::vec3(-120.0f, 0.0f, -100.0f));

        /* Load the geometry */
        Asset3D *daxter = game->getRenderer()->loadAsset3D("data/models/internal/daxter.model");
        Procedural::Plane *plane = new Procedural::Plane();
        Procedural::Sphere *sphere1 = new Procedural::Sphere(25.0f, glm::vec3(1.0f, 1.0f, 1.0f), 50, 50);
        Procedural::Sphere *sphere2 = new Procedural::Sphere(25.0f, glm::vec3(1.0f, 1.0f, 1.0f), 50, 50);
        Procedural::Sphere *sphere3 = new Procedural::Sphere(25.0f, glm::vec3(1.0f, 1.0f, 1.0f), 50, 50);

        if (game->getRenderer()->prepareAsset3D(*plane) == false) {
            log("ERROR preparing plane asset\n");
            return false;
        }
        if (game->getRenderer()->prepareAsset3D(*sphere1) == false) {
            log("ERROR preparing sphere1 asset\n");
            return false;
        }
        if (game->getRenderer()->prepareAsset3D(*sphere2) == false) {
            log("ERROR preparing sphere2 asset\n");
            return false;
        }
        if (game->getRenderer()->prepareAsset3D(*sphere3) == false) {
            log("ERROR preparing sphere3 asset\n");
            return false;
        }

        _scene.add("M3D_daxter", new Model3D(daxter));
        _scene.getModel("M3D_daxter")->setScaleFactor(glm::vec3(100.0f, 100.0f, 100.0f));
        _scene.getModel("M3D_daxter")->setPosition(glm::vec3(0.0f, 0.0f, 150.0f));
        _scene.getModel("M3D_daxter")->setLightingShader(shaderBlinnLight);

        _scene.add("M3D_daxter2", new Model3D(daxter));
        _scene.getModel("M3D_daxter2")->setScaleFactor(glm::vec3(100.0f, 100.0f, 100.0f));
        _scene.getModel("M3D_daxter2")->setPosition(glm::vec3(160.0f, 0.0f, -100.0f));
        _scene.getModel("M3D_daxter2")->setLightingShader(shaderBlinnLight);

        _scene.add("M3D_daxter3", new Model3D(daxter));
        _scene.getModel("M3D_daxter3")->setScaleFactor(glm::vec3(100.0f, 100.0f, 100.0f));
        _scene.getModel("M3D_daxter3")->setPosition(glm::vec3(-160.0f, 0.0f, -100.0f));
        _scene.getModel("M3D_daxter3")->setLightingShader(shaderBlinnLight);

        /* Use a plane for the floor */
        _scene.add("M3D_plane", plane);
        _scene.getModel("M3D_plane")->setScaleFactor(glm::vec3(600.0f, 1.0f, 600.0f));
        _scene.getModel("M3D_plane")->setPosition(glm::vec3(0.0f, -70.0f, 0.0f));
        _scene.getModel("M3D_plane")->setLightingShader(shaderBlinnLight);
        _scene.getModel("M3D_plane")->setShadowCaster(false);

        /* Add light spheres */
        _scene.add("M3D_sphere1", sphere1);
        _scene.getModel("M3D_sphere1")->setPosition(glm::vec3(0.0f, 150.0f, 150.0f));
        _scene.getModel("M3D_sphere1")->setLightingShader(shaderLightEmit);
        _scene.getModel("M3D_sphere1")->setShadowCaster(false);
        _scene.getModel("M3D_sphere1")->setShadowReceiver(false);
        _scene.add("M3D_sphere2", sphere2);
        _scene.getModel("M3D_sphere2")->setPosition(glm::vec3(160.0f, 150.0f, -100.0f));
        _scene.getModel("M3D_sphere2")->setLightingShader(shaderLightEmit);
        _scene.getModel("M3D_sphere2")->setShadowCaster(false);
        _scene.getModel("M3D_sphere2")->setShadowReceiver(false);
        _scene.add("M3D_sphere3", sphere3);
        _scene.getModel("M3D_sphere3")->setPosition(glm::vec3(-160.0f, 150.0f, -100.0f));
        _scene.getModel("M3D_sphere3")->setLightingShader(shaderLightEmit);
        _scene.getModel("M3D_sphere3")->setShadowCaster(false);
        _scene.getModel("M3D_sphere3")->setShadowReceiver(false);

        /* Create the game camera */
        _scene.add("C_camera1", new Camera());
        _scene.getCamera("C_camera1")->setProjection((float)_width, (float)_height, 0.1f, 10000.0f, 45.0f);

        _cameraMotion.setPosition(glm::vec3(0.0f, 250.0f, 400.0f));
        _cameraMotion.rotatePitch(40.0f);

        return true;
    }

    bool handleTick(Game *game, double elapsedMs)
    {
        /* Keyboard */
        if (_inputManager._keys[GLFW_KEY_ESCAPE]) {
            return false;
        }
        if (_inputManager._keys['W']) {
            _cameraMotion.forward(_KeyboardSensibility * (float)elapsedMs);
        }
        if (_inputManager._keys['S']) {
            _cameraMotion.forward(-_KeyboardSensibility * (float)elapsedMs);
        }
        if (_inputManager._keys['A']) {
            _cameraMotion.right(-_KeyboardSensibility * (float)elapsedMs);
        }
        if (_inputManager._keys['D']) {
            _cameraMotion.right(_KeyboardSensibility * (float)elapsedMs);
        }
        if (_inputManager._keys['R']) {
            game->resetStats();
        }
        if (_inputManager._keys['1']) {
            _scene.setActiveRenderTarget("RT_noaa");
            _enableBloom = false;
        }
        if (_inputManager._keys['2']) {
            _scene.setActiveRenderTarget("RT_HDR");
            _enableBloom = false;
        }
        if (_inputManager._keys['3']) {
            _scene.setActiveRenderTarget("RT_HDR");
            _enableBloom = true;
        }

        if (_inputManager._keys['Y'] && _keyPlusPressed == false) {
            HDRRenderTarget *target = dynamic_cast<HDRRenderTarget*>(_scene.getRenderTarget("RT_HDR"));

            if (_scene.getActiveRenderTarget() == target) {
                target->setExposure(target->getExposure()*1.1f);
            }
        }
        _keyPlusPressed = _inputManager._keys['Y'];

        if (_inputManager._keys['H'] && _keyMinusPressed == false) {
            HDRRenderTarget *target = dynamic_cast<HDRRenderTarget*>(_scene.getRenderTarget("RT_HDR"));

            if (_scene.getActiveRenderTarget() == target) {
                target->setExposure(target->getExposure()/1.1f);
            }
        }
        _keyMinusPressed = _inputManager._keys['H'];

        /* Mouse */
        if (_prevX == 0xFFFFFF) {
            _prevX = _inputManager._xMouse;
        }
        if (_prevY == 0xFFFFFF) {
            _prevY = _inputManager._yMouse;
        }

        int32_t diffMouseX = _inputManager._xMouse - _prevX;
        int32_t diffMouseY = (int32_t)(_InvertMouse * (_inputManager._yMouse - _prevY));

        if (diffMouseX) {
            _cameraMotion.rotateYaw((float)(_MouseSensibility * PI * diffMouseX / _width));
        }
        if (diffMouseY) {
            _cameraMotion.rotatePitch((float)(_MouseSensibility * PI * diffMouseY / _height));
        }

        _prevX = _inputManager._xMouse;
        _prevY = _inputManager._yMouse;

        return true;
    }

    bool handleRender(Game *game)
    {
        /* Apply the motion to the camera */
        _cameraMotion.applyTo(*_scene.getCamera("C_camera1"));

        game->getRenderer()->renderScene(_scene, *_viewport, false);
        _scene.getActiveRenderTarget()->blit();

        /* Bloom effect */
        if (_enableBloom == true && _scene.getActiveRenderTarget() == _scene.getRenderTarget("RT_HDR")) {
            _gaussianBlurH->clear();
            _gaussianBlurV->clear();

            _gaussianBlurV->bind();
            dynamic_cast<HDRRenderTarget*>(_scene.getActiveRenderTarget())->disableToneMapping();
            dynamic_cast<HDRRenderTarget*>(_scene.getActiveRenderTarget())->setBlendingMode(RenderTarget::BLENDING_NONE);

            _scene.getActiveRenderTarget()->blit(0, 0, _width, _height, 1, false);
            _gaussianBlurV->unbind();

            _gaussianBlurH->bind();
            _gaussianBlurV->blit(0, 0, _width, _height, 0, false);
            _gaussianBlurH->unbind();

            _scene.getActiveRenderTarget()->bind();
            _gaussianBlurH->blit(0, 0, _width, _height, 0, false);
            _scene.getActiveRenderTarget()->unbind();

            dynamic_cast<HDRRenderTarget*>(_scene.getActiveRenderTarget())->enableToneMapping();
            dynamic_cast<HDRRenderTarget*>(_scene.getActiveRenderTarget())->setBlendingMode(RenderTarget::BLENDING_ADDITIVE);
            _scene.getActiveRenderTarget()->blit();
        }

        game->getTextConsole()->gprintf("1=Normal, 2=HDR, 3=Bloom, Y=Increase exposure H=Decrease exposure\n");
        if (_scene.getActiveRenderTarget() == _scene.getRenderTarget("RT_HDR")) {
            game->getTextConsole()->gprintf("Bloom: %s, Exposure: %.2f\n", _enableBloom ? "On" : "Off", dynamic_cast<HDRRenderTarget*>(_scene.getRenderTarget("RT_HDR"))->getExposure());
        } else {
            game->getTextConsole()->gprintf("HDR disabled\n");
        }
        return true;
    }

  private:
    FlyMotion _cameraMotion;
    InputManager _inputManager;
    std::string _current;
    Scene _scene;

    float _MouseSensibility;
    float _KeyboardSensibility;
    float _InvertMouse;
    int32_t _prevX;
    int32_t _prevY;
    uint32_t _width;
    uint32_t _height;
    bool _enableBloom, _keyPlusPressed, _keyMinusPressed;
    Viewport *_viewport;
    GaussianBlurRenderTarget *_gaussianBlurV, *_gaussianBlurH;
};

int main()
{
    Game *game;
    Demo demo;

    game = new Game("HDR demo");
    if (game == NULL) {
        log("ERROR creating new game\n");
        return 1;
    }

    game->setHandler(&demo);
    game->setWindowSize(2560, 1440, true);
    game->setFPS(60);

    if (game->init() == false) {
        log("ERROR initializing game\n");
        return 1;
    }

    game->loop();

    delete game;

    return 0;
}
