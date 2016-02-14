/*
 * Vertex Buffer Objects demo
 *
 * @author	Roberto Sosa Cano
 */
#include <string.h>
#include "Game.hpp"
#include "Cube.hpp"

int main(int argc, char** argv)
{
	Game *game = Game::GetGame();

	std::string title("Cube Test");
	game->init(title);

	/* Add the geometry */
    procedural::Cube cube;
	game->getRenderer()->addObject(&cube);

	game->loop();
	return 0;
}
