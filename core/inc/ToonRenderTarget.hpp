/**
 * @class	ToonRenderTarget
 * @brief	A render target allows to render objects to it instead of
 *          to the main screen. The target can then be blitted onto the
 *          screen
 *
 *          The Toon render target applies a toon shader to the rastered
 *          image to add the typical cartoon thick border to the model
 *          edges
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
