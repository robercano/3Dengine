#include <stdio.h>
#include "Game.hpp"
#include "OBJFormat.hpp"
#include "OpenGL.h"
#include "Shader.hpp"
#include "NOAARenderTarget.hpp"
#include "MSAARenderTarget.hpp"
#include "SSAARenderTarget.hpp"
#include "FXAARenderTarget.hpp"
#include "FXAA2RenderTarget.hpp"

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
			_renderTargetNOAA = NOAARenderTarget::New();
			if (_renderTargetNOAA == NULL) {
				fprintf(stderr, "ERROR allocating render target\n");
				return false;
			}
			_renderTargetMSAA = MSAARenderTarget::New();
			if (_renderTargetMSAA == NULL) {
				fprintf(stderr, "ERROR allocating render target\n");
				return false;
			}
			_renderTargetSSAA = SSAARenderTarget::New();
			if (_renderTargetSSAA == NULL) {
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

			_renderTargetNOAA->init(_width, _height);
			_renderTargetMSAA->init(_width, _height, MSAARenderTarget::getMaxSamples() < 4 ?
					                MSAARenderTarget::getMaxSamples() : 4);
			_renderTargetSSAA->init(_width, _height, 4);
			_renderTargetFXAA->init(_width, _height);
			_renderTargetFXAA2->init(_width, _height);

			/* Choose the render target here */
			_selectedRenderTarget = _renderTargetNOAA;
			_renderTargetName = "NOAA";

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
            _shader = Shader::New();

            std::string error;
            if (_shader->loadVertexShader("data/shaders/lighting/blinnphong_reflection.vert", error) == false) {
                printf("ERROR compiling vertex shader: %s\n", error.c_str());
                return false;
            }
            if (_shader->loadFragmentShader("data/shaders/lighting/blinnphong_reflection.frag", error) == false) {
                printf("ERROR 00compiling fragment shader: %s\n", error.c_str());
                return false;
            }
            if (_shader->linkProgram(error) == false) {
                printf("ERROR linking shader: %s\n", error.c_str());
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
            _model3D = game->getRenderer()->prepareObject3D(obj3D);

            /* Create the game camera */
            _camera = new FlyCamera();
            _camera->setProjection(45, _width/(float)_height, 0.1, 1000.0);
            _camera->setPosition( glm::vec4(220, 135, -1, 1) );
            _camera->rotateYaw(-90);

            return true;
        }

        bool handleTick(Game *game, double elapsedMs)
        {
			/* Keyboard */
            if (_inputManager._keys[GLFW_KEY_ESCAPE]) {
                return false;
            }
            if (_inputManager._keys['W']) {
                _camera->forward(0.1*elapsedMs);
            }
            if (_inputManager._keys['S']) {
                _camera->forward(-0.1*elapsedMs);
            }
            if (_inputManager._keys['A']) {
                _camera->right(-0.1*elapsedMs);
            }
            if (_inputManager._keys['D']) {
                _camera->right(0.1*elapsedMs);
            }
            if (_inputManager._keys['R']) {
                game->resetStats();
            }
			if (_inputManager._keys['1']) {
				_selectedRenderTarget = _renderTargetNOAA;
				_renderTargetName = "NOAA";
                game->resetStats();
			} else if (_inputManager._keys['2']) {
				_selectedRenderTarget = _renderTargetMSAA;
				_renderTargetName = "MSAA";
                game->resetStats();
			} else if (_inputManager._keys['3']) {
				_selectedRenderTarget = _renderTargetSSAA;
				_renderTargetName = "SSAA";
                game->resetStats();
			} else if (_inputManager._keys['4']) {
				_selectedRenderTarget = _renderTargetFXAA;
				_renderTargetName = "FXAA";
                game->resetStats();
			} else if (_inputManager._keys['5']) {
				_selectedRenderTarget = _renderTargetFXAA2;
				_renderTargetName = "FXAA2";
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
			int32_t diffMouseY = _InvertMouse*(_inputManager._yMouse - _prevY);

            if (diffMouseX) {
                _camera->rotateYaw(_MouseSensibility*M_PI*diffMouseX/_width);
            }
            if (diffMouseY) {
                _camera->rotatePitch(_MouseSensibility*M_PI*diffMouseY/_height);
            }

			_prevX = _inputManager._xMouse;
			_prevY = _inputManager._yMouse;

            return true;
        }

        bool handleRender(Game *game)
        {
            /* Render all objects */
            game->getRenderer()->renderObject3D(*_model3D, *_shader, _lights, 0.0,
                                                _camera->getProjection(), _camera->getView(),
                                                *_selectedRenderTarget);

            _selectedRenderTarget->blit(0, 0, _width, _height);

            game->getTextConsole()->gprintf("1=NOAA, 2=MSAA, 3=SSAA, 4=FXAA, 5=FXAA2\n");
            game->getTextConsole()->gprintf("Anti-aliasing: %s\n", _renderTargetName.c_str());
            return true;
        }

    private:
        Camera             *_camera;
        RendererObject3D   *_model3D;
        Shader             *_shader;
		NOAARenderTarget   *_renderTargetNOAA;
		MSAARenderTarget   *_renderTargetMSAA;
		SSAARenderTarget   *_renderTargetSSAA;
		FXAARenderTarget   *_renderTargetFXAA;
		FXAA2RenderTarget  *_renderTargetFXAA2;
		RenderTarget       *_selectedRenderTarget;
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
    game->setWindowSize(2560, 1440, true);
    game->setFPS(60);

    if (game->init() == false) {
        fprintf(stderr, "ERROR initializing game\n");
        return 1;
    }

    game->loop();
    return 0;
}
