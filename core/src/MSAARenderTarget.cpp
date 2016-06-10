/**
 * @class	MSAARenderTarget
 * @brief	A render target allows to render objects to it instead of
 *          to the main screen. The target can then be blitted onto the
 *          screen
 *
 *          The MSAA render target applies multi-sampling antialising
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */

#include "MSAARenderTarget.hpp"
#include "OpenGLMSAARenderTarget.hpp"

uint32_t MSAARenderTarget::getMaxSamples(void) { return OpenGLMSAARenderTarget::getMaxSamples(); }
MSAARenderTarget *MSAARenderTarget::New(void) { return new OpenGLMSAARenderTarget(); }
void MSAARenderTarget::Delete(MSAARenderTarget *target) { delete target; }
