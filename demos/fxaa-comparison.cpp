#include <stdio.h>
#include "BlinnPhongShader.hpp"
#include "Camera.hpp"
#include "FXAA2RenderTarget.hpp"
#include "FXAARenderTarget.hpp"
#include "FlyMotion.hpp"
#include "Game.hpp"
#include "MSAARenderTarget.hpp"
#include "NOAARenderTarget.hpp"
#include "OpenGL.h"
#include "PointLight.hpp"
#include "SSAARenderTarget.hpp"
#include "SpotLight.hpp"

#define PI 3.14159265358979323846

class FxaaComparisonDemo : public GameHandler
{
  public:
    FxaaComparisonDemo()
    {
        _MouseSensibility = 10.0;
        _InvertMouse = 1.0;
        _prevX = 0xFFFFFF;
        _prevY = 0xFFFFFF;
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
        keys.push_back('F');
        keys.push_back('N');
        keys.push_back(GLFW_KEY_ESCAPE);

        game->getWindowManager()->getKeyManager()->registerListener(_inputManager, keys);
        game->getWindowManager()->getMouseManager()->registerListener(_inputManager);

        /* Get the window size */
        game->getWindowManager()->getWindowSize(&_width, &_height);

        /* Create the viewport */
        _viewportLeft = new Viewport(0, 0, _width / 2, _height);
        _viewportRight = new Viewport(_width / 2, 0, _width / 2, _height);

        _scene1.add("RT_noaa", NOAARenderTarget::New());
        _scene1.add("RT_fxaa", FXAARenderTarget::New());

        _scene2.add("RT_noaa", _scene1.getRenderTarget("RT_noaa"));
        _scene2.add("RT_fxaa2", FXAA2RenderTarget::New());

        _scene1.getRenderTarget("RT_noaa")->init(_width / 2, _height);
        _scene1.getRenderTarget("RT_fxaa")->init(_width / 2, _height);
        _scene2.getRenderTarget("RT_fxaa2")->init(_width / 2, _height);

        /* Add light */
        _scene1.add("PL_light1", new PointLight(glm::vec3(5.0, 5.0, 5.0), glm::vec3(5.0, 5.0, 5.0), glm::vec3(5.0, 5.0, 5.0),
                                           glm::vec3(50.0, 100.0, 50.0), 0.0000099999f, 1000.0f));
        _scene1.getPointLight("PL_light1")->getShadowMap()->init(1, 1);

        _scene2.add("PL_light1", _scene1.getPointLight("PL_light1"));

        /* Choose the render target here */
        _scene1.setActiveRenderTarget("RT_noaa");
        _scene2.setActiveRenderTarget("RT_noaa");
        _renderTargetName = "NOAA Left / NOAA Right";

        /* Create a Blinn-phong shader for the geometry */
        BlinnPhongShader *blinnPhongShader = BlinnPhongShader::New();
        if (blinnPhongShader->init() != true) {
            printf("ERROR initializing blinn-phong shader\n");
            return false;
        }

        /* Load the geometry */
        _scene1.add("M3D_deadpool", game->getRenderer()->loadModel("data/models/internal/deadpool.model"));
        _scene1.getModel("M3D_deadpool")->setScaleFactor(glm::vec3(100.0f, 100.0f, 100.0f));
        _scene1.getModel("M3D_deadpool")->setLightingShader(blinnPhongShader);

        _scene2.add("M3D_deadpool", _scene1.getModel("M3D_deadpool"));

        /* Create the game camera */
        _scene1.add("C_camera1", new Camera());
        _scene1.getCamera("C_camera1")->setProjection((float)_width / 2.0f, (float)_height, 0.1f, 1000.0f, 45.0f);

        _scene2.add("C_camera1", _scene1.getCamera("C_camera1"));

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
        if (_inputManager._keys['F']) {
            _scene1.setActiveRenderTarget("RT_fxaa");
            _scene2.setActiveRenderTarget("RT_fxaa2");
            _renderTargetName = "FXAA Left / FXAA2 Right";
        }
        if (_inputManager._keys['N']) {
            _scene1.setActiveRenderTarget("RT_noaa");
            _scene2.setActiveRenderTarget("RT_noaa");
            _renderTargetName = "NOAA Left / NOAA Right";
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
        /* Apply the motion to the camera */
        _cameraMotion.applyTo(*_scene1.getCamera("C_camera1"));

        game->getRenderer()->renderScene(_scene1, *_viewportLeft);
        game->getRenderer()->renderScene(_scene2, *_viewportRight);

        game->getTextConsole()->gprintf("N=NOAA, F=FXAA\n");
        game->getTextConsole()->gprintf("%s\n", _renderTargetName.c_str());
        return true;
    }

  private:
    FlyMotion _cameraMotion;
    std::string _renderTargetName;
    InputManager _inputManager;
    Scene _scene1, _scene2;
    Viewport *_viewportLeft, *_viewportRight;

    float _MouseSensibility;
    float _InvertMouse;
    int32_t _prevX;
    int32_t _prevY;
    uint32_t _width;
    uint32_t _height;
};

int main()
{
    Game *game;
    FxaaComparisonDemo demo;

    game = new Game("Anti Aliasing Comparison demo");
    if (game == NULL) {
        fprintf(stderr, "ERROR creating new game\n");
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
        fprintf(stderr, "ERROR initializing game\n");
        return 1;
    }

    game->loop();
    return 0;
}
