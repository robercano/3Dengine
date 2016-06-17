/**
 * @class	BlinnPhongShader
 * @brief   Shader to provide Blinn-Phong ilumination. This shader is of
 *          type LightingShader indicating it needs a specific set of
 *          parameters to work, that are defined in LightingShader
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include "LightingShader.hpp"

/* virtual inheritance is used here because on one hand we want
 * BlinnPhongShader to be an instantiable LightingShader
 * and on the other hand because the implementation dependant
 * <API>LightingShader will inherit from LightingShader as well:
 *
 *               LightingShader
 *                    /   \
 *                   /     \
 *     BlinnPhongShader   <API>LightingShader
 *                   \      /
 *                    \    /
 *              <API>BlinnPhongShader
 */
class BlinnPhongShader : public virtual LightingShader
{
  public:
    /**
     * Allocates a new BlinnPhongShader of the specific underlaying API
     *
     * To free the returned memory BlinnPhongShader::Delete() must be used
     *
     * @return A pointer to the newly allocated BlinnPhongShader
     */
    static BlinnPhongShader *New();

    /**
     * Frees the shader previously allocated by BlinnPhongShader::New()
     *
     * @param target  Pointer to the allocated BlinnPhongShader
     */
    static void Delete(BlinnPhongShader *target);
};
