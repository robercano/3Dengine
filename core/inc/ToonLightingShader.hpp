/**
 * @class	ToonLightingShader
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include "LightingShader.hpp"

/* virtual inheritance is used here because on one hand we want
 * ToonLightingShader to be an instantiable LightingShader
 * and on the other hand because the implementation dependant
 * <API>LightingShader will inherit from LightingShader as well:
 *
 *               LightingShader
 *                    /   \
 *                   /     \
 *   ToonLightingShader   <API>LightingShader
 *                   \      /
 *                    \    /
 *              <API>ToonLightingShader
 */
class ToonLightingShader : public virtual LightingShader
{
  public:
    static ToonLightingShader *New();
    static void Delete(ToonLightingShader *target);
};
