/*
 * Vertex Buffer Objects demo
 *
 * @author	Roberto Sosa Cano
 */
#include <string.h>
#include "Game.hpp"
#include "Cube.hpp"
#include "Sphere.hpp"

int main(int argc, char** argv)
{
    procedural::Cube cube;
    procedural::Sphere sphere;
	Game *game = Game::GetGame();

	std::string title("Cube Test");
	game->init(title);

	/* Request a new shader */
	Shader *shader = Renderer::GetRenderer()->getShader();

	/* Basic shaders with only position and color attributes, with no camera */
	std::string error;
	if (shader->loadVertexShader("Shaders/mvp.vert", error) == false) {
		printf("ERROR compiling vertex shader: %s\n", error.c_str());
		return false;
	}
	if (shader->loadFragmentShader("Shaders/color.frag", error) == false) {
		printf("ERROR compiling fragment shader: %s\n", error.c_str());
		return false;
	}
	if (shader->linkProgram(error) == false) {
		printf("ERROR linking shader: %s\n", error.c_str());
		return false;
	}

    /* Add the shader to the geometry */
    cube.addShader(shader);

	/* Add the geometry */
	game->getRenderer()->addObject(&cube);

    /* Start */
	game->loop();
	return 0;
}
