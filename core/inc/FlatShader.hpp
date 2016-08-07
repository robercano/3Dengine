/**
 * @class	FlatShader
 * @brief   Shader to provide flat ilumination
 *
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include "LightingShader.hpp"

/* virtual inheritance is used here because on one hand we want
 * FlatShader to be an instantiable LightingShader
 * and on the other hand because the implementation dependant
 * <API>LightingShader will inherit from LightingShader as well:
 *
 *               LightingShader
 *                    /   \
 *                   /     \
 *     FlatShader   <API>LightingShader
 *                   \      /
 *                    \    /
 *              <API>FlatShader
 */
class FlatShader : public virtual LightingShader
{
  public:
    /**
     * Allocates a new FlatShader of the specific underlaying API
     *
     * To free the returned memory FlatShader::Delete() must be used
     *
     * @return A pointer to the newly allocated FlatShader
     */
    static FlatShader *New();

    /**
     * Frees the shader previously allocated by FlatShader::New()
     *
     * @param target  Pointer to the allocated FlatShader
     */
    static void Delete(FlatShader *target);

    /**
     * Disables lighting for this object, it only renders
     * the diffuse map with the original intensity
     */
    void disableLighting() { _disableLight = 1; }

    /**
     * Enables lighting for this object, rendering takes
     * into account provided lights
     */
    void enableLighting() { _disableLight = 0; }

  protected:
    /**
     * Constructor
     */
    FlatShader() : _disableLight(0) {}

    uint32_t _disableLight; /**< Disables light calculations */
};
