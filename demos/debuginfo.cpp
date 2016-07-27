#include <stdio.h>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include "BlinnPhongShader.hpp"
#include "Camera.hpp"
#include "DirectLight.hpp"
#include "FXAA2RenderTarget.hpp"
#include "FlyMotion.hpp"
#include "Game.hpp"
#include "NOAARenderTarget.hpp"
#include "NormalShadowMapShader.hpp"
#include "OpenGL.h"
#include "Plane.hpp"
#include "PointLight.hpp"
#include "Logging.hpp"
#include "Scene.hpp"

using namespace Logging;

#define PI 3.14159265358979323846

class ShadowsDemo : public GameHandler
{
  public:
    ShadowsDemo()
    {
        _MouseSensibility = 10.0f;
        _KeyboardSensibility = 0.1f;
        _InvertMouse = 1.0;
        _prevX = 0xFFFFFF;
        _prevY = 0xFFFFFF;
        _angle = 0.0f;
        _enableBoundingBox = true;
        _enableNormals = true;
        _enableLights = true;
        _enableWireframe = true;
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
        keys.push_back('4');
        keys.push_back(GLFW_KEY_ESCAPE);

        game->getWindowManager()->getKeyManager()->registerListener(_inputManager, keys);
        game->getWindowManager()->getMouseManager()->registerListener(_inputManager);

        /* Get window size */
        game->getWindowManager()->getWindowSize(&_width, &_height);

        /* Create the viewport */
        _viewport = new Viewport(0, 0, _width, _height);

        /* Setup the normal render target for the shadow mapping */
        _scene.add("RT_noaa", NOAARenderTarget::New());
        _scene.getRenderTarget("RT_noaa")->init(_width, _height);
        _scene.getRenderTarget("RT_noaa")->setClearColor(0.0, 0.0, 0.0, 1.0);

        /* Create a Blinn-phong shader for the geometry */
        BlinnPhongShader *shaderBlinnLight = BlinnPhongShader::New();
        if (shaderBlinnLight->init() == false) {
            log("ERROR initializing toon lighting shader\n");
            return false;
        }

        /* Point light */
        _scene.add("PL_light1", new PointLight(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f),
                                           glm::vec3(90.0f, 90.0f, -10.0f), 0.0000099999f, 1000.0f));
        _scene.add("PL_light2", new PointLight(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),
                                           glm::vec3(110.0f, 110.0f, 20.0f), 0.0000099999f, 1000.0f));
        _scene.add("PL_light3", new PointLight(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f),
                                           glm::vec3(140.0f, 140.0f, 30.0f), 0.0000099999f, 1000.0f));

        _scene.getPointLight("PL_light1")->setProjection((float)_width / 4.0f, (float)_height / 4.0f, 0.1f, 10000.0f);
        _scene.getPointLight("PL_light1")->getShadowMap()->init(_width, _height);
        _scene.getPointLight("PL_light2")->setProjection((float)_width / 4.0f, (float)_height / 4.0f, 0.1f, 10000.0f);
        _scene.getPointLight("PL_light2")->getShadowMap()->init(_width, _height);
        _scene.getPointLight("PL_light3")->setProjection((float)_width / 4.0f, (float)_height / 4.0f, 0.1f, 10000.0f);
        _scene.getPointLight("PL_light3")->getShadowMap()->init(_width, _height);

        /* Load the geometry */
        _scene.add("M3D_daxter", game->getRenderer()->loadModelOBJ("data/objects/daxter"));
        _scene.getModel("M3D_daxter")->setScaleFactor(glm::vec3(100.0f, 100.0f, 100.0f));
        _scene.getModel("M3D_daxter")->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
        _scene.getModel("M3D_daxter")->rotate(glm::toMat4(glm::quat(glm::vec3(0.0f, 45.0f, 0.0f))));
        _scene.getModel("M3D_daxter")->setLightingShader(shaderBlinnLight);

        /* Use a plane for the floor */
        _scene.add("M3D_plane", game->getRenderer()->prepareModel(Procedural::Plane()));
        _scene.getModel("M3D_plane")->setScaleFactor(glm::vec3(500.0f, 1.0f, 500.0f));
        _scene.getModel("M3D_plane")->setPosition(glm::vec3(0.0f, -70.0f, 0.0f));
        _scene.getModel("M3D_plane")->setLightingShader(shaderBlinnLight);
        _scene.getModel("M3D_plane")->setShadowCaster(false);

        /* Create the game camera */
        _scene.add("C_camera1", new Camera());
        _scene.getCamera("C_camera1")->setProjection((float)_width, (float)_height, 0.1f, 1000.0f, 45.0f);

        _cameraMotion.setPosition(glm::vec3(150.0f, 100.0f, 150.0f));
        _cameraMotion.rotateYaw(-45.0f);
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
        if (_inputManager._keys['1'] && _key1Pressed == false) {
            _enableBoundingBox = !_enableBoundingBox;
        }
        _key1Pressed = _inputManager._keys['1'];

        if (_inputManager._keys['2'] && _key2Pressed == false) {
            _enableNormals = !_enableNormals;
        }
        _key2Pressed = _inputManager._keys['2'];

        if (_inputManager._keys['3'] && _key3Pressed == false) {
            _enableLights = !_enableLights;
        }
        _key3Pressed = _inputManager._keys['3'];

        if (_inputManager._keys['4'] && _key4Pressed == false) {
            _enableWireframe = !_enableWireframe;
        }
        _key4Pressed = _inputManager._keys['4'];

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

        game->getRenderer()->setWireframeMode(_enableWireframe);
        game->getRenderer()->setRenderNormals(_enableNormals);
        game->getRenderer()->setRenderLightsMarkers(_enableLights);

        _scene.getModel("M3D_daxter")->setRenderBoundingVolumes(_enableBoundingBox);

        game->getRenderer()->renderScene(_scene, *_viewport);

        game->getTextConsole()->gprintf("1=Bounding Box%s, 2=Normals %s, 3=Lights %s, 4=Wireframe %s\n",
                                        _enableBoundingBox ? "Off" : "On",
                                        _enableNormals ? "Off" : "On",
                                        _enableLights ? "Off" : "On",
                                        _enableWireframe ? "Off" : "On");
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
    float _angle;
    bool _enableBoundingBox, _enableNormals;
    bool _enableLights, _enableWireframe;
    bool _key1Pressed, _key2Pressed, _key3Pressed, _key4Pressed;
    Viewport *_viewport;
};

int main()
{
    Game *game;
    ShadowsDemo shadowsDemo;

    game = new Game("Anti Aliasing Comparison demo");
    if (game == NULL) {
        log("ERROR creating new game\n");
        return 1;
    }

    game->setHandler(&shadowsDemo);
#if defined(_WIN32)
    game->setWindowSize(800, 600, false);
#else
    game->setWindowSize(2560, 1440, false);
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
