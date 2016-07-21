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
#include "SpotLight.hpp"
#include "ToonLightingShader.hpp"
#include "ToonRenderTarget.hpp"
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
        _key1Pressed = false;
        _enableMovement = true;
        _enableSphere = true;
        _enableAABB = true;
        _enableOOBB = true;
    }

    bool handleInit(Game *game)
    {
        _sun = new DirectLight(glm::vec3(1.4f, 1.4f, 1.4f), glm::vec3(1.4f, 1.4f, 1.4f), glm::vec3(1.4f, 1.4f, 0.f),
                               glm::vec3(-200.0f, 200.0f, -150.0f));

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
        keys.push_back('5');
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

        /* Load the geometry */
        _model1 = game->getRenderer()->loadModelOBJ("data/objects/daxter");
        _model1->setScaleFactor(glm::vec3(50.0f, 50.0f, 50.0f));
        _model1->setPosition(glm::vec3(100.0f, 0.0f, 0.0f));

        _model2 = game->getRenderer()->loadModelOBJ("data/objects/deadpool");
        _model2->setScaleFactor(glm::vec3(50.0f, 50.0f, 50.0f));
        _model2->setPosition(glm::vec3(100.0f, 0.0f, 0.0f));

        _model3 = game->getRenderer()->loadModelOBJ("data/objects/deadpool");
        _model3->setScaleFactor(glm::vec3(50.0f, 50.0f, 50.0f));
        _model3->setPosition(glm::vec3(100.0f, 0.0f, 0.0f));

        /* Use a plane for the floor */
        Procedural::Plane plane;

        _plane = game->getRenderer()->prepareModel(plane);
        _plane->setScaleFactor(glm::vec3(500.0f, 1.0f, 500.0f));
        _plane->setPosition(glm::vec3(0.0f, -70.0f, 0.0f));

        /* Create the game camera */
        _camera.setProjection((float)_width, (float)_height, 0.1f, 1000.0f, 45.0f);
        _cameraMotion.setPosition(glm::vec3(150.0f, 100.0f, 150.0f));
        _cameraMotion.rotateYaw(-45.0f);

        /* TODO: Hack to properly calculate direct light frustum */
        _sun->setPosition(glm::vec3(245.0f, 300.0f, 170.0f));
        _sun->setProjection((float)_width / 4.0f, (float)_height / 4.0f, 0.1f, 1000.0f);
        _sun->getShadowMap()->init(_width, _height);
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
            _enableMovement = !_enableMovement;
        }
        _key1Pressed = _inputManager._keys['1'];
        if (_inputManager._keys['2'] && _key2Pressed == false) {
            _enableSphere = !_enableSphere;
        }
        _key2Pressed = _inputManager._keys['2'];
        if (_inputManager._keys['3'] && _key3Pressed == false) {
            _enableAABB = !_enableAABB;
        }
        _key3Pressed = _inputManager._keys['3'];
        if (_inputManager._keys['4'] && _key4Pressed == false) {
            _enableOOBB = !_enableOOBB;
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

        /* Move and rotate the model */
        if (_enableMovement) {
            _angle += (float)(2.0f * PI * elapsedMs / 10000.0f);
            if (_angle > 2.0f * PI) {
                _angle -= (float)(2.0f * PI);
            }

            glm::quat orientation = glm::quat(glm::vec3(0.0f, _angle, 0.0f));
            _model1->setOrientation(glm::toMat4(orientation));
            _model1->setPosition(glm::vec3(150.0f + 50.0f*glm::cos(_angle), 0.0, 50.0f*glm::sin(_angle)));

            orientation = glm::quat(glm::vec3(0.0f, -_angle, 0.0f));
            _model2->setOrientation(glm::toMat4(orientation));
            _model2->setPosition(glm::vec3(-50.0f + 50.0f*glm::cos(-_angle), 0.0, 50.0f*glm::sin(-_angle)));

            orientation = glm::quat(glm::vec3(0.0f, _angle, 0.0f));
            _model3->setOrientation(glm::toMat4(orientation));
            _model3->setPosition(glm::vec3(50.0f*glm::cos(_angle), 0.0, 160.0f + 50.0f*glm::sin(_angle)));
        }

        return true;
    }

    bool handleRender(Game *game)
    {
        std::vector<PointLight *> _emptyPointLights;
        std::vector<SpotLight *> _emptySpotLights;

        /* Apply the motion to the camera */
        _cameraMotion.applyTo(_camera);
        _renderTargetNormal->clear();

        /* Render the shadow map for the sun */
        _sun->lookAt(glm::vec3(0.0f, 0.0f, 0.0f));
        _sun->getShadowMap()->clear();
        game->getRenderer()->renderToShadowMap(*_model1, *_sun, *_shaderShadow);
        game->getRenderer()->renderToShadowMap(*_model2, *_sun, *_shaderShadow);
        game->getRenderer()->renderToShadowMap(*_model3, *_sun, *_shaderShadow);

        /* Render all objects */
        game->getRenderer()->renderModel3D(*_model1, _camera, *_shaderBlinnLight, _sun, _emptyPointLights, _emptySpotLights, 0.4f, *_renderTargetNormal);
        game->getRenderer()->renderModel3D(*_model2, _camera, *_shaderBlinnLight, _sun, _emptyPointLights, _emptySpotLights, 0.4f, *_renderTargetNormal);
        game->getRenderer()->renderModel3D(*_model3, _camera, *_shaderBlinnLight, _sun, _emptyPointLights, _emptySpotLights, 0.4f, *_renderTargetNormal);
        game->getRenderer()->renderModel3D(*_plane, _camera, *_shaderBlinnLight, _sun, _emptyPointLights, _emptySpotLights, 0.4f, *_renderTargetNormal);

        game->getRenderer()->renderModelBoundingVolumes(*_model1, _camera, *_renderTargetNormal, _enableSphere, _enableAABB, _enableOOBB);
        game->getRenderer()->renderModelBoundingVolumes(*_model2, _camera, *_renderTargetNormal, _enableSphere, _enableAABB, _enableOOBB);
        game->getRenderer()->renderModelBoundingVolumes(*_model3, _camera, *_renderTargetNormal, _enableSphere, _enableAABB, _enableOOBB);

        _renderTargetNormal->blit();

        game->getTextConsole()->gprintf("1=Movement %s, 2=Sphere %s, 3=AABB %s, 4=OOBB %s\n",
                _enableMovement ? "Off" : "On",
                _enableSphere ? "Off" : "On",
                _enableAABB ? "Off" : "On",
                _enableOOBB ? "Off" : "On");
        game->getTextConsole()->gprintf("Red=Sphere, Green=AABB, Blue=OOBB\n");
        return true;
    }

  private:
    Camera _camera;
    FlyMotion _cameraMotion;
    Model3D *_model1, *_model2, *_model3;
    Model3D *_plane;
    BlinnPhongShader *_shaderBlinnLight;
    NormalShadowMapShader *_shaderShadow;
    NOAARenderTarget *_renderTargetNormal;
    DirectLight *_sun;
    InputManager _inputManager;
    std::string _current;

    float _MouseSensibility;
    float _KeyboardSensibility;
    float _InvertMouse;
    int32_t _prevX;
    int32_t _prevY;
    uint32_t _width;
    uint32_t _height;
    float _angle;
    bool _key1Pressed, _key2Pressed, _key3Pressed, _key4Pressed;
    bool _enableMovement;
    bool _enableSphere, _enableAABB, _enableOOBB;
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
