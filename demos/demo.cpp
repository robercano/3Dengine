/*
 * Vertex Buffer Objects demo
 *
 * @author	Roberto Sosa Cano
 */
#include <string.h>
#include "Game.hpp"
#include "OBJFormat.hpp"
#include "Shader.hpp"
#include "TrueTypeFont.hpp"
#include "FontRenderer.hpp"

int main(int argc, char** argv)
{
    OBJFormat lego;

	Game *game = Game::GetGame();

	std::string title("OpenGL Test");
	game->init(title, 60, true);

	/* Request a new shader */
	Shader *shader = Renderer::GetRenderer()->newShader();

    /* Setup the font renderer */
    TrueTypeFont *font = TrueTypeFont::NewFont();

    if (font->init("/Library/Fonts/Courier New.ttf", 20) == false) {
        printf("ERROR loading font\n");
        return 1;
    }

    FontRenderer *fontRenderer = FontRenderer::GetFontRenderer();
    if (fontRenderer == NULL) {
        printf("ERROR getting font renderer\n");
        return 1;
    }

    fontRenderer->setFont(font);

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
    std::string legoobj = "data/objects/deadpool.obj";

    if (lego.load(legoobj) == false) {
        printf("ERROR loading OBJ file\n");
        return 1;
    }

	/* Add the geometry */
	game->addObject3D(&lego, shader);

    /* Start */
	game->loop();
	return 0;
}
