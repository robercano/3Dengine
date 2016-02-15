/*
 * Vertex Buffer Objects demo
 *
 * @author	Roberto Sosa Cano
 */
#include <string.h>
#include "Game.hpp"
#include "OBJFormat.hpp"

int main(int argc, char** argv)
{
    OBJFormat lego;

	Game *game = Game::GetGame();

	std::string title("Cube Test");
	game->init(title);

	/* Request a new shader */
	Shader *shader = Renderer::GetRenderer()->getShader();

	/* Basic shaders with only position and color attributes, with no camera */
	std::string error;
	if (shader->loadVertexShader("Shaders/phong/mvp.vert", error) == false) {
		printf("ERROR compiling vertex shader: %s\n", error.c_str());
		return 1;
	}
	if (shader->loadFragmentShader("Shaders/phong/phong.frag", error) == false) {
		printf("ERROR compiling fragment shader: %s\n", error.c_str());
		return 1;
	}
	if (shader->linkProgram(error) == false) {
		printf("ERROR linking shader: %s\n", error.c_str());
		return 1;
	}

    /* Load the geometry */
    std::string legoobj = "Samples/lego.obj";

    if (lego.load(legoobj) == false) {
        printf("ERROR loading OBJ file\n");
        return 1;
    }

    /* Add the shader to the geometry */
    lego.addShader(shader);

	/* Add the geometry */
	game->getRenderer()->addObject(&lego);

    /* Start */
	game->loop();
	return 0;
}
