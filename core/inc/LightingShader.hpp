/**
 * @class	LightingShader
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include "DirectLight.hpp"
#include "Material.hpp"
#include "PointLight.hpp"
#include "Shader.hpp"
#include "SpotLight.hpp"

/* virtual inheritance is used here because on one hand we want
 * LightingShader to be an instantiable Shader for generic
 * lighting shaders, and on the other hand because the
 * implementation dependant <API>LightingShader will inherit
 * from <API>Shader that inherits itself from Shader:
 *
 *                    Shader
 *                    /   \
 *                   /     \
 *       LightingShader    <API>Shader
 *                   \      /
 *                    \    /
 *              <API>LightingShader
 */
class LightingShader : public virtual Shader
{
  public:
    virtual uint32_t getMaxLights() = 0;
    virtual void setDirectLight(DirectLight &directLight) = 0;
    virtual void setPointLight(uint32_t numLight, PointLight &pointLight) = 0;
    virtual void setSpotLight(uint32_t numLight, SpotLight &pointLight) = 0;
    virtual void setMaterial(Material &material) = 0;
};
