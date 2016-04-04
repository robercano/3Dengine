/**
 * @class	OpenGLToonRenderTarget
 * @brief	Render target for OpenGL. A render target allows to render objects to it
 *          instead of to the main screen. Then the target can be rendered to the main screen as
 *          a texture
 *
 *          The Toon render target applies a toon shader to the rastered
 *          image to add the typical cartoon thick border to the model
 *          edges
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include "OpenGL.h"
#include "Shader.hpp"
#include "ToonRenderTarget.hpp"
#include "OpenGLFilterRenderTarget.hpp"

#pragma warning( disable : 4250 )

class OpenGLToonRenderTarget : public ToonRenderTarget, public OpenGLFilterRenderTarget
{
	private:
		bool customInit() {
			std::string error;
			if (_shader->use("effects/toon", error) == false) {
				printf("ERROR loading shader effects/toon: %s\n", error.c_str());
				return false;
			}
			return true;
		}
		void setCustomParams(void)
		{
		}
		void unsetCustomParams(void)
        {
		}
};
