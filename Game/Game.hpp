/**
 * @class	Game
 * @brief	Main game components aggregator
 *
 * @author	Roberto Sosa Cano
 */
#ifndef __GAME_HPP__
#define __GAME_HPP__

#include <string>
#include "WindowManager.hpp"
#include "InputManager.hpp"
#include "Renderer.hpp"
#include "Camera.hpp"

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
		bool init(std::string &gameName);

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
		 * Input manager
		 */
		InputManager _inputManager;

		/**
		 * Camera
		 */
		Camera *_camera;
};

#endif
