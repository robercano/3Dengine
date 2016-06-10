/**
 * @class	FBRenderTarget
 * @brief	A render target allows to render objects to it instead of
 *          to the main screen. The target can then be blitted onto the
 *          screen
 *
 *          The FB render target applies no anti-aliasing
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */

#include "FBRenderTarget.hpp"
#include "OpenGLFBRenderTarget.hpp"

FBRenderTarget *FBRenderTarget::New(void) { return new OpenGLFBRenderTarget(); }
void FBRenderTarget::Delete(FBRenderTarget *target) { delete target; }
