/**
 * @class	ToonRenderTarget
 * @brief	A render target allows to render objects to it instead of
 *          to the main screen. The target can then be blitted onto the
 *          screen
 *
 *          The Toon render target applies no anti-aliasing
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */

#include "ToonRenderTarget.hpp"
#include "OpenGLToonRenderTarget.hpp"

ToonRenderTarget *ToonRenderTarget::New(void) { return new OpenGLToonRenderTarget(); }
void ToonRenderTarget::Delete(ToonRenderTarget *target) { delete target; }
