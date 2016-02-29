/**
 * @class	Game
 * @brief	Main game components aggregator
 *
 * @author	Roberto Sosa Cano
 */
#include <sys/time.h>
#include "OpenGL.h" // For GLFW_KEY_ESC
#include "Game.hpp"
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

	/* Use our system renderer. This only supports OpenGL for now */
	_renderer = Renderer::GetRenderer();
	if (_renderer == NULL) {
		fprintf(stderr, "ERROR allocating renderer\n");
		WindowManager::DisposeWindowManager(_windowManager);
		_windowManager = NULL;
		return false;
	}

    /* Create a render target to allow post-processing */
    _renderTargetNOAA = NOAARenderTarget::NewNOAARenderTarget();
	if (_renderTargetNOAA == NULL) {
		fprintf(stderr, "ERROR allocating render target\n");
		WindowManager::DisposeWindowManager(_windowManager);
		_windowManager = NULL;
		return false;
	}
    _renderTargetMSAA = MSAARenderTarget::NewMSAARenderTarget();
	if (_renderTargetMSAA == NULL) {
		fprintf(stderr, "ERROR allocating render target\n");
		WindowManager::DisposeWindowManager(_windowManager);
		_windowManager = NULL;
		return false;
	}
    _renderTargetSSAA = SSAARenderTarget::NewSSAARenderTarget();
	if (_renderTargetSSAA == NULL) {
		fprintf(stderr, "ERROR allocating render target\n");
		WindowManager::DisposeWindowManager(_windowManager);
		_windowManager = NULL;
		return false;
	}

	/* Init the window manager and the render*/
	_windowManager->init();

	/* Set the window size */
    _gameName = gameName;
	_windowManager->createWindow(_gameName, _width, _height, false);
    _windowManager->getWindowSize(&_width, &_height);

	_renderer->init();	// only after creating the window
    _renderTargetNOAA->init(_width, _height);
    _renderTargetMSAA->init(_width, _height, MSAARenderTarget::getMaxSamples());
    _renderTargetSSAA->init(_width, _height, 2);

    /* Choose the render target here */
    _selectedRenderTarget = _renderTargetNOAA;
    _renderTargetName = "NOAA";

	_windowManager->setRenderer(_renderer);

    /* Setup the text console */
    glm::vec4 color(1.0, 0.5, 0.2, 1.0);
    std::string fontPath = "data/fonts/Arial.ttf";

    if (_console.init(fontPath, 14, color, _width, _height) == false) {
        printf("ERROR creating text console\n");
        return 1;
    }

	/* Register the key and mouse listener */
	std::vector<uint32_t> keys; // The keys should be read from a config file

	keys.push_back('W');
	keys.push_back('S');
	keys.push_back('A');
	keys.push_back('D');
	keys.push_back('1');
	keys.push_back('2');
	keys.push_back('3');
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
    float FPS = 0.0;
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
        if (_inputManager._keys['1']) {
            _selectedRenderTarget = _renderTargetNOAA;
            _renderTargetName = "NOAA";
        } else if (_inputManager._keys['2']) {
            _selectedRenderTarget = _renderTargetMSAA;
            _renderTargetName = "MSAA";
        } else if (_inputManager._keys['3']) {
            _selectedRenderTarget = _renderTargetSSAA;
            _renderTargetName = "SSAA";
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

		//usleep(20000);

		/* If frame is due, render it */
		double render_ms = (now.tv_sec - lastRender.tv_sec)*1000.0 + (now.tv_usec - lastRender.tv_usec)/1000.0;
		if (_unboundFPS == true || render_ms > (1000.0/_targetFPS)) {
			renders++;

            /* Render all objects */
            uint32_t i;
            for (i=0; i<_objects.size(); ++i) {
                _renderer->renderObject3D(*_objects[i], *_shaders[i],
                                          _camera->getProjection(), _camera->getView(),
                                          *_selectedRenderTarget);
            }

            _console.clear();
            _console.gprintf("Title: %s", _gameName.c_str());
            _console.gprintf("Anti-aliasing: %s", _renderTargetName.c_str());
            _console.gprintf("FPS: %.2f", FPS);

            _selectedRenderTarget->blit(0, 0, _width, _height);
            _console.blit();
        }

		if (render_ms >= (1000.0/_targetFPS)) {
			_windowManager->swapBuffers();
			gettimeofday(&lastRender, NULL);

            /* Calculate FPS */
            FPS = 1000.0*renders/render_ms;
            renders = 0;
		}
	}

    gettimeofday(&fps_end, NULL);
	fprintf(stderr, "Passes: %d, Renders: %d, Ratio: %f, FPS: %.2f\n", passes, renders, passes/(float)renders,
			renders/(fps_end.tv_sec-fps_start.tv_sec + (fps_end.tv_usec - fps_start.tv_usec)/1000000.0));
	return true;
}
