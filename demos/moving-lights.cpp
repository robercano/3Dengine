#include <stdio.h>
#include "BlinnPhongShader.hpp"
#include "Camera.hpp"
#include "FXAA2RenderTarget.hpp"
#include "FlyMotion.hpp"
#include "Game.hpp"
#include "OpenGL.h"
#include "PointLight.hpp"
#include "Scene.hpp"

#define PI 3.14159265358979323846

class AntiaAliasingDemo : public GameHandler
{
  public:
    AntiaAliasingDemo()
    {
        _MouseSensibility = 10.0;
        _InvertMouse = 1.0;
        _prevX = 0xFFFFFF;
        _prevY = 0xFFFFFF;
        _angle = 0.0;
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
        keys.push_back('5');
        keys.push_back(GLFW_KEY_ESCAPE);

        game->getWindowManager()->getKeyManager()->registerListener(_inputManager, keys);
        game->getWindowManager()->getMouseManager()->registerListener(_inputManager);

        /* Get the window size */
        game->getWindowManager()->getWindowSize(&_width, &_height);

        /* Create the viewport */
        _viewport = new Viewport(0, 0, _width, _height);

        /* Add some lights */
        _scene.add("PL_light1", new PointLight(glm::vec3(5.0, 5.0, 5.0), glm::vec3(5.0, 5.0, 5.0), glm::vec3(5.0, 5.0, 5.0),
                                            glm::vec3(50.0, 100.0, 50.0), 0.0000099999f, 1000.0f));
        _scene.add("PL_light2", new PointLight(glm::vec3(5.0, 3.0, 3.0), glm::vec3(5.0, 3.0, 3.0), glm::vec3(5.0, 3.0, 3.0),
                                            glm::vec3(-50.0, 100.0, 50.0), 0.0000099999f, 1000.0f));
        _scene.add("PL_light3", new PointLight(glm::vec3(1.0, 1.0, 3.0), glm::vec3(1.0, 1.0, 3.0), glm::vec3(1.0, 1.0, 3.0),
                                            glm::vec3(50.0, 200.0, 100.0), 0.0000099999f, 1000.0f));

        _scene.getPointLight("PL_light1")->getShadowMap()->init(1, 1);
        _scene.getPointLight("PL_light2")->getShadowMap()->init(1, 1);
        _scene.getPointLight("PL_light3")->getShadowMap()->init(1, 1);

        /* Create a render target to allow post-processing */
        _scene.add("RT_fxaa2", FXAA2RenderTarget::New());
        _scene.getRenderTarget("RT_fxaa2")->init(_width, _height);

        /* Create a Blinn-phong shader for the geometry */
        BlinnPhongShader *blinnPhongShader = BlinnPhongShader::New();
        if (blinnPhongShader->init() == false) {
            printf("ERROR initializing blinn-phong shader\n");
            return false;
        }

        /* Load the geometry */
        Asset3D *deadpool = game->getRenderer()->loadAsset3D("data/models/internal/deadpool.model");

        _scene.add("M3D_deadpool", new Model3D(deadpool));
        _scene.getModel("M3D_deadpool")->setScaleFactor(glm::vec3(100.0f, 100.0f, 100.0f));
        _scene.getModel("M3D_deadpool")->setLightingShader(blinnPhongShader);

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
            _cameraMotion.forward(0.1f * (float)elapsedMs);
        }
        if (_inputManager._keys['S']) {
            _cameraMotion.forward(-0.1f * (float)elapsedMs);
        }
        if (_inputManager._keys['A']) {
            _cameraMotion.right(-0.1f * (float)elapsedMs);
        }
        if (_inputManager._keys['D']) {
            _cameraMotion.right(0.1f * (float)elapsedMs);
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

        /* Move the lights */
        _angle += (float)(2 * PI * elapsedMs / 5000.0);
        if (_angle > 2 * PI) {
            _angle -= (float)(2 * PI);
        }

        _scene.getPointLight("PL_light1")->setPosition(glm::vec3(50.0 * glm::sin(-_angle), 100.0, 50.0 * glm::cos(-_angle)));
        _scene.getPointLight("PL_light2")->setPosition(glm::vec3(-50.0 * glm::sin(_angle), 100.0, 500.0 * glm::cos(_angle)));
        _scene.getPointLight("PL_light3")->setPosition(glm::vec3(100.0 * glm::sin(_angle), 150.0, 100.0 * glm::cos(_angle)));

        return true;
    }

    bool handleRender(Game *game)
    {
        std::vector<SpotLight *> empty;

        /* Apply the motion to the camera */
        _cameraMotion.applyTo(*_scene.getCamera("C_camera1"));

        /* Render the scene */
        game->getRenderer()->renderScene(_scene, *_viewport);

        return true;
    }

  private:
    FlyMotion _cameraMotion;
    InputManager _inputManager;
    Scene _scene;

    float _MouseSensibility;
    float _InvertMouse;
    int32_t _prevX;
    int32_t _prevY;
    uint32_t _width;
    uint32_t _height;
    float _angle;
    Viewport *_viewport;
};

int main()
{
    Game *game;
    AntiaAliasingDemo antiAliasingDemo;

    game = new Game("Anti Aliasing Comparison demo");
    if (game == NULL) {
        fprintf(stderr, "ERROR creating new game\n");
        return 1;
    }

    game->setHandler(&antiAliasingDemo);
#if defined(_WIN32)
    game->setWindowSize(800, 600, false);
#else
    game->setWindowSize(2560, 1440, true);
#endif
    game->setFPS(60);

    if (game->init() == false) {
        fprintf(stderr, "ERROR initializing game\n");
        return 1;
    }

    game->loop();
    return 0;
}
