/**
 * @class	OpenGLBlinnPhongShader
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include "OpenGL.h"
#include "OpenGLShader.hpp"
#include "LightingShader.hpp"
#include "OpenGLShaderPointLight.hpp"
//#include "OpenGLShaderDirectLight.hpp"

#pragma warning( disable : 4250 )

/* Even though OpenGLLightingShader is the end of the diamond for
 * the following diagram:
 *
 *                    Shader
 *                    /    \
 *                   /      \
 *       LightingShader   <API>Shader
 *                   \      /
 *                    \    /
 *              <API>LightingShader
 *
 * and it should not need virtual inheritance anymore, it happens to be
 * involved in another diamond, which relates it to any specialization
 * of the LightingShader, like BlinnPhongShader, thus it must inherit
 * virtually from LightingShader.
 */
class OpenGLLightingShader : public virtual LightingShader, public OpenGLShader
{
	public:
		static const uint32_t MAX_LIGHTS=10;

		bool init()
		{
			attach();

			_material.init(0);
			if (_material.prepareForShader(_programID) != true) {
				printf("ERROR preparing material for blinnphong_reflection shader\n");
				return false;
			}

			for (uint32_t i=0; i<MAX_LIGHTS; ++i) {
				_pointLights[i].init(i+1, i);
				if (_pointLights[i].prepareForShader(_programID) != true) {
					printf("ERROR preparing point light %d for blinnphong_reflection shader\n", i);
					return false;
				}
			}

			detach();
			return true;
		}

		uint32_t getMaxLights()
		{
			return MAX_LIGHTS;
		}

/*		void setDirectLight(DirectLight &directLight)
		{
			_directLight.copyLight(directLight);
		}*/

		void setPointLight(uint32_t numLight, PointLight &pointLight)
		{
			if (numLight >= MAX_LIGHTS) {
				printf("ERROR light number %d higher than max. %d in blinnphong_reflection shader\n", numLight, MAX_LIGHTS);
				return;
			}
			_pointLights[numLight].copyLight(pointLight);
		}

		void setMaterial(Material &material)
		{
			_material.copyMaterial(material);
		}

	private:
		OpenGLShaderMaterial    _material;
//		OpenGLShaderDirectLight _directLight;
		OpenGLShaderPointLight  _pointLights[MAX_LIGHTS];
//		OpenGLShaderSpotLight _spotLights[MAX_LIGHTS];
};
