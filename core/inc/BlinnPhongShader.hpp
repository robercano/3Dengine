/**
 * @class	BlinnPhongShader
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
        static BlinnPhongShader *New();
        static void Delete(BlinnPhongShader *target);
};
