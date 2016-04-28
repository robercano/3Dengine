#include <stdio.h>
#include "Game.hpp"
#include "OBJFormat.hpp"
#include "OpenGL.h"
#include "BlinnPhongShader.hpp"
#include "NOAARenderTarget.hpp"
#include "MSAARenderTarget.hpp"
#include "SSAARenderTarget.hpp"
#include "FXAARenderTarget.hpp"
#include "FXAA2RenderTarget.hpp"
#include "Camera.hpp"
#include "FlyMotion.hpp"

#define PI 3.14159265358979323846

class AntiaAliasingDemo : public GameHandler
{
    public:
        AntiaAliasingDemo() {
            _MouseSensibility = 10.0;
			_InvertMouse = 1.0;
			_prevX = 0xFFFFFF;
			_prevY = 0xFFFFFF;
        }

        bool handleInit(Game *game)
        {
			Light *light = new Light(glm::vec3(5.0, 5.0, 5.0),
									 glm::vec3(5.0, 5.0, 5.0),
									 glm::vec3(5.0, 5.0, 5.0),
									 glm::vec3(50.0, 100.0, 50.0));
            _lights.push_back(light);

            game->getWindowManager()->getWindowSize(&_width, &_height);

			/* Create a render target to allow post-processing */
			_renderTargetNOAA = NOAARenderTarget::New();
			if (_renderTargetNOAA == NULL) {
				fprintf(stderr, "ERROR allocating render target\n");
				return false;
			}
			_renderTargetFXAA = FXAARenderTarget::New();
			if (_renderTargetFXAA == NULL) {
				fprintf(stderr, "ERROR allocating render target\n");
				return false;
			}
			_renderTargetFXAA2 = FXAA2RenderTarget::New();
			if (_renderTargetFXAA2 == NULL) {
				fprintf(stderr, "ERROR allocating render target\n");
				return false;
			}

			_renderTargetNOAA->init(_width/2, _height);
			_renderTargetFXAA->init(_width/2, _height);
			_renderTargetFXAA2->init(_width/2, _height);

			/* Choose the render target here */
			_selectedTargetLeft = _renderTargetNOAA;
			_selectedTargetRight = _renderTargetNOAA;
			_renderTargetName = "NOAA Left / NOAA Right";

            /* Register the key and mouse listener */
			std::vector<uint32_t> keys; // The keys should be read from a config file

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

            /* Create a Blinn-phong shader for the geometry */
            _blinnPhongShader = BlinnPhongShader::New();
			if (_blinnPhongShader->init() != true) {
				printf("ERROR initializing blinn-phong shader\n");
				return false;
			}

            /* Load the geometry */
            std::string meshPath = "data/objects/deadpool";

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
            _camera.setProjection(45, (float)_width/2.0, (float)_height, 0.1f, 1000.0f);
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
                _cameraMotion.forward(0.1f*(float)elapsedMs);
            }
            if (_inputManager._keys['S']) {
                _cameraMotion.forward(-0.1f*(float)elapsedMs);
            }
            if (_inputManager._keys['A']) {
                _cameraMotion.right(-0.1f*(float)elapsedMs);
            }
            if (_inputManager._keys['D']) {
                _cameraMotion.right(0.1f*(float)elapsedMs);
            }
            if (_inputManager._keys['R']) {
                game->resetStats();
            }
			if (_inputManager._keys['F']) {
				_selectedTargetLeft = _renderTargetFXAA;
				_selectedTargetRight = _renderTargetFXAA2;
				_renderTargetName = "FXAA Left / FXAA2 Right";
			}
			if (_inputManager._keys['N']) {
				_selectedTargetLeft = _renderTargetNOAA;
				_selectedTargetRight = _renderTargetNOAA;
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

            /* Render all objects */
			_selectedTargetLeft->clear();
			_selectedTargetRight->clear();
            game->getRenderer()->renderModel3D(*_model3D, _camera, *_blinnPhongShader, _lights, 0.0, *_selectedTargetLeft);
            game->getRenderer()->renderModel3D(*_model3D, _camera, *_blinnPhongShader, _lights, 0.0, *_selectedTargetRight);

            _selectedTargetLeft->blit(0, 0, _width/2, _height);
            _selectedTargetRight->blit(_width/2, 0, _width/2, _height);

            game->getTextConsole()->gprintf("N=NOAA, F=FXAA\n");
            game->getTextConsole()->gprintf("%s\n", _renderTargetName.c_str());
            return true;
        }

    private:
        Camera             _camera;
		FlyMotion          _cameraMotion;
        RendererModel3D    *_model3D;
        BlinnPhongShader   *_blinnPhongShader;
		NOAARenderTarget   *_renderTargetNOAA;
		FXAARenderTarget   *_renderTargetFXAA;
		FXAA2RenderTarget  *_renderTargetFXAA2;
		RenderTarget       *_selectedTargetLeft;
		RenderTarget       *_selectedTargetRight;
		std::string         _renderTargetName;
        std::vector<Light*> _lights;
		InputManager        _inputManager;

        float _MouseSensibility;
		float _InvertMouse;
		int32_t _prevX;
		int32_t	_prevY;
        uint32_t _width;
        uint32_t _height;
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
#ifdef _WIN32
    game->setWindowSize(800, 600, false);
#else
    game->setWindowSize(1440, 900, true);
#endif
    game->setFPS(60);

    if (game->init() == false) {
        fprintf(stderr, "ERROR initializing game\n");
        return 1;
    }

    game->loop();
    return 0;
}
