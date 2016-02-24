/**
 * @class	Game
 * @brief	Main game components aggregator
 *
 * @author	Roberto Sosa Cano
 */
#include <sys/time.h>
#include "OpenGL.h"
#include "Game.hpp"
#include "OpenGLRenderer.hpp"
#include "OpenGLNOAARenderTarget.hpp"
#include "OpenGLMSAARenderTarget.hpp"
#include "OpenGLSSAARenderTarget.hpp"
#include "WalkingCamera.hpp"

Game *Game::_game = NULL;

Game *Game::GetGame(void)
{
	if (_game == NULL) {
		_game = new Game();
	}
	return _game;
}

void Game::DisposeGame()
{
	delete _game;
	_game = NULL;
}

Game::Game() : _windowManager(NULL), _renderer(NULL), _camera(NULL)
{
}

Game::~Game()
{
	WindowManager::DisposeWindowManager(_windowManager);
	delete _renderer;
}

bool Game::init(std::string &gameName, uint32_t targetFPS, bool unboundFPS)
{
	/* TODO: Get the settings from a config file */
    _width = 1440;
    _height = 900;

    _targetFPS = targetFPS;
    _unboundFPS = unboundFPS;

	_windowManager = WindowManager::GetWindowManager(WindowManager::WINDOW_MANAGER_GLFW);
	if (_windowManager == NULL) {
		fprintf(stderr, "ERROR creating new window manager\n");
		return false;
	}

	/* Use our Open GL renderer */
	_renderer = new OpenGLRenderer();
	if (_renderer == NULL) {
		fprintf(stderr, "ERROR allocating renderer\n");
		WindowManager::DisposeWindowManager(_windowManager);
		_windowManager = NULL;
		return false;
	}

    /* Create a render target to allow post-processing */
    OpenGLNOAARenderTarget *renderTargetNOAA = new OpenGLNOAARenderTarget();
	if (renderTargetNOAA == NULL) {
		fprintf(stderr, "ERROR allocating render target\n");
        delete _renderer;
		WindowManager::DisposeWindowManager(_windowManager);
		_windowManager = NULL;
		return false;
	}
    OpenGLMSAARenderTarget *renderTargetMSAA = new OpenGLMSAARenderTarget();
	if (renderTargetMSAA == NULL) {
		fprintf(stderr, "ERROR allocating render target\n");
        delete _renderer;
		WindowManager::DisposeWindowManager(_windowManager);
		_windowManager = NULL;
		return false;
	}
    OpenGLSSAARenderTarget *renderTargetSSAA = new OpenGLSSAARenderTarget();
	if (renderTargetSSAA == NULL) {
		fprintf(stderr, "ERROR allocating render target\n");
        delete _renderer;
		WindowManager::DisposeWindowManager(_windowManager);
		_windowManager = NULL;
		return false;
	}

	/* Init the window manager and the render*/
	_windowManager->init();

	/* Set the window size */
	_windowManager->createWindow(gameName, _width, _height, false);
    _windowManager->getWindowSize(&_width, &_height);

	_renderer->init();	// only after creating the window
    renderTargetNOAA->init(_width, _height);
    //renderTargetMSAA->init(_width/2, _height, OpenGLMSAARenderTarget::getMaxSamples());
    //renderTargetSSAA->init(_width/2, _height, 8);

    _renderTargetNOAA = renderTargetNOAA;
    //_renderTargetMSAA = renderTargetMSAA;
    //_renderTargetSSAA = renderTargetSSAA;

	_windowManager->setRenderer(_renderer);

	/* Register the key and mouse listener */
	std::vector<uint32_t> keys; // The keys should be read from a config file

	keys.push_back('W');
	keys.push_back('S');
	keys.push_back('A');
	keys.push_back('D');
	keys.push_back(GLFW_KEY_ESC);

	_windowManager->getKeyManager()->registerListener(_inputManager, keys);
	_windowManager->getMouseManager()->registerListener(_inputManager);

	/* Create the game camera */
	//_camera = new WalkingCamera();
	_camera = new Camera();
	_camera->setProjection(45, _width/(float)_height, 0.1, 1000.0);
    glm::vec4 pos( 220, 135, -1, 1);
    _camera->setPosition(pos);
    _camera->rotateYaw(-90);

    return true;
}

