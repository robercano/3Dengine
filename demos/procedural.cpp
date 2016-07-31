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
#include "Terrain.hpp"
#include "Plane.hpp"
#include "Cylinder.hpp"
#include "Circle.hpp"
#include "Cube.hpp"
#include "BentPlane.hpp"
#include "Torus.hpp"
#include "Sphere.hpp"
#include "Triangle.hpp"
#include "Scene.hpp"
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

        /* Get the window size */
        game->getWindowManager()->getWindowSize(&_width, &_height);

        /* Create the viewport */
        _viewport = new Viewport(0, 0, _width, _height);

        /* Setup the normal render target for the shadow mapping */
        _scene.add("RT_noaa", NOAARenderTarget::New());
        _scene.getRenderTarget("RT_noaa")->init(_width, _height);
        _scene.getRenderTarget("RT_noaa")->setClearColor(135.0f/255.0f, 206.0f/255.0f, 250.0f/255.0f, 1.0);

        /* Setup the sun */
        _scene.add("Sun", new DirectLight(glm::vec3(64.0/255.0f, 156.0f/255.0f, 255.0f/255.0f) * _sunIntensity,
                                         glm::vec3(1.0f, 1.0f, 1.0f) * _sunIntensity,
                                         glm::vec3(1.0f, 1.0f, 1.0f) * _sunIntensity,
                                         glm::vec3(-100.0f, -100.0f, -100.0f)));
        _scene.getDirectLight("Sun")->setPosition(glm::vec3(0.0f, 300.0f, 170.0f));
        _scene.getDirectLight("Sun")->setProjection((float)_width / 4.0f, (float)_height / 4.0f, 0.1f, 1000.0f);
        _scene.getDirectLight("Sun")->getShadowMap()->init(_width, _height);

        /* Create a Blinn-phong shader for the geometry */
        BlinnPhongShader *lightShader = BlinnPhongShader::New();
        if (lightShader->init() == false) {
            log("ERROR initializing toon lighting shader\n");
            return false;
        }

        /* Generate a terrain for the _scene */
        Asset3D *terrain = game->getRenderer()->prepareAsset3D(Procedural::Terrain(500.0f, 500.0f, 500.0f, 0, glm::vec3(1.0, 0.3f, 0.6f), 200, 200, 5, 0.5));

        _scene.add("M3D_terrain", new Model3D(*terrain));
        _scene.getModel("M3D_terrain")->setPosition(glm::vec3(0.0f, -100.0f, 0.0f));
        _scene.getModel("M3D_terrain")->setLightingShader(lightShader);
        _scene.getModel("M3D_terrain")->setShadowCaster(false);

        /* 2 bent planes for background */
        Asset3D *plane1 = game->getRenderer()->prepareAsset3D(Procedural::BentPlane(500.0f, 500.0f, glm::vec3(1.0, 0.8f, 0.1f), (float)(PI/2.0f), 20, 20));
        Asset3D *plane2 = game->getRenderer()->prepareAsset3D(Procedural::BentPlane(500.0f, 500.0f, glm::vec3(0.8, 1.0f, 0.1f), (float)PI, 20, 20));

        _scene.add("M3D_plane1", new Model3D(*plane1));
        _scene.getModel("M3D_plane1")->setOrientation(glm::toMat4(glm::quat(glm::vec3(0.0f, PI/2.0f, 0.0f))));
        _scene.getModel("M3D_plane1")->rotate(glm::toMat4(glm::quat(glm::vec3(PI/2.0, PI/4.0f, 0.0f))));
        _scene.getModel("M3D_plane1")->setPosition(glm::vec3(-300.0f, 220.0f, -300.0f));
        _scene.getModel("M3D_plane1")->setLightingShader(lightShader);
        _scene.getModel("M3D_plane1")->setShadowCaster(false);

        _scene.add("M3D_plane2", new Model3D(*plane2));
        _scene.getModel("M3D_plane2")->setOrientation(glm::toMat4(glm::quat(glm::vec3(PI/2.0, -PI/4.0f, 0.0f))));
        _scene.getModel("M3D_plane2")->setPosition(glm::vec3(300.0f, 220.0f, -300.0f));
        _scene.getModel("M3D_plane2")->setLightingShader(lightShader);
        _scene.getModel("M3D_plane2")->setShadowCaster(false);

        /* A sample triangle */
        Asset3D *triangle = game->getRenderer()->prepareAsset3D(Procedural::Triangle(glm::vec3(-100.0f, 10.0f, 130.0f),
                                                                           glm::vec3(0.0f, 30.0f, 130.0f),
                                                                           glm::vec3(-50.0f, 80.0f, 100.0f),
                                                                           glm::vec3(1.0f, 1.0f, 1.0f)));

        _scene.add("M3D_triangle", new Model3D(*triangle));
        _scene.getModel("M3D_triangle")->setLightingShader(lightShader);

        /* Torus */
        Asset3D *torus = game->getRenderer()->prepareAsset3D(Procedural::Torus(10.0f, 4.0f, glm::vec3(1.0f, 0.0f, 0.0f), 50, 50));

        _scene.add("M3D_torus", new Model3D(*torus));
        _scene.getModel("M3D_torus")->setOrientation(glm::toMat4(glm::quat(glm::vec3(0.0f, -PI/2.0f, 0.0f))));
        _scene.getModel("M3D_torus")->rotate(glm::toMat4(glm::quat(glm::vec3(-PI/4.0f, 0.0f, 0.0f))));
        _scene.getModel("M3D_torus")->setPosition(glm::vec3(120.0f, 100.0f, 100.0f));
        _scene.getModel("M3D_torus")->setLightingShader(lightShader);

        /* Create a cube */
        Asset3D *cube = game->getRenderer()->prepareAsset3D(Procedural::Cube(150.0f, 50.0f, 100.0f, glm::vec3(0.5f, 0.3f, 1.0f), 10, 8, 2));
        _scene.add("M3D_cube", new Model3D(*cube));
        _scene.getModel("M3D_cube")->setPosition(glm::vec3(0.0f, 60.0f, 0.0f));
        _scene.getModel("M3D_cube")->setLightingShader(lightShader);

        /* Create a circle */
        Asset3D *circle = game->getRenderer()->prepareAsset3D(Procedural::Circle(100.0f, glm::vec3(0.8f, 0.9f, 0.1f), 50));

        _scene.add("M3D_circle", new Model3D(*circle));
        _scene.getModel("M3D_circle")->setPosition(glm::vec3(0.0f, 1.0f, 0.0f));
        _scene.getModel("M3D_circle")->setLightingShader(lightShader);

        /* Create a cylinder */
        Asset3D *cylinder = game->getRenderer()->prepareAsset3D(Procedural::Cylinder(20.0f, 40.0f, glm::vec3(0.2f, 1.0f, 0.4f), 50, 50));

        _scene.add("M3D_cylinder", new Model3D(*cylinder));
        _scene.getModel("M3D_cylinder")->setPosition(glm::vec3(0.0f, 90.0f, 150.0f));
        _scene.getModel("M3D_cylinder")->setLightingShader(lightShader);

        /* Create a sphere */
        Asset3D *sphere = game->getRenderer()->prepareAsset3D(Procedural::Sphere(30.0f, glm::vec3(0.4, 0.8f, 0.9f), 50, 50));
        _scene.add("M3D_sphere", new Model3D(*sphere));
        _scene.getModel("M3D_sphere")->setPosition(glm::vec3(-120.0f, 30.0f, 200.0f));
        _scene.getModel("M3D_sphere")->setLightingShader(lightShader);

        /* Create the game camera */
        _scene.add("Camera1", new Camera());
        _scene.getCamera("Camera1")->setProjection((float)_width, (float)_height, 0.1f, 1000.0f, 45.0f);

        /* And the camera motion */
        _cameraMotion.setPosition(glm::vec3(244.421936, 113.758423, 404.655487));
        _cameraMotion.rotateYaw(-30.0f);

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
        /* Apply the motion to the camera */
        _cameraMotion.applyTo(*_scene.getCamera("Camera1"));

        if (game->getRenderer()->renderScene(_scene, *_viewport) != true) {
            log("ERROR rendering scene\n");
            return false;
        }

        return true;
    }

  private:
    FlyMotion _cameraMotion;
    InputManager _inputManager;
    Scene _scene;
    float _MouseSensibility;
    float _KeyboardSensibility;
    float _InvertMouse;
    int32_t _prevX;
    int32_t _prevY;
    float _angle;
    float _sunIntensity;
    uint32_t _width, _height;
    Viewport *_viewport;
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
