/**
 * @class	Game
 * @brief	Main game components aggregator
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#include <sys/time.h>
#include <unistd.h>
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
    _renderTargetMSAA->init(_width, _height, MSAARenderTarget::getMaxSamples() < 4 ?
                                             MSAARenderTarget::getMaxSamples() : 4);
    _renderTargetSSAA->init(_width, _height, 4);

    /* Choose the render target here */
    _selectedRenderTarget = _renderTargetNOAA;
    _renderTargetName = "NOAA";

	_windowManager->setRenderer(_renderer);

    /* Setup the text console */
    glm::vec4 color(1.0, 0.5, 0.2, 1.0);
    std::string fontPath = "data/fonts/Arial.ttf";

    if (_console.init(fontPath, 14, _width, _height) == false) {
        printf("ERROR creating text console\n");
        return 1;
    }

    _console.setForegroundColor(1.0, 0.5, 0.2, 1.0);
    _console.setBackgroundColor(0.0, 0.0, 0.0, 0.0);

	/* Register the key and mouse listener */
	std::vector<uint32_t> keys; // The keys should be read from a config file

	keys.push_back('W');
	keys.push_back('S');
	keys.push_back('A');
	keys.push_back('D');
	keys.push_back('1');
	keys.push_back('2');
	keys.push_back('3');
	keys.push_back('R');
	keys.push_back(GLFW_KEY_ESC);

	_windowManager->getKeyManager()->registerListener(_inputManager, keys);
	_windowManager->getMouseManager()->registerListener(_inputManager);

	/* Create the game camera */
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
    uint32_t i;
	const float MouseSensibility = 10.0;
	const float InvertMouse = 1.0;
    float FPS = 0.0;
	static int32_t _prevX = 0xFFFFFF, _prevY = 0xFFFFFF;
	struct timeval renderNow, renderPrevious;
    struct timeval inputNow, inputPrevious;
    bool resetStats = false;

#define NUM_AVG_FPS 500
    float avgFPS[NUM_AVG_FPS] = {0};
    uint32_t avgFPSIdx = 0;
    float minFPS = 10000000.0f;
    float maxFPS = 0.0f;
    float totalAvgFPS = 0;
	uint32_t  renders=0;

	gettimeofday(&inputNow, NULL);
	gettimeofday(&renderNow, NULL);

	while (true)
	{
		/* Read input */
		_windowManager->poll();
		if (_inputManager._keys[GLFW_KEY_ESC]) {
			break;
		}

		/* Get elapsed time */
		inputPrevious = inputNow;
		gettimeofday(&inputNow, NULL);
		double inputElapsedMs = (inputNow.tv_sec - inputPrevious.tv_sec)*1000.0 +
                                (inputNow.tv_usec - inputPrevious.tv_usec)/1000.0;

		/* Dispatch input to geometry */
		if (_inputManager._keys['W']) {
			_camera->forward(0.1*inputElapsedMs);
		} else if (_inputManager._keys['S']) {
			_camera->forward(-0.1*inputElapsedMs);
		} else if (_inputManager._keys['A']) {
			_camera->right(-0.1*inputElapsedMs);
		} else if (_inputManager._keys['D']) {
			_camera->right(0.1*inputElapsedMs);
		}
        if (_inputManager._keys['1']) {
            _selectedRenderTarget = _renderTargetNOAA;
            _renderTargetName = "NOAA";
            resetStats = true;
        } else if (_inputManager._keys['2']) {
            _selectedRenderTarget = _renderTargetMSAA;
            _renderTargetName = "MSAA";
            resetStats = true;
        } else if (_inputManager._keys['3']) {
            _selectedRenderTarget = _renderTargetSSAA;
            _renderTargetName = "SSAA";
            resetStats = true;
        }
        if (_inputManager._keys['R']) {
            resetStats = true;
        }
        if (resetStats) {
            minFPS = 1000000.0f;
            maxFPS = 0.0f;
            for (i=0; i<NUM_AVG_FPS; ++i) {
                avgFPS[i] = FPS;
            }
            resetStats = false;
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

        /* Render all objects */
        for (i=0; i<_objects.size(); ++i) {
            _renderer->renderObject3D(*_objects[i], *_shaders[i],
                    _camera->getProjection(), _camera->getView(),
                    *_selectedRenderTarget);
        }

        _console.clear();
        _console.gprintf("Title: %s\n", _gameName.c_str());
        _console.gprintf("Anti-aliasing: %s\n", _renderTargetName.c_str());
        _console.gprintf("Avg. FPS: %.2f\n", totalAvgFPS);
        _console.gprintf("Min. FPS: %.2f\n", minFPS);
        _console.gprintf("Max. FPS: %.2f\n", maxFPS);

        _selectedRenderTarget->blit(0, 0, _width, _height);
        _console.blit();

        /* Flush all operations so we can have a good measure
         * of the time it takes to render the scene. Otherwise this
         * will only happen when swapBuffers() is called. In a proper
         * engine you want to accumulate operations and have a measurement
         * of how long the card will take to render the scene, so you can
         * maximimize the number of operations you can issue before running
         * out of time:
         *
         *  |------------------------------ framerate time ----------------------------|
         *  |__________________________________________________________________________|
         *  |- process input -|- prepare scene -|- send rendering commands -|- render -|
         *  |--------------------------------------------------------------------------|
         *
         *  Obviously not at scale. Rendering takes much longer than the rest.
         */
        _renderer->flush();
        renders++;

		/* If frame is due, blit it */
		gettimeofday(&renderNow, NULL);
		double renderElapsedMs = (renderNow.tv_sec - renderPrevious.tv_sec)*1000.0 +
                                 (renderNow.tv_usec - renderPrevious.tv_usec)/1000.0;
		if (renderElapsedMs >= (1000.0/_targetFPS)) {
			_windowManager->swapBuffers();
            renderPrevious = renderNow;

            /* Calculate FPS now */
            FPS = 1000.0*renders/renderElapsedMs;

            if (FPS > maxFPS) {
                maxFPS = FPS;
            }
            if (FPS < minFPS) {
                minFPS = FPS;
            }

            avgFPS[avgFPSIdx] = FPS;
            avgFPSIdx = (avgFPSIdx + 1)%NUM_AVG_FPS;

            /* Calculate the average FPS */
            for (i=0; i<NUM_AVG_FPS; ++i) {
                totalAvgFPS += avgFPS[i];
            }
            totalAvgFPS /= NUM_AVG_FPS;

            renders = 0;
        }
	}

    fprintf(stderr, "Average FPS: %.2f\n", totalAvgFPS);
    fprintf(stderr, "Minimum FPS: %.2f\n", minFPS);
    fprintf(stderr, "Maximum FPS: %.2f\n", maxFPS);
	return true;
}
