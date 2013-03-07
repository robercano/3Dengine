/*
 * Vertex Buffer Objects demo
 *
 * @author	Roberto Sosa Cano
 */
#include <string.h>
#include "Game.hpp"
#include "ObjectLib.hpp"

int main(int argc, char** argv)
{
	Game *game = Game::GetGame();

	std::string title("Cube Test");
	game->init(title);

	/* Add the geometry */
	ColorCube cube;
	game->getRenderer()->addObject(&cube);

	game->loop();
	return 0;
}
