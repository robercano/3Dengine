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
        _enableBoundingBox = false;
        _enableNormals = false;
        _enableLights = true;
        _enableWireframe = false;
    }

    bool handleInit(Game *game)
    {
        game->getWindowManager()->getWindowSize(&_width, &_height);

        /* Setup the normal render target for the shadow mapping */
        _renderTargetNormal = NOAARenderTarget::New();
        if (_renderTargetNormal == NULL) {
            log("ERROR allocating normal render target\n");
            return false;
        }

        _renderTargetNormal->init(_width, _height);
        _renderTargetNormal->setClearColor(0.0, 0.0, 0.0, 1.0);

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

        /* Create a Blinn-phong shader for the geometry */
        _shaderBlinnLight = BlinnPhongShader::New();
        if (_shaderBlinnLight->init() == false) {
            log("ERROR initializing toon lighting shader\n");
            return false;
        }
        _shaderShadow = NormalShadowMapShader::New();
        if (_shaderShadow->init() == false) {
            log("ERROR initializing shadow map shader\n");
            return false;
        }

        /* Point light */
        PointLight *light1 = new PointLight(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f),
                                           glm::vec3(20.0f, 20.0f, 0.0f), 0.0000099999f, 1000.0f);
        PointLight *light2 = new PointLight(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),
                                           glm::vec3(108.0f, 108.0f, 0.0f), 0.0000099999f, 1000.0f);
        PointLight *light3 = new PointLight(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f),
                                           glm::vec3(116.0f, 116.0f, 0.0f), 0.0000099999f, 1000.0f);

        light1->setProjection((float)_width / 4.0f, (float)_height / 4.0f, 0.1f, 10000.0f);
        light1->getShadowMap()->init(_width, _height);
        light2->setProjection((float)_width / 4.0f, (float)_height / 4.0f, 0.1f, 10000.0f);
        light2->getShadowMap()->init(_width, _height);
        light3->setProjection((float)_width / 4.0f, (float)_height / 4.0f, 0.1f, 10000.0f);
        light3->getShadowMap()->init(_width, _height);

        _pointLights.push_back(light1);
        _pointLights.push_back(light2);
        _pointLights.push_back(light3);

        /* Load the geometry */
        _model = game->getRenderer()->loadModelOBJ("data/objects/daxter");
        _model->setScaleFactor(glm::vec3(100.0f, 100.0f, 100.0f));
        _model->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
        _model->rotate(glm::toMat4(glm::quat(glm::vec3(0.0f, 45.0f, 0.0f))));

        /* Use a plane for the floor */
        _plane = game->getRenderer()->prepareModel(Procedural::Plane());
        _plane->setScaleFactor(glm::vec3(500.0f, 1.0f, 500.0f));
        _plane->setPosition(glm::vec3(0.0f, -70.0f, 0.0f));

        /* Create the game camera */
        _camera.setProjection((float)_width, (float)_height, 0.1f, 1000.0f, 45.0f);
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
        std::vector<PointLight *> _emptyPointLights;
        std::vector<SpotLight *> _emptySpotLights;

        _lightMarkers.clear();

        /* Apply the motion to the camera */
        _cameraMotion.applyTo(_camera);
        _renderTargetNormal->clear();

        for (std::vector<PointLight *>::iterator it = _pointLights.begin(); it != _pointLights.end(); ++it) {
            /* TODO: lookAt the center of the calculated bounding box, but for
             * now this is enough */
            (*it)->lookAt(_model->getPosition());
            (*it)->getShadowMap()->clear();

            game->getRenderer()->renderToShadowMap(*_model, *(*it), *_shaderShadow);
        }

        game->getRenderer()->setWireframeMode(_enableWireframe);

        /* Render all objects */
        game->getRenderer()->renderModel3D(*_model, _camera, *_shaderBlinnLight, NULL, _pointLights, _emptySpotLights, 0.4f, *_renderTargetNormal);
        game->getRenderer()->renderModel3D(*_plane, _camera, *_shaderBlinnLight, NULL, _pointLights, _emptySpotLights, 0.4f, *_renderTargetNormal);

        if (_enableNormals) {
            game->getRenderer()->renderModelNormals(*_model, _camera, *_renderTargetNormal);
        }
        if (_enableBoundingBox) {
            game->getRenderer()->renderModelBoundingBoxes(*_model, _camera, *_renderTargetNormal);
        }
        if (_enableLights) {
            for (std::vector<PointLight *>::iterator it = _pointLights.begin(); it != _pointLights.end(); ++it) {
                _lightMarkers.push_back(*it);
            }
        }
        game->getRenderer()->renderLights(_lightMarkers, _camera, *_renderTargetNormal);

        _renderTargetNormal->blit();

        game->getTextConsole()->gprintf("1=Bounding Box%s, 2=Normals %s, 3=Lights %s, 4=Wireframe %s\n",
                                        _enableBoundingBox ? "Off" : "On",
                                        _enableNormals ? "Off" : "On",
                                        _enableLights ? "Off" : "On",
                                        _enableWireframe ? "Off" : "On");
        return true;
    }

  private:
    Camera _camera;
    FlyMotion _cameraMotion;
    Model3D *_model;
    Model3D *_plane;
    BlinnPhongShader *_shaderBlinnLight;
    NormalShadowMapShader *_shaderShadow;
    NOAARenderTarget *_renderTargetNormal;
    DirectLight *_sun;
    InputManager _inputManager;
    std::string _current;
    std::vector<PointLight *> _pointLights;
    std::vector<Light *> _lightMarkers;

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
