/**
 * @class	LightingShader
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include "Shader.hpp"
#include "Light.hpp"
#include "Material.hpp"

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
		virtual void setLight(uint32_t numLight, Light &light) = 0;
		virtual void setMaterial(Material &material) = 0;
};
