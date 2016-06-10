/**
 * @class	SSAARenderTarget
 * @brief	A render target allows to render objects to it instead of
 *          to the main screen. The target can then be blitted onto the
 *          screen
 *
 *          The SSAA render target applies no anti-aliasing
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include "RenderTarget.hpp"

class SSAARenderTarget : public RenderTarget
{
  public:
    static SSAARenderTarget *New();
    static void Delete(SSAARenderTarget *target);
    virtual ~SSAARenderTarget() {}
};
