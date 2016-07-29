#include <stdio.h>
#include "BlinnPhongShader.hpp"
#include "Camera.hpp"
#include "FXAA2RenderTarget.hpp"
#include "FlyMotion.hpp"
#include "Game.hpp"
#include "OpenGL.h"
#include "PointLight.hpp"
#include "ToonLightingShader.hpp"
#include "ToonRenderTarget.hpp"
#include "Scene.hpp"

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

        _scene.add("PL_light1", new PointLight(glm::vec3(3.5, 3.5, 4.5), glm::vec3(3.5, 3.5, 4.5), glm::vec3(3.5, 3.5, 4.5),
                                            glm::vec3(75.0, 300.0, 150.0), 0.0000099999f, 1000.0f));

        _scene.getPointLight("PL_light1")->getShadowMap()->init(1, 1);

        /* Create a render target to allow post-processing */
        _scene.add("RT_fxaa2", FXAA2RenderTarget::New());
        _scene.getRenderTarget("RT_fxaa2")->init(_width, _height);
        _scene.getRenderTarget("RT_fxaa2")->setClearColor(1.0, 1.0, 1.0, 0.0);

        /* Create the toon target to add a border */
        _scene.add("RT_toon", ToonRenderTarget::New());
        _scene.getRenderTarget("RT_toon")->init(_width, _height);
        _scene.getRenderTarget("RT_toon")->setClearColor(1.0, 1.0, 1.0, 1.0);
        dynamic_cast<ToonRenderTarget*>(_scene.getRenderTarget("RT_toon"))->setBorderColor(glm::vec4(0.0, 0.0, 0.0, 1.0));

        _scene.setActiveRenderTarget("RT_fxaa2");

        /* Create a Blinn-phong shader for the geometry */
        _shaderToonLight = ToonLightingShader::New();
        if (_shaderToonLight->init() == false) {
            printf("ERROR initializing toon lighting shader\n");
            return false;
        }
        _shaderBlinnLight = BlinnPhongShader::New();
        if (_shaderBlinnLight->init() == false) {
            printf("ERROR initializing blinn-phong shader\n");
            return false;
        }

        _current = "Normal";

        /* Load the geometry */
        _scene.add("M3D_daxter1", game->getRenderer()->loadModel("data/models/internal/daxter.model"));
        _scene.getModel("M3D_daxter1")->setScaleFactor(glm::vec3(100.0f, 100.0f, 100.0f));
        _scene.getModel("M3D_daxter1")->setLightingShader(_shaderBlinnLight);
        _scene.getModel("M3D_daxter1")->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));

        _scene.add("M3D_daxter2", game->getRenderer()->loadModel("data/models/internal/daxter.model"));
        _scene.getModel("M3D_daxter2")->setScaleFactor(glm::vec3(100.0f, 100.0f, 100.0f));
        _scene.getModel("M3D_daxter2")->setLightingShader(_shaderBlinnLight);
        _scene.getModel("M3D_daxter2")->setPosition(glm::vec3(0.0f, 0.0f, -50.0f));

        _scene.add("M3D_daxter3", game->getRenderer()->loadModel("data/models/internal/daxter.model"));
        _scene.getModel("M3D_daxter3")->setScaleFactor(glm::vec3(100.0f, 100.0f, 100.0f));
        _scene.getModel("M3D_daxter3")->setLightingShader(_shaderBlinnLight);
        _scene.getModel("M3D_daxter3")->setPosition(glm::vec3(0.0f, 0.0f, -100.0f));

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
        if (_inputManager._keys['1']) {
            _scene.setActiveRenderTarget("RT_fxaa2");

            for (std::vector<Model3D*>::iterator it = _scene.getModels().begin();
                 it != _scene.getModels().end(); ++it) {
                (*it)->setLightingShader(_shaderBlinnLight);
            }
            _current = "Normal";
        }
        if (_inputManager._keys['2']) {
            _scene.setActiveRenderTarget("RT_fxaa2");

            for (std::vector<Model3D*>::iterator it = _scene.getModels().begin();
                 it != _scene.getModels().end(); ++it) {
                (*it)->setLightingShader(_shaderToonLight);
            }
            _current = "ToonLight";
        }
        if (_inputManager._keys['3']) {
            _scene.setActiveRenderTarget("RT_toon");

            for (std::vector<Model3D*>::iterator it = _scene.getModels().begin();
                 it != _scene.getModels().end(); ++it) {
                (*it)->setLightingShader(_shaderToonLight);
            }
            _current = "ToonLight+Filter";
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
        _cameraMotion.applyTo(*_scene.getCamera("C_camera1"));

        game->getRenderer()->renderScene(_scene, *_viewport);

        game->getTextConsole()->gprintf("Current = %s\n", _current.c_str());
        game->getTextConsole()->gprintf("1=Normal, 2=ToonLight, 3=ToonLight+Filter\n");

        return true;
    }

  private:
    Camera _camera;
    FlyMotion _cameraMotion;
    BlinnPhongShader *_shaderBlinnLight;
    ToonLightingShader *_shaderToonLight;
    std::string _renderTargetName;
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
    Viewport *_viewport;
};

int main()
{
    Game *game;
    Demo demo;

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
