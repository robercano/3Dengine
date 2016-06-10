/**
 * @class	OpenGLNOAARenderTarget
 * @brief	Render target for OpenGL. A render target allows to render objects to it
 *          instead of to the main screen. Then the target can be rendered to the main screen as
 *          a texture
 *
 *          The NOAA render target applies no anti-aliasing
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include "OpenGL.h"
#include "Shader.hpp"
#include "NOAARenderTarget.hpp"
#include "OpenGLFilterRenderTarget.hpp"

#pragma warning( disable : 4250 )

class OpenGLNOAARenderTarget : public NOAARenderTarget, public OpenGLFilterRenderTarget
{
	public:
		bool customInit() {
			std::string error;
			if (_shader->use("anti-aliasing/noaa", error) == false) {
				printf("ERROR loading shader anti-aliasing/noaa: %s\n", error.c_str());
				return false;
			}
			return true;
		}
		void setCustomParams(void) {
			__( glDisable(GL_DEPTH_TEST) );
			__( glEnable(GL_BLEND) );
			__( glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) );
		}
		void unsetCustomParams(void) {
			__( glDisable(GL_BLEND) );
			__( glEnable(GL_DEPTH_TEST) );
		}
};
