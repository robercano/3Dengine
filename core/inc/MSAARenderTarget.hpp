/**
 * @class	MSAARenderTarget
 * @brief	A render target allows to render objects to it instead of
 *          to the main screen. The target can then be blitted onto the
 *          screen
 *
 *          The MSAA render target applies multi-sampling aliasing
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include "RenderTarget.hpp"

class MSAARenderTarget : public RenderTarget
{
  public:
    static MSAARenderTarget *New();
    static void Delete(MSAARenderTarget *target);
    static uint32_t getMaxSamples();

    virtual ~MSAARenderTarget() {}
};
