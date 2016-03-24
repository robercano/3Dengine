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

class GameHandler
{
    public:
        virtual bool handleInit(WindowManager &windowManager, Renderer &renderer, std::vector<uint32_t> &keys) = 0;
        virtual bool handleKeyboard(InputManager &inputManager, double inputElapsedMs) = 0;
        virtual bool handleMouse(int32_t diffMouseX, int32_t diffMouseY) = 0;
        virtual bool handleRender(Renderer &renderer, TextConsole &console) = 0;
};

class Game {
    public:
        Game(const std::string &name) : _name(name), _gameHandler(NULL) {}
        void setHandler(GameHandler *gameHandler) { _gameHandler = gameHandler; }

        void setWindowSize(uint32_t width, uint32_t height, bool fullscreen = false);
        void setFPS(uint32_t FPS, bool unbound = false);
        bool init();
        bool loop();

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
        bool _unboundFPS;
        bool _fullscreen;
        uint32_t _width;
        uint32_t _height;
};
