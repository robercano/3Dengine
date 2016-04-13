/**
 * @class	FXAA2RenderTarget
 * @brief	A render target allows to render objects to it instead of
 *          to the main screen. The target can then be blitted onto the
 *          screen
 *
 *          The FXAA render target applies Fast Aproximate Anti-Aliasing
 *          as implemented following the Nvidia paper
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include "RenderTarget.hpp"

class FXAA2RenderTarget : public virtual RenderTarget
{
	public:
        static FXAA2RenderTarget *New();
        static void Delete(FXAA2RenderTarget *target);

        virtual ~FXAA2RenderTarget() {}
};
