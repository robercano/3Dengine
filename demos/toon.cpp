#include <stdio.h>
#include "Game.hpp"
#include "OBJFormat.hpp"
#include "OpenGL.h"
#include "ToonLightingShader.hpp"
#include "BlinnPhongShader.hpp"
#include "FXAA2RenderTarget.hpp"
#include "ToonRenderTarget.hpp"
#include "Camera.hpp"
#include "FlyMotion.hpp"

#define PI 3.14159265358979323846

class AntiaAliasingDemo : public GameHandler
{
    public:
        AntiaAliasingDemo() {
            _MouseSensibility = 10.0f;
			_KeyboardSensibility = 0.1f;
			_InvertMouse = 1.0;
			_prevX = 0xFFFFFF;
			_prevY = 0xFFFFFF;
			_angle = 0.0;
        }

        bool handleInit(Game *game)
        {
			Light *light1 = new Light(glm::vec3(3.5, 3.5, 4.5),
									 glm::vec3(3.5, 3.5, 4.5),
									 glm::vec3(3.5, 3.5, 4.5),
									 glm::vec3(75.0, 300.0, 150.0));

            _lights.push_back(light1);

            game->getWindowManager()->getWindowSize(&_width, &_height);

			/* Create a render target to allow post-processing */
			_renderTargetFXAA2 = FXAA2RenderTarget::New();
			if (_renderTargetFXAA2 == NULL) {
				fprintf(stderr, "ERROR allocating fxaa2 render target\n");
				return false;
			}

			_renderTargetFXAA2->init(_width, _height);
			_renderTargetFXAA2->setClearColor(1.0, 1.0, 1.0, 0.0);

			/* Create the toon target to add a border */
			_renderTargetToon = ToonRenderTarget::New();
			if (_renderTargetToon == NULL) {
				fprintf(stderr, "ERROR allocating toon render target\n");
				return false;
			}

			_renderTargetToon->init(_width, _height);
			_renderTargetToon->setClearColor(1.0, 1.0, 1.0, 1.0);
			_renderTargetToon->setBorderColor(glm::vec4(0.0, 0.0, 0.0, 1.0));

            /* Register the key and mouse listener */
			std::vector<uint32_t> keys; // The keys should be read from a config file

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
            _shaderToonLight  = ToonLightingShader::New();
			if (_shaderToonLight->init() == false) {
				printf("ERROR initializing toon lighting shader\n");
				return false;
			}
            _shaderBlinnLight = BlinnPhongShader::New();
			if (_shaderBlinnLight->init() == false) {
				printf("ERROR initializing blinn-phong shader\n");
				return false;
			}

            _shaderLight = _shaderBlinnLight;
            _renderTarget = _renderTargetFXAA2;
            _current = "Normal";

            /* Load the geometry */
            std::string meshPath = "data/objects/daxter";

            OBJFormat obj3D;
            if (obj3D.load(meshPath) == false) {
                printf("ERROR loading OBJ file\n");
                return false;
            }

            /* Wrap the geometry for the renderer, this typically generates any
             * renderer API specific structures and uploads data to the graphics card */
            _model3D = game->getRenderer()->prepareModel3D(obj3D);
            _model3D->setScaleFactor(glm::vec3(100.0f, 100.0f, 100.0f));

            /* Create the game camera */
            _camera.setProjection(45, _width/(float)_height, 0.1f, 1000.0f);
			_cameraMotion.setPosition( glm::vec3(150.0f, 100.0f, 150.0f) );
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
                _cameraMotion.forward(_KeyboardSensibility*(float)elapsedMs);
            }
            if (_inputManager._keys['S']) {
                _cameraMotion.forward(-_KeyboardSensibility*(float)elapsedMs);
            }
            if (_inputManager._keys['A']) {
                _cameraMotion.right(-_KeyboardSensibility*(float)elapsedMs);
            }
            if (_inputManager._keys['D']) {
                _cameraMotion.right(_KeyboardSensibility*(float)elapsedMs);
            }
            if (_inputManager._keys['R']) {
                game->resetStats();
            }
            if (_inputManager._keys['1']) {
                _shaderLight = _shaderBlinnLight;
                _renderTarget = _renderTargetFXAA2;
                _current = "Normal";
            }
            if (_inputManager._keys['2']) {
                _shaderLight = _shaderToonLight;
                _renderTarget = _renderTargetFXAA2;
                _current = "ToonLight";
            }
            if (_inputManager._keys['3']) {
                _shaderLight = _shaderToonLight;
                _renderTarget = _renderTargetToon;
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
			int32_t diffMouseY = (int32_t)(_InvertMouse*(_inputManager._yMouse - _prevY));

            if (diffMouseX) {
                _cameraMotion.rotateYaw((float)(_MouseSensibility*PI*diffMouseX/_width));
            }
            if (diffMouseY) {
                _cameraMotion.rotatePitch((float)(_MouseSensibility*PI*diffMouseY/_height));
            }

			_prevX = _inputManager._xMouse;
			_prevY = _inputManager._yMouse;

            return true;
        }

        bool handleRender(Game *game)
        {
			/* Apply the motion to the camera */
			_cameraMotion.applyTo(_camera);

            _renderTarget->clear();

            /* Render all objects */
            game->getRenderer()->renderModel3D(*_model3D, _camera, *_shaderLight, _lights, 0.2f, *_renderTarget);
            game->getTextConsole()->gprintf("Current = %s\n", _current.c_str());
            game->getTextConsole()->gprintf("1=Normal, 2=ToonLight, 3=ToonLight+Filter\n");

            _renderTarget->blit();
            return true;
        }

    private:
        Camera             _camera;
		FlyMotion          _cameraMotion;
        RendererModel3D    *_model3D;
        LightingShader     *_shaderLight;
        BlinnPhongShader   *_shaderBlinnLight;
        ToonLightingShader *_shaderToonLight;
        RenderTarget       *_renderTarget;
		FXAA2RenderTarget  *_renderTargetFXAA2;
		ToonRenderTarget   *_renderTargetToon;
		std::string         _renderTargetName;
        std::vector<Light*> _lights;
		InputManager        _inputManager;
        std::string         _current;

        float _MouseSensibility;
        float _KeyboardSensibility;
		float _InvertMouse;
		int32_t _prevX;
		int32_t	_prevY;
        uint32_t _width;
        uint32_t _height;
		float _angle;
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
    game->setWindowSize(2560, 1440, true);
    game->setFPS(60);

    if (game->init() == false) {
        fprintf(stderr, "ERROR initializing game\n");
        return 1;
    }

    game->loop();
    return 0;
}
