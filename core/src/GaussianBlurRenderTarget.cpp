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

#include "GaussianBlurRenderTarget.hpp"
#include "OpenGLGaussianBlurRenderTarget.hpp"

GaussianBlurRenderTarget *GaussianBlurRenderTarget::New(void) { return new OpenGLGaussianBlurRenderTarget(); }
void GaussianBlurRenderTarget::Delete(GaussianBlurRenderTarget *target) { delete target; }
