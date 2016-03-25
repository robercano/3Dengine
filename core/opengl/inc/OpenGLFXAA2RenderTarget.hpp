/**
 * @class	OpenGLFXAA2RenderTarget
 * @brief	Render target for OpenGL. A render target allows to render objects to it
 *          instead of to the main screen. Then the target can be rendered to the main screen as
 *          a texture
 *
 *          The FXAA2 render target applies Fast Approximate Anti-Aliasing from
 *          Timothy Lottes paper t Nvidia
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include "OpenGL.h"
#include "Shader.hpp"
#include "FXAA2RenderTarget.hpp"
#include "OpenGLFilterRenderTarget.hpp"

class OpenGLFXAA2RenderTarget : public FXAA2RenderTarget, public OpenGLFilterRenderTarget
{
	private:
		bool customInit() {
			std::string error;
			if (_shader->use("anti-aliasing/fxaa", error) == false) {
				printf("ERROR loading shader: %s\n", error.c_str());
				return false;
			}
			return true;
		}
		void setCustomParams(void)
		{
			glm::vec2 rpcFrame(1.0f/_width, 1.0f/_height);
			_shader->setUniformVec2("f_rpcFrame", rpcFrame);
			GL( glDisable(GL_BLEND) );
			GL( glDisable(GL_DEPTH_TEST) );
		}
		void unsetCustomParams(void) {
			glEnable(GL_DEPTH_TEST);
		}
};
