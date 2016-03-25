/**
 * @class	FXAA/FXAA2 (custom implementation) comparison
 * @brief	Demo to compare Timothy Lottes FXAA implementation against mine
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include <string>
#include <vector>
#include "WindowManager.hpp"
#include "InputManager.hpp"
#include "TimeManager.hpp"
#include "Renderer.hpp"
#include "FlyCamera.hpp"
#include "RenderTarget.hpp"
#include "TextConsole.hpp"

#define TARGET_FPS 60

class Game;

class GameHandler
{
    public:
        virtual bool handleInit(Game *_game) = 0;
        virtual bool handleTick(Game *_game, double elapsedMs) = 0;
        virtual bool handleRender(Game *game) = 0;
};

class Game {
    public:
        Game(const std::string &name) : _name(name), _gameHandler(NULL),
		                                _minRenderFrameMs(10000000.0f), _maxRenderFrameMs(0.0f) {}
        void setHandler(GameHandler *gameHandler) { _gameHandler = gameHandler; }

        void setWindowSize(uint32_t width, uint32_t height, bool fullscreen = false);
        void setFPS(uint32_t FPS, bool unbound = false);
		void resetStats();
        bool init();
        bool loop();

		WindowManager *getWindowManager() { return _windowManager; }
		Renderer *getRenderer() { return _renderer; }
		TextConsole *getTextConsole() { return &_console; }

    private:
        GameHandler   *_gameHandler;
        WindowManager *_windowManager;
        Renderer      *_renderer;
        InputManager   _inputManager;
        TimeManager   *_timer;
        //    Camera *_camera;
        TextConsole    _console;
        std::string    _name;

        uint32_t _targetFPS;
		float _minRenderFrameMs;
		float _maxRenderFrameMs;
		float _avgRenderMs[120];
        bool _unboundFPS;
        bool _fullscreen;
        uint32_t _width;
        uint32_t _height;
};
