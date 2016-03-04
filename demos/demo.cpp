/*
 * Vertex Buffer Objects demo
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#include <string.h>
#include "Game.hpp"
#include "OBJFormat.hpp"
#include "Shader.hpp"
#include "ShaderMaterial.hpp"

int main(int argc, char** argv)
{
    OBJFormat obj3D;

	Game *game = Game::GetGame();

	std::string title("OpenGL Test");
	game->init(title, 60, true);

	/* Request a new shader */
	Shader *shader = Renderer::GetRenderer()->newShader();

	/* Basic shaders with only position and color attributes, with no camera */
	std::string error;
	if (shader->loadVertexShader("data/shaders/lighting/blinnphong_reflection.vert", error) == false) {
		printf("ERROR compiling vertex shader: %s\n", error.c_str());
		return 1;
	}
	if (shader->loadFragmentShader("data/shaders/lighting/blinnphong_reflection.frag", error) == false) {
		printf("ERROR compiling fragment shader: %s\n", error.c_str());
		return 1;
	}
	if (shader->linkProgram(error) == false) {
		printf("ERROR linking shader: %s\n", error.c_str());
        return 1;
    }

    /* Load the geometry */
    std::string meshPath = "data/objects/deadpool";

    if (obj3D.load(meshPath) == false) {
        printf("ERROR loading OBJ file\n");
        return 1;
    }

	/* Add the geometry */
	game->addObject3D(&obj3D, shader);

    /* Start */
	game->loop();
	return 0;
}
