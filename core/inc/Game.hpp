/**
 * @class	Game
 * @brief	Main game components aggregator
 *
 * @author	Roberto Sosa Cano
 */
#pragma once

#include <string>
#include "WindowManager.hpp"
#include "InputManager.hpp"
#include "Renderer.hpp"
#include "Camera.hpp"
#include "RenderTarget.hpp"
#include "TextConsole.hpp"
#include "NOAARenderTarget.hpp"
#include "MSAARenderTarget.hpp"
#include "SSAARenderTarget.hpp"

class Game
{
	public:
		/**
		 * Singleton
		 */
		static Game *GetGame(void);

		/**
		 * Singleton disposal
		 */
		static void DisposeGame();

		/**
		 * Constructor
		 */
		Game();

		/**
		 * Destructor
		 */
		~Game();

		/**
		 * Initialization function
		 *
		 * @param	gameName	Name of the game, used for the window title
		 */
		bool init(std::string &gameName, uint32_t targetFPS, bool unboundFPS);

        /**
         * Add objects to the scene. Upon addition
         * the objects are prepared for the renderer
         * and cached
         *
         * @param object  Object 3D to be added to the scene
         * @param shader  Shader associated with the object
         */
        bool addObject3D(Object3D *object, Shader *shader);

		/**
		 * Main game loop
		 */
		bool loop(void);

		/**
		 * Gets the current renderer
		 */
		Renderer *getRenderer(void) { return _renderer; }

	private:
		/**
		 * Singleton instance
		 */
		static Game *_game;

		/**
		 * Window manager
		 */
		WindowManager *_windowManager;

		/**
		 * Renderer
		 */
		Renderer *_renderer;

        /**
         * Render target
         */
        NOAARenderTarget *_renderTargetNOAA;
        MSAARenderTarget *_renderTargetMSAA;
        SSAARenderTarget *_renderTargetSSAA;

		/**
		 * Input manager
		 */
		InputManager _inputManager;

		/**
		 * Camera
		 */
		Camera *_camera;

		/**
		 * List of 3D objects and shaders to be rendered
		 */
		std::vector<RendererObject3D*> _objects;
		std::vector<Shader*> _shaders;

        std::string _gameName;

        /* Text console */
        TextConsole _console;

        uint32_t _targetFPS;
        bool _unboundFPS;
        uint32_t _width;
        uint32_t _height;
};
