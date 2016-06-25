/**
 * @class	FBRenderTarget
 * @brief	A render target that acts as a frame buffer. When enabled all other
 *          blittings are performed onto this render target. Then this render
 *          target can be blitted onto the main frame buffer
 *
 *          This render target performs no post-processing
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include "RenderTarget.hpp"

class FBRenderTarget : public RenderTarget
{
  public:
    /**
     * Allocates a new FBRenderTarget of the specific underlaying API
     *
     * To free the returned memory FBRenderTarget::Delete() must be used
     *
     * @return A pointer to the newly allocated FBRenderTarget
     */
    static FBRenderTarget *New();

    /**
     * Frees the render target previously allocated by FBRenderTarget::New()
     *
     * @param target  Pointer to the allocated FBRenderTarget
     */
    static void Delete(FBRenderTarget *target);

    /**
     * Destructor
     */
    virtual ~FBRenderTarget() {}
};
