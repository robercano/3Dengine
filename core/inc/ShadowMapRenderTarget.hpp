/**
 * @class	ShadowMapRenderTarget
 * @brief	A render target allows to render objects to it instead of
 *          to the main screen. The target can then be blitted onto the
 *          screen
 *
 *          The ShadowMap render target applies no anti-aliasing
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include "RenderTarget.hpp"

class ShadowMapRenderTarget : public virtual RenderTarget
{
  public:
    static ShadowMapRenderTarget *New();
    static void Delete(ShadowMapRenderTarget *target);

    virtual ~ShadowMapRenderTarget() {}
};
