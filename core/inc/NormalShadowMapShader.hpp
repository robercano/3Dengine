/**
 * @class	NormalShadowMapShader
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include "Shader.hpp"

class NormalShadowMapShader : public virtual Shader
{
	public:
        static NormalShadowMapShader *New();
        static void Delete(NormalShadowMapShader *target);
};
