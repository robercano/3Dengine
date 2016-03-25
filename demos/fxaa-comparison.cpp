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
            _shader = Shader::New();

            std::string error;
            if (_shader->loadVertexShader("data/shaders/lighting/blinnphong_reflection.vert", error) == false) {
                printf("ERROR compiling vertex shader: %s\n", error.c_str());
                return false;
            }
            if (_shader->loadFragmentShader("data/shaders/lighting/blinnphong_reflection.frag", error) == false) {
                printf("ERROR compiling fragment shader: %s\n", error.c_str());
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
            _camera->setProjection(45, _width/(2.0*(float)_height), 0.1, 1000.0);
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
                                                *_selectedTargetLeft);
            game->getRenderer()->renderObject3D(*_model3D, *_shader, _lights, 0.0,
                                                _camera->getProjection(), _camera->getView(),
                                                *_selectedTargetRight);

            _selectedTargetLeft->blit(0, 0, _width/2, _height);
            _selectedTargetRight->blit(_width/2, 0, _width/2, _height);

            game->getTextConsole()->gprintf("N=NOAA, F=FXAA\n");
            game->getTextConsole()->gprintf("%s\n", _renderTargetName.c_str());
            return true;
        }

    private:
        Camera             *_camera;
        RendererObject3D   *_model3D;
        Shader             *_shader;
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
    game->setWindowSize(2560, 1440, true);
    game->setFPS(60);

    if (game->init() == false) {
        fprintf(stderr, "ERROR initializing game\n");
        return 1;
    }

    game->loop();
    return 0;
}
