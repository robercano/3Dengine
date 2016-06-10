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

#include "ShadowMapRenderTarget.hpp"
#include "OpenGLShadowMapRenderTarget.hpp"

ShadowMapRenderTarget *ShadowMapRenderTarget::New(void) { return new OpenGLShadowMapRenderTarget(); }
void ShadowMapRenderTarget::Delete(ShadowMapRenderTarget *target) { delete target; }
