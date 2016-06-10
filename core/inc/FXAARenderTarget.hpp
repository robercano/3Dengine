/**
 * @class	FXAARenderTarget
 * @brief	A render target allows to render objects to it instead of
 *          to the main screen. The target can then be blitted onto the
 *          screen
 *
 *          The FXAA render target applies Fast Aproximate Anti-Aliasing
 *          from Timothy Lottes paper at Nvidia
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include "RenderTarget.hpp"

class FXAARenderTarget : public virtual RenderTarget
{
  public:
    static FXAARenderTarget *New();
    static void Delete(FXAARenderTarget *target);

    virtual ~FXAARenderTarget() {}
};
