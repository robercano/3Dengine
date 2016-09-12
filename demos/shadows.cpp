#include <stdio.h>
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
#include "Scene.hpp"
#include "Logging.hpp"

#define PI 3.14159265358979323846

using namespace Logging;

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
        _angle = 0.0;
        _enableDirectLight = true;
        _enableSpotLight = true;
        _enablePointLight = true;
        _key1Pressed = false;
        _key2Pressed = false;
        _key3Pressed = false;
        _key4Pressed = false;
        _key5Pressed = false;
        _key6Pressed = false;
        _animateSun = false;
        _moonlight = true;
        _stopMotion = false;
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
        keys.push_back('6');
        keys.push_back(GLFW_KEY_ESCAPE);

        game->getWindowManager()->getKeyManager()->registerListener(_inputManager, keys);
        game->getWindowManager()->getMouseManager()->registerListener(_inputManager);

        /* Get the window size */
        game->getWindowManager()->getWindowSize(&_width, &_height);

        /* Create the viewport */
        _viewport = new Viewport(0, 0, _width, _height);

        _scene.add("PL_light1", new PointLight(glm::vec3(1.0f, 1.0f, 0.2f), glm::vec3(0.4f, 0.2f, 0.2f), glm::vec3(0.4f, 0.2f, 0.2f),
                                            glm::vec3(-100.0f, 100.0f, 100.0f), 0.0000099999f, 1000.0f));
        _scene.getPointLight("PL_light1")->setProjection((float)_width / 4.0f, (float)_height / 4.0f, 0.1f, 10.0f);
        _scene.getPointLight("PL_light1")->getShadowMap()->init(_width, _height);

        _scene.add("PL_light2", new PointLight(glm::vec3(0.5f, 1.0f, 0.5f), glm::vec3(0.5f, 1.0f, 0.5f), glm::vec3(0.5f, 1.0f, 0.5f),
                                            glm::vec3(-100.0f, 100.0f, 100.0f), 0.0000099999f, 1000.0f));
        _scene.getPointLight("PL_light2")->setProjection((float)_width / 4.0f, (float)_height / 4.0f, 0.1f, 10000.0f);
        _scene.getPointLight("PL_light2")->getShadowMap()->init(_width, _height);
        _scene.add("PL_light3", new PointLight(glm::vec3(0.5f, 0.5f, 1.0f), glm::vec3(0.5f, 0.5f, 1.0f), glm::vec3(0.5f, 0.5f, 1.0f),
                                            glm::vec3(-100.0f, 100.0f, 100.0f), 0.0000099999f, 1000.0f));
        _scene.getPointLight("PL_light3")->setProjection((float)_width / 4.0f, (float)_height / 4.0f, 0.1f, 10000.0f);
        _scene.getPointLight("PL_light3")->getShadowMap()->init(_width, _height);

        _scene.add("SL_light1", new SpotLight(glm::vec3(2.0f, 0.5f, 0.5f), glm::vec3(2.0f, 0.5f, 0.5f), glm::vec3(2.0f, 0.5f, 0.5f),
                                          glm::vec3(160.0f, 170.0f, 0.0f), 15.0f, 3.0f, 0.0000099999f, 1000.0f));
        _scene.getSpotLight("SL_light1")->setProjection((float)_width / 4.0f, (float)_height / 4.0f, 0.1f, 10000.0f);
        _scene.getSpotLight("SL_light1")->getShadowMap()->init(_width, _height);
        _scene.getSpotLight("SL_light1")->lookAt(glm::vec3(0.0f, 0.0f, 0.0f));

        _scene.add("DL_light1", new DirectLight(glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(0.4f, 0.4f, 0.f),
                               glm::vec3(-100.0f, -100.0f, -100.0f)));
        /* TODO: Hack to properly calculate direct light frustum */
        _scene.getDirectLight("DL_light1")->setPosition(glm::vec3(245.0f, 300.0f, 170.0f));
        _scene.getDirectLight("DL_light1")->setProjection((float)_width / 4.0f, (float)_height / 4.0f, 0.1f, 1000.0f);
        _scene.getDirectLight("DL_light1")->getShadowMap()->init(_width, _height);

        /* Setup the normal render target for the shadow mapping */
        _scene.add("RT_noaa", NOAARenderTarget::New());
        _scene.getRenderTarget("RT_noaa")->init(_width, _height);
        _scene.getRenderTarget("RT_noaa")->setClearColor(0.0, 0.0, 0.0, 1.0);

        /* Create a Blinn-phong shader for the geometry */
        BlinnPhongShader *shaderBlinnLight = BlinnPhongShader::New();
        if (shaderBlinnLight->init() == false) {
            printf("ERROR initializing toon lighting shader\n");
            return false;
        }

        /* Load the geometry */
        Procedural::Plane *plane = new Procedural::Plane();

        Asset3D *daxter = game->getRenderer()->loadAsset3D("data/models/internal/daxter.model");
        if (game->getRenderer()->prepareAsset3D(*plane) == false) {
            log("ERROR preparing plane asset\n");
            return false;
        }

        _scene.add("M3D_daxter", new Model3D(daxter));
        _scene.getModel("M3D_daxter")->setScaleFactor(glm::vec3(100.0f, 100.0f, 100.0f));
        _scene.getModel("M3D_daxter")->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
        _scene.getModel("M3D_daxter")->setLightingShader(shaderBlinnLight);

        glm::vec3 angles(0.0f, 45.0f, 0.0f);
        glm::quat rot = glm::quat(angles);
        _scene.getModel("M3D_daxter")->rotate(glm::toMat4(rot));

        /* Use a plane for the floor */
        _scene.add("M3D_plane", plane);
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
            if (_scene.getDirectLight("DL_light1")->isEnabled()) {
                _scene.getDirectLight("DL_light1")->disable();
            } else {
                _scene.getDirectLight("DL_light1")->enable();
            }
            _enableDirectLight = !_enableDirectLight;
        }
        _key1Pressed = _inputManager._keys['1'];

        if (_inputManager._keys['2'] && _key2Pressed == false) {
            for (std::vector<SpotLight*>::iterator it = _scene.getSpotLights().begin();
                 it != _scene.getSpotLights().end(); ++it) {
                if ((*it)->isEnabled()) {
                    (*it)->disable();
                } else {
                    (*it)->enable();
                }
            }
            _enableSpotLight = !_enableSpotLight;
        }
        _key2Pressed = _inputManager._keys['2'];

        if (_inputManager._keys['3'] && _key3Pressed == false) {
            for (std::vector<PointLight*>::iterator it = _scene.getPointLights().begin();
                 it != _scene.getPointLights().end(); ++it) {
                if ((*it)->isEnabled()) {
                    (*it)->disable();
                } else {
                    (*it)->enable();
                }
            }
            _enablePointLight = !_enablePointLight;
        }
        _key3Pressed = _inputManager._keys['3'];

        if (_inputManager._keys['4'] && _key4Pressed == false) {
            _animateSun = !_animateSun;
        }
        _key4Pressed = _inputManager._keys['4'];

        if (_inputManager._keys['5'] && _key5Pressed == false) {
            _moonlight = !_moonlight;
        }
        _key5Pressed = _inputManager._keys['5'];

        if (_inputManager._keys['6'] && _key6Pressed == false) {
            _stopMotion = !_stopMotion;
        }
        _key6Pressed = _inputManager._keys['6'];

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

        /* Move the lights  */
        if (_stopMotion == false) {
            _angle += (float)(2 * PI * elapsedMs / 20000.0);
            if (_angle > 2 * PI) {
                _angle -= (float)(2 * PI);
            }

            for (int i = 0; i < (int)_scene.getPointLights().size(); ++i) {
                int sign = i % 2 ? -1 : 1;
                if (i == 0) {
                    _scene.getPointLights()[i]->setPosition(glm::vec3(10.0, 300.0, 300.0 * glm::cos((i + 1) * sign * _angle)));
                } else {
                    _scene.getPointLights()[i]->setPosition(
                            glm::vec3(200.0 * glm::sin((i + 1) * sign * _angle), 200.0, 200.0 * glm::cos((i + 1) * sign * _angle)));
                }
            }

            for (int i = 0; i < (int)_scene.getSpotLights().size(); ++i) {
                int sign = i % 2 ? -1 : 1;
                if (i == 1) {
                    _scene.getSpotLights()[i]->setPosition(glm::vec3(20.0, 200.0, 200.0 * glm::cos((i + 1) * sign * _angle)));
                } else {
                    _scene.getSpotLights()[i]->setPosition(
                            glm::vec3(240.0 * glm::sin((i + 1) * sign * _angle), 250.0, 260.0 * glm::cos((i + 1) * sign * _angle)));
                }
                _scene.getSpotLights()[i]->lookAt(glm::vec3(0.0f, 0.0f, 0.0f));
            }

            if (_animateSun) {
                float sunDial = 0.8f;
                float sunAngle = (float)(PI / 2.0f) + ((_angle <= PI ? _angle : (float)(2.0f * PI) - _angle) - (float)(PI / 2.0f)) * sunDial;

                _scene.getDirectLight("DL_light1")->setPosition(glm::vec3(200.0f * glm::cos(sunAngle), 200.0f * glm::sin(sunAngle), -150.0f));
            } else {
                _scene.getDirectLight("DL_light1")->setPosition(glm::vec3(-200.0f, 200.0f, -150.0f));
            }
        }

        if (_moonlight) {
            _scene.getDirectLight("DL_light1")->setAmbient(glm::vec3(0.4f, 0.4f, 0.4f));
            _scene.getDirectLight("DL_light1")->setDiffuse(glm::vec3(0.4f, 0.4f, 0.4f));
            _scene.getDirectLight("DL_light1")->setSpecular(glm::vec3(0.4f, 0.4f, 0.4f));
        } else {
            _scene.getDirectLight("DL_light1")->setAmbient(glm::vec3(1.4f, 1.4f, 1.1f));
            _scene.getDirectLight("DL_light1")->setDiffuse(glm::vec3(1.4f, 1.4f, 1.1f));
            _scene.getDirectLight("DL_light1")->setSpecular(glm::vec3(1.4f, 1.4f, 1.1f));
        }

        return true;
    }

    bool handleRender(Game *game)
    {
        /* Apply the motion to the camera */
        _cameraMotion.applyTo(*_scene.getCamera("C_camera1"));

        game->getRenderer()->renderScene(_scene, *_viewport);

        game->getTextConsole()->gprintf("1=DirectLight %s, 2=SpotLight %s, 3=PointLights %s, 4=Move Sun %s, 5=Moonlight/Daylight, 6=Motion %s\n",
                                        _enableDirectLight ? "Off" : "On", _enableSpotLight ? "Off" : "On",
                                        _enablePointLight ? "Off" : "On", _animateSun ? "Off" : "On",
                                        _stopMotion ? "On" : "Off");
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
    bool _enableDirectLight, _enableSpotLight, _enablePointLight;
    bool _key1Pressed, _key2Pressed, _key3Pressed, _key4Pressed, _key5Pressed, _key6Pressed;
    bool _animateSun;
    bool _moonlight;
    bool _stopMotion;
    Viewport *_viewport;
};

int main()
{
    Game *game;
    ShadowsDemo shadowsDemo;

    game = new Game("Anti Aliasing Comparison demo");
    if (game == NULL) {
        fprintf(stderr, "ERROR creating new game\n");
        return 1;
    }

    game->setHandler(&shadowsDemo);
    game->setWindowSize(2560, 1440, true);
    game->setFPS(60);

    if (game->init() == false) {
        fprintf(stderr, "ERROR initializing game\n");
        return 1;
    }

    game->loop();

    delete game;

    return 0;
}
