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
#include "Plane.hpp"
#include "Cube.hpp"
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
        _scene.getRenderTarget("RT_HDR")->init(_width, _height);
        _scene.getRenderTarget("RT_HDR")->setClearColor(0.0, 0.0, 0.0, 1.0);

        /* Create the normal render target */
        _scene.add("RT_noaa", NOAARenderTarget::New());
        _scene.getRenderTarget("RT_noaa")->init(_width, _height);
        _scene.getRenderTarget("RT_noaa")->setClearColor(0.0, 0.0, 0.0, 1.0);

        _scene.setActiveRenderTarget("RT_noaa");

        /* Create a Blinn-phong shader for the geometry */
        BlinnPhongShader *shaderBlinnLight = BlinnPhongShader::New();
        if (shaderBlinnLight->init() == false) {
            log("ERROR initializing toon lighting shader\n");
            return false;
        }

        /* Point light */
        _scene.add("SL_light1", new SpotLight(glm::vec3(1.0f, 1.0f, 0.8f), glm::vec3(1.0f, 1.0f, 0.8f), glm::vec3(1.0f, 1.0f, 0.8f),
                                           glm::vec3(0.0f, 150.0f, 150.0f), 30.0f, 3.0f, 0.0000099999f, 250.0f));
        _scene.add("SL_light2", new SpotLight(glm::vec3(10.0f, 10.0f, 8.0f), glm::vec3(10.0f, 10.0f, 8.0f), glm::vec3(10.0f, 10.0f, 8.0f),
                                           glm::vec3(120.0f, 150.0f, -100.0f), 30.0f, 3.0f, 0.0000099999f, 250.0f));
        _scene.add("SL_light3", new SpotLight(glm::vec3(50.0f, 50.0f, 40.0f), glm::vec3(50.0f, 50.0f, 40.0f), glm::vec3(50.0f, 50.0f, 40.0f),
                                           glm::vec3(-120.0f, 150.0f, -100.0f), 30.0f, 3.0f, 0.0000099999f, 250.0f));

        _scene.getSpotLight("SL_light1")->setProjection((float)_width / 4.0f, (float)_height / 4.0f, 0.1f, 10000.0f);
        _scene.getSpotLight("SL_light1")->getShadowMap()->init(_width, _height);
        _scene.getSpotLight("SL_light1")->lookAt(glm::vec3(0.0f, 0.0f, 150.0f));
        _scene.getSpotLight("SL_light2")->setProjection((float)_width / 4.0f, (float)_height / 4.0f, 0.1f, 10000.0f);
        _scene.getSpotLight("SL_light2")->getShadowMap()->init(_width, _height);
        _scene.getSpotLight("SL_light2")->lookAt(glm::vec3(120.0f, 0.0f, -100.0f));
        _scene.getSpotLight("SL_light3")->setProjection((float)_width / 4.0f, (float)_height / 4.0f, 0.1f, 10000.0f);
        _scene.getSpotLight("SL_light3")->getShadowMap()->init(_width, _height);
        _scene.getSpotLight("SL_light3")->lookAt(glm::vec3(-120.0f, 0.0f, -100.0f));

        /* Load the geometry */
        Asset3D *daxter = game->getRenderer()->loadAsset3D("data/models/internal/daxter.model");
        Procedural::Plane *plane = new Procedural::Plane();

        if (game->getRenderer()->prepareAsset3D(*plane) == false) {
            log("ERROR preparing plane asset\n");
            return false;
        }

        _scene.add("M3D_daxter", new Model3D(daxter));
        _scene.getModel("M3D_daxter")->setScaleFactor(glm::vec3(100.0f, 100.0f, 100.0f));
        _scene.getModel("M3D_daxter")->setPosition(glm::vec3(0.0f, 0.0f, 150.0f));
        _scene.getModel("M3D_daxter")->setLightingShader(shaderBlinnLight);

        _scene.add("M3D_daxter2", new Model3D(daxter));
        _scene.getModel("M3D_daxter2")->setScaleFactor(glm::vec3(100.0f, 100.0f, 100.0f));
        _scene.getModel("M3D_daxter2")->setPosition(glm::vec3(120.0f, 0.0f, -100.0f));
        _scene.getModel("M3D_daxter2")->setLightingShader(shaderBlinnLight);

        _scene.add("M3D_daxter3", new Model3D(daxter));
        _scene.getModel("M3D_daxter3")->setScaleFactor(glm::vec3(100.0f, 100.0f, 100.0f));
        _scene.getModel("M3D_daxter3")->setPosition(glm::vec3(-120.0f, 0.0f, -100.0f));
        _scene.getModel("M3D_daxter3")->setLightingShader(shaderBlinnLight);

        /* Use a plane for the floor */
        _scene.add("M3D_plane", plane);
        _scene.getModel("M3D_plane")->setScaleFactor(glm::vec3(500.0f, 1.0f, 500.0f));
        _scene.getModel("M3D_plane")->setPosition(glm::vec3(0.0f, -70.0f, 0.0f));
        _scene.getModel("M3D_plane")->setLightingShader(shaderBlinnLight);
        _scene.getModel("M3D_plane")->setShadowCaster(false);

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
        }

        if (_inputManager._keys['2']) {
            _scene.setActiveRenderTarget("RT_HDR");
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

        game->getRenderer()->renderScene(_scene, *_viewport);

        game->getTextConsole()->gprintf("1=Normal, 2=HDR, Y=Increase exposure H=Decrease exposure\n");
        if (_scene.getActiveRenderTarget() == _scene.getRenderTarget("RT_HDR")) {
            game->getTextConsole()->gprintf("Exposure: %.2f\n", dynamic_cast<HDRRenderTarget*>(_scene.getRenderTarget("RT_HDR"))->getExposure());
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
    bool _keyPlusPressed, _keyMinusPressed;
    Viewport *_viewport;
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
#if defined(_WIN32)
    game->setWindowSize(800, 600, false);
#else
    game->setWindowSize(2560, 1440, true);
#endif
    game->setFPS(60);

    if (game->init() == false) {
        log("ERROR initializing game\n");
        return 1;
    }

    game->loop();

    delete game;

    return 0;
}
