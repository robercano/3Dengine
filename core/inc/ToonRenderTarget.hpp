/**
 * @class	ToonRenderTarget
 * @brief	A render target allows to render objects to it instead of
 *          to the main screen. The target can then be blitted onto the
 *          screen
 *
 *			The Toon render target post processed a model to add cartoon
 *			borders to the edges
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include "RenderTarget.hpp"

class ToonRenderTarget : public virtual RenderTarget
{
	public:
        static ToonRenderTarget *New();
        static void Delete(ToonRenderTarget *target);
};