bool Game::addObject3D(Object3D *object, Shader *shader)
{
    RendererObject3D *renderObject = _renderer->prepareObject3D(*object);

    _objects.push_back(renderObject);
    _shaders.push_back(shader);
    return true;
}

bool Game::loop(void)
{
	const float MouseSensibility = 10.0;
	const float InvertMouse = 1.0;
	static int32_t _prevX = 0xFFFFFF, _prevY = 0xFFFFFF;
	struct timeval lastRender, now, previous;
	gettimeofday(&now, NULL);
	lastRender = previous = now;

	struct timeval fps_start, fps_end;
	gettimeofday(&fps_start, NULL);

	uint32_t passes = 0, renders = 0;
	while (true)
	{
		/* Read input */
		_windowManager->poll();
		if (_inputManager._keys[GLFW_KEY_ESC]) {
			break;
		}

		/* Get elapsed time */
		previous = now;
		gettimeofday(&now, NULL);
		double elapsed_ms = (now.tv_sec - previous.tv_sec)*1000.0 + (now.tv_usec - previous.tv_usec)/1000.0;
		passes++;

		/* Dispatch input to geometry */
		if (_inputManager._keys['W']) {
			_camera->forward(0.1*elapsed_ms);
		} else if (_inputManager._keys['S']) {
			_camera->forward(-0.1*elapsed_ms);
		} else if (_inputManager._keys['A']) {
			_camera->right(-0.1*elapsed_ms);
		} else if (_inputManager._keys['D']) {
			_camera->right(0.1*elapsed_ms);
		}

		if (_prevX == 0xFFFFFF) {
			_prevX = _inputManager._xMouse;
		}
		if (_prevY == 0xFFFFFF) {
			_prevY = _inputManager._yMouse;
		}
		int32_t diffMouseX = _inputManager._xMouse - _prevX;
		int32_t diffMouseY = InvertMouse*(_inputManager._yMouse - _prevY);

		if (diffMouseX) {
			_camera->rotateYaw(MouseSensibility*M_PI*diffMouseX/_width);
		}
		if (diffMouseY) {
			_camera->rotatePitch(MouseSensibility*M_PI*diffMouseY/_height);
		}

		_prevX = _inputManager._xMouse;
		_prevY = _inputManager._yMouse;

		//usleep(2000);

		/* If frame is due, render it */
		double render_ms = (now.tv_sec - lastRender.tv_sec)*1000.0 + (now.tv_usec - lastRender.tv_usec)/1000.0;
		if (_unboundFPS == true || render_ms > (1000.0/_targetFPS)) {
			renders++;

            /* Render all objects */
            uint32_t i;
            for (i=0; i<_objects.size(); ++i) {
                _renderer->renderObject3D(*_objects[i], *_shaders[i],
                                          _camera->getProjection(), _camera->getView(),
                                          *_renderTargetNOAA);
#if 0
                _renderer->renderObject3D(*_objects[i], *_shaders[i],
                                          _camera->getProjection(), _camera->getView(),
                                          *_renderTargetMSAA);
                _renderer->renderObject3D(*_objects[i], *_shaders[i],
                                          _camera->getProjection(), _camera->getView(),
                                          *_renderTargetSSAA);
#endif
            }

            _renderTargetNOAA->blit(0, 0, _width, _height);
            //_renderTargetMSAA->blit(_width/2, 0, _width, _height);
            //_renderTargetSSAA->blit(_width/2, 0, _width, _height);
        }

		if (render_ms > (1000.0/_targetFPS)) {
			_windowManager->swapBuffers();
			gettimeofday(&lastRender, NULL);
		}
	}

	gettimeofday(&fps_end, NULL);
	fprintf(stderr, "Passes: %d, Renders: %d, Ratio: %f, FPS: %.2f\n", passes, renders, passes/(float)renders,
			renders/(fps_end.tv_sec-fps_start.tv_sec + (fps_end.tv_usec - fps_start.tv_usec)/1000000.0));
	return true;
}
