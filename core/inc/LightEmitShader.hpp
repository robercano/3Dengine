/**
 * @class	LightEmitShader
 * @brief   Shader to provide flat ilumination
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include "LightingShader.hpp"

/* virtual inheritance is used here because on one hand we want
 * LightEmitShader to be an instantiable LightingShader
 * and on the other hand because the implementation dependant
 * <API>LightingShader will inherit from LightingShader as well:
 *
 *               LightingShader
 *                    /   \
 *                   /     \
 *     LightEmitShader   <API>LightingShader
 *                   \      /
 *                    \    /
 *              <API>LightEmitShader
 */
class LightEmitShader : public virtual LightingShader
{
  public:
    /**
     * Allocates a new LightEmitShader of the specific underlaying API
     *
     * To free the returned memory LightEmitShader::Delete() must be used
     *
     * @return A pointer to the newly allocated LightEmitShader
     */
    static LightEmitShader *New();

    /**
     * Frees the shader previously allocated by LightEmitShader::New()
     *
     * @param target  Pointer to the allocated LightEmitShader
     */
    static void Delete(LightEmitShader *target);
};
