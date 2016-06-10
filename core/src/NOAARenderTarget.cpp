/**
 * @class	NOAARenderTarget
 * @brief	A render target allows to render objects to it instead of
 *          to the main screen. The target can then be blitted onto the
 *          screen
 *
 *          The NOAA render target applies no anti-aliasing
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */

#include "NOAARenderTarget.hpp"
#include "OpenGLNOAARenderTarget.hpp"

NOAARenderTarget *NOAARenderTarget::New(void) { return new OpenGLNOAARenderTarget(); }
void NOAARenderTarget::Delete(NOAARenderTarget *target) { delete target; }
