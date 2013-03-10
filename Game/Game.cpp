/**
 * @class	Game
 * @brief	Main game components aggregator
 *
 * @author	Roberto Sosa Cano
 */
#include <GL/glfw.h>
#include <sys/time.h>
#include "Game.hpp"
#include "OpenGLRenderer.hpp"
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

bool Game::init(std::string &gameName)
{
	/* TODO: Get the settings from a config file */

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

	/* Init the window manager and the render*/
	_windowManager->init();

	/* Set the window size */
	_windowManager->createWindow(gameName, 1440, 900, false);

	_renderer->init();	// only after creating the window
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
	_camera = new WalkingCamera();
	_camera->setProjection(45, 1440.0/900.0, 0.1, 100.0);
}

bool Game::loop(void)
{
	const uint32_t fps=60;
	const float MouseSensibility = 5.0;
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
			_camera->forward(0.01*elapsed_ms);
		} else if (_inputManager._keys['S']) {
			_camera->forward(-0.01*elapsed_ms);
		} else if (_inputManager._keys['A']) {
			_camera->right(-0.01*elapsed_ms);
		} else if (_inputManager._keys['D']) {
			_camera->right(0.01*elapsed_ms);
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
			_camera->rotateYaw(MouseSensibility*M_PI*diffMouseX/1440.0);
		}
		if (diffMouseY) {
			_camera->rotatePitch(MouseSensibility*M_PI*diffMouseY/900.0);
		}

		_prevX = _inputManager._xMouse;
		_prevY = _inputManager._yMouse;

		//usleep(2000);

		/* If frame is due, render it */
		double render_ms = (now.tv_sec - lastRender.tv_sec)*1000.0 + (now.tv_usec - lastRender.tv_usec)/1000.0;
		if (render_ms > (1000.0/fps)) {
			renders++;
			_renderer->render(_camera->getProjection(), _camera->getView());
			_windowManager->swapBuffers();
			gettimeofday(&lastRender, NULL);
		}
	}

	gettimeofday(&fps_end, NULL);
	fprintf(stderr, "Passes: %d, Renders: %d, Ratio: %f, FPS: %.2f\n", passes, renders, passes/(float)renders,
			renders/(fps_end.tv_sec-fps_start.tv_sec + (fps_end.tv_usec - fps_start.tv_usec)/1000000.0));
	return true;
}
