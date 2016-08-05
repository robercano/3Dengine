/**
 * @class	HDRRenderTarget
 * @brief	A render target allows to render objects to it instead of
 *          to the main screen. The target can then be blitted onto the
 *          screen
 *
 *          The HDR render target applies no anti-aliasing
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */

#include "HDRRenderTarget.hpp"
#include "OpenGLHDRRenderTarget.hpp"

HDRRenderTarget *HDRRenderTarget::New(void) { return new OpenGLHDRRenderTarget(); }
void HDRRenderTarget::Delete(HDRRenderTarget *target) { delete target; }
