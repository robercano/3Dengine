#include <stdio.h>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include "BlinnPhongShader.hpp"
#include "Camera.hpp"
#include "DirectLight.hpp"
#include "FlyMotion.hpp"
#include "Game.hpp"
#include "NOAARenderTarget.hpp"
#include "NormalShadowMapShader.hpp"
#include "OpenGL.h"
#include "Plane.hpp"
#include "Cylinder.hpp"
#include "Circle.hpp"
#include "Cube.hpp"
#include "Torus.hpp"
#include "Logging.hpp"

using namespace Logging;

#define PI 3.14159265358979323846

class ProceduralDemo : public GameHandler
{
  public:
    ProceduralDemo()
    {
        _MouseSensibility = 10.0f;
        _KeyboardSensibility = 0.1f;
        _InvertMouse = 1.0;
        _prevX = 0xFFFFFF;
        _prevY = 0xFFFFFF;
        _angle = 0.0f;
        _sunIntensity = 0.8f;
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

        PointLight *light = new PointLight(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f),
                                           glm::vec3(-100.0f, 100.0f, 100.0f), 0.00000000001f, 1000000.0f);

        _sun = new DirectLight(glm::vec3(64.0/255.0f, 156.0f/255.0f, 255.0f/255.0f) * _sunIntensity,
                               glm::vec3(1.0f, 1.0f, 1.0f) * _sunIntensity,
                               glm::vec3(1.0f, 1.0f, 1.0f) * _sunIntensity,
                               glm::vec3(-100.0f, -100.0f, -100.0f));

        _renderTargetNormal->init(_width, _height);
        _renderTargetNormal->setClearColor(135.0f/255.0f, 206.0f/255.0f, 250.0f/255.0f, 1.0);

        /* Register the key and mouse listener */
        std::vector<uint32_t> keys;  // The keys should be read from a config file

        keys.push_back('W');
        keys.push_back('S');
        keys.push_back('A');
        keys.push_back('D');
        keys.push_back('R');
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

