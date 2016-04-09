#include <stdio.h>
#include "Game.hpp"
#include "OBJFormat.hpp"
#include "OpenGL.h"
#include "BlinnPhongShader.hpp"
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
			_angle = 0.0;
        }

        bool handleInit(Game *game)
        {
            Light *light1 = new Light(glm::vec3(5.0, 5.0, 5.0),
                    glm::vec3(5.0, 5.0, 5.0),
                    glm::vec3(5.0, 5.0, 5.0),
                    glm::vec3(50.0, 100.0, 50.0));
            Light *light2 = new Light(glm::vec3(5.0, 3.0, 3.0),
                    glm::vec3(5.0, 3.0, 3.0),
                    glm::vec3(5.0, 3.0, 3.0),
                    glm::vec3(-50.0, 100.0, 50.0));
            Light *light3 = new Light(glm::vec3(1.0, 1.0, 3.0),
                    glm::vec3(1.0, 1.0, 3.0),
                    glm::vec3(1.0, 1.0, 3.0),
                    glm::vec3(50.0, 200.0, 100.0));

            _lights.push_back(light1);
            _lights.push_back(light2);
            _lights.push_back(light3);

            game->getWindowManager()->getWindowSize(&_width, &_height);

			/* Create a render target to allow post-processing */
			_renderTargetFXAA2 = FXAA2RenderTarget::New();
			if (_renderTargetFXAA2 == NULL) {
				fprintf(stderr, "ERROR allocating render target\n");
				return false;
			}

			_renderTargetFXAA2->init(_width, _height);

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
            _blinnPhongShader = BlinnPhongShader::New();
			if (_blinnPhongShader->init() == false) {
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

			/* Move the lights */
			_angle += (float)(2*PI*elapsedMs/5000.0);
			if (_angle > 2*PI) {
				_angle -= (float)(2*PI);
			}

			_lights[0]->setPosition(glm::vec3(50.0*glm::sin(-_angle), 100.0, 50.0*glm::cos(-_angle)));
			_lights[1]->setPosition(glm::vec3(-50.0*glm::sin(_angle), 100.0, 500.0*glm::cos(_angle)));
			_lights[2]->setPosition(glm::vec3(100.0*glm::sin(_angle), 150.0, 100.0*glm::cos(_angle)));

            return true;
        }

        bool handleRender(Game *game)
        {
			/* Apply the motion to the camera */
			_cameraMotion.applyTo(_camera);

            /* Render all objects */
			_renderTargetFXAA2->clear();
            game->getRenderer()->renderModel3D(*_model3D, _camera, *_blinnPhongShader, _lights, 0.0, *_renderTargetFXAA2);

            _renderTargetFXAA2->blit(0, 0, _width, _height);
            return true;
        }

    private:
        Camera             _camera;
		FlyMotion          _cameraMotion;
        RendererModel3D    *_model3D;
        BlinnPhongShader   *_blinnPhongShader;
		FXAA2RenderTarget  *_renderTargetFXAA2;
		std::string         _renderTargetName;
        std::vector<Light*> _lights;
		InputManager        _inputManager;

        float _MouseSensibility;
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
    game->setWindowSize(800, 600, false);
    game->setFPS(60);

    if (game->init() == false) {
        fprintf(stderr, "ERROR initializing game\n");
        return 1;
    }

    game->loop();
    return 0;
}
