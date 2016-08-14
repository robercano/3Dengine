/**
 * @class	GaussianBlurRenderTarget
 * @brief	Render target that performs a 1-pixel Gaussian Blur
 *          in vertical and horizontal direction. To achieve a higher
 *          level of blur 2 GaussianBlurRenderTarget can be used in a
 *          ping-pong fashion to blit the target onto the other target
 *          until the required blur level is achieved
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include "RenderTarget.hpp"

class GaussianBlurRenderTarget : public virtual RenderTarget
{
  public:
      /**
       * Allocates a new render target using the underlaying rendering API
       *
       * @return Allocated HDR render target or NULL if OOM
       */
    static GaussianBlurRenderTarget *New();

    /**
     * Deletes an Gaussian blur render target previously allocated with New()
     *
     * @param The render target pointer
     */
    static void Delete(GaussianBlurRenderTarget *target);

    /**
     * Destructor
     */
    virtual ~GaussianBlurRenderTarget() {}

    /**
     * Sets the direction of the blur
     *
     * @param horizontal  True if blur is horizontal, false for vertical
     */
    void setHorizontal(bool flag) { _horizontal = flag; }

    /**
     * Gets the direction of the blur
     *
     * @return True if blur is horizontal, false for vertical
     */
    bool getHorizontal() { return _horizontal; }

  protected:
    /**
     * Constructor
     */
    GaussianBlurRenderTarget() : _horizontal(true) {}

    bool _horizontal;  /**< Direction of the blur */
};