        /* Use a plane for the floor */
        _plane1 = game->getRenderer()->prepareModel(Procedural::Plane(500.0f, 500.0f, glm::vec3(1.0, 0.3f, 0.6f), 0.0f, 0.0f, 20, 20));
        _plane1->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));

        /* 2 bent planes for background */
        _plane2 = game->getRenderer()->prepareModel(Procedural::Plane(500.0f, 500.0f, glm::vec3(1.0, 0.8f, 0.1f), 0.0f, PI/2.0f, 20, 20));
        _plane2->setOrientation(glm::toMat4(glm::quat(glm::vec3(PI/2.0, PI/4.0f, 0.0f))));
        _plane2->setPosition(glm::vec3(-300.0f, 220.0f, -300.0f));
        _plane3 = game->getRenderer()->prepareModel(Procedural::Plane(500.0f, 500.0f, glm::vec3(0.8, 1.0f, 0.1f), PI, 0.0f, 20, 20));
        _plane3->setOrientation(glm::toMat4(glm::quat(glm::vec3(PI/2.0, -PI/4.0f, 0.0f))));
        _plane3->setPosition(glm::vec3(300.0f, 220.0f, -300.0f));

        /* Torus */
        _torus = game->getRenderer()->prepareModel(Procedural::Torus(10.0f, 4.0f, glm::vec3(1.0f, 0.0f, 0.0f), 50, 50));
        _torus->setOrientation(glm::toMat4(glm::quat(glm::vec3(0.0f, -PI/2.0f, 0.0f))));
        _torus->setPosition(glm::vec3(120.0f, 100.0f, 100.0f));

        /* Create a cube */
        _cube = game->getRenderer()->prepareModel(Procedural::Cube(150.0f, 50.0f, 100.0f, glm::vec3(0.5f, 0.3f, 1.0f), 10, 8, 2));
        _cube->setPosition(glm::vec3(0.0f, 60.0f, 0.0f));

        /* Create a circle */
        _circle = game->getRenderer()->prepareModel(Procedural::Circle(100.0f, glm::vec3(0.8f, 0.9f, 0.1f), 50));
        _circle->setPosition(glm::vec3(0.0f, 1.0f, 0.0f));

        /* Create a cylinder */
        _cylinder = game->getRenderer()->prepareModel(Procedural::Cylinder(20.0f, 40.0f, glm::vec3(0.2f, 1.0f, 0.4f), 50, 50));
        _cylinder->setPosition(glm::vec3(0.0f, 90.0f, 150.0f));

        /* Create the game camera */
        _camera.setProjection((float)_width, (float)_height, 0.1f, 1000.0f, 45.0f);
        _cameraMotion.setPosition(glm::vec3(150.0f, 100.0f, 150.0f));
        _cameraMotion.rotateYaw(-45.0f);

        /* Sun setting */
        _sun->setPosition(glm::vec3(245.0f, 300.0f, 170.0f));
        _sun->setProjection((float)_width / 4.0f, (float)_height / 4.0f, 0.1f, 1000.0f);
        _sun->getShadowMap()->init(_width, _height);

        light->setProjection((float)_width / 4.0f, (float)_height / 4.0f, 0.1f, 10000.0f);
        light->getShadowMap()->init(_width, _height);

        //_pointLights.push_back(light);

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
        std::vector<SpotLight *> _emptySpotLights;

        /* Apply the motion to the camera */
        _cameraMotion.applyTo(_camera);
        _renderTargetNormal->clear();

        //game->getRenderer()->setWireframeMode(true);

        for (std::vector<PointLight *>::iterator it = _pointLights.begin(); it != _pointLights.end(); ++it) {
            /* TODO: lookAt the center of the calculated bounding box, but for now this is enough */
            (*it)->getShadowMap()->clear();
            (*it)->lookAt(_cylinder->getPosition());
            game->getRenderer()->renderToShadowMap(*_cube, *(*it), *_shaderShadow);
            game->getRenderer()->renderToShadowMap(*_cylinder, *(*it), *_shaderShadow);
        }

        _sun->lookAt(_plane1->getPosition());
        _sun->getShadowMap()->clear();
        game->getRenderer()->renderToShadowMap(*_cube, *_sun, *_shaderShadow);
        game->getRenderer()->renderToShadowMap(*_cylinder, *_sun, *_shaderShadow);
        //game->getRenderer()->renderToShadowMap(*_circle, *_sun, *_shaderShadow);

        /* Render all objects */
        game->getRenderer()->renderModel3D(*_plane1, _camera, *_shaderBlinnLight, _sun, _pointLights, _emptySpotLights, _sunIntensity, *_renderTargetNormal);
        game->getRenderer()->renderModel3D(*_plane2, _camera, *_shaderBlinnLight, _sun, _pointLights, _emptySpotLights, _sunIntensity, *_renderTargetNormal);
        game->getRenderer()->renderModel3D(*_plane3, _camera, *_shaderBlinnLight, _sun, _pointLights, _emptySpotLights, _sunIntensity, *_renderTargetNormal);
        game->getRenderer()->renderModel3D(*_cube, _camera, *_shaderBlinnLight, _sun, _pointLights, _emptySpotLights, _sunIntensity, *_renderTargetNormal);
        game->getRenderer()->renderModel3D(*_cylinder, _camera, *_shaderBlinnLight, _sun, _pointLights, _emptySpotLights, _sunIntensity, *_renderTargetNormal);
       game->getRenderer()->renderModel3D(*_circle, _camera, *_shaderBlinnLight, _sun, _pointLights, _emptySpotLights, _sunIntensity, *_renderTargetNormal);
       game->getRenderer()->renderModel3D(*_torus, _camera, *_shaderBlinnLight, _sun, _pointLights, _emptySpotLights, _sunIntensity, *_renderTargetNormal);
        _renderTargetNormal->blit();

        return true;
    }

  private:
    Camera _camera;
    FlyMotion _cameraMotion;
    Model3D *_plane1, *_plane2, *_plane3, *_cube, *_cylinder, *_circle, *_torus;
    BlinnPhongShader *_shaderBlinnLight;
    NormalShadowMapShader *_shaderShadow;
    NOAARenderTarget *_renderTargetNormal;
    DirectLight *_sun;
    InputManager _inputManager;
    std::vector<PointLight *> _pointLights;

    float _MouseSensibility;
    float _KeyboardSensibility;
    float _InvertMouse;
    int32_t _prevX;
    int32_t _prevY;
    uint32_t _width;
    uint32_t _height;
    float _angle;
    float _sunIntensity;
};

int main()
{
    Game *game;
    ProceduralDemo proceduralDemo;

    game = new Game("Anti Aliasing Comparison demo");
    if (game == NULL) {
        log("ERROR creating new game\n");
        return 1;
    }

    game->setHandler(&proceduralDemo);
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
