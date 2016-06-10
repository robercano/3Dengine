/**
 * @class	SSAARenderTarget
 * @brief	A render target allows to render objects to it instead of
 *          to the main screen. The target can then be blitted onto the
 *          screen
 *
 *          The SSAA render target applies no super sampling antialising
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */

#include "SSAARenderTarget.hpp"
#include "OpenGLSSAARenderTarget.hpp"

SSAARenderTarget *SSAARenderTarget::New(void) { return new OpenGLSSAARenderTarget(); }
void SSAARenderTarget::Delete(SSAARenderTarget *target) { delete target; }
