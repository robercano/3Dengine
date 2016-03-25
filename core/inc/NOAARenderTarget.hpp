/**
 * @class	NOAARenderTarget
 * @brief	A render target allows to render objects to it instead of
 *          to the main screen. The target can then be blitted onto the
 *          screen
 *
 *          The NOAA render target applies no anti-aliasing
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include "RenderTarget.hpp"

class NOAARenderTarget : public virtual RenderTarget
{
	public:
        static NOAARenderTarget *New();
        static void Delete(NOAARenderTarget *target);
};
