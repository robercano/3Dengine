/**
 * @class	OpenGLBlinnPhongShader
 * @author	Roberto Cano (http://www.robertocano.es)
 */
#pragma once

#include "OpenGL.h"
#include "OpenGLShader.hpp"
#include "LightingShader.hpp"

/* Even though OpenGLLightingShader is the end of the diamond for
 * the following diagram:
 *
 *                    Shader
 *                    /   \
 *                   /     \
 *       LightingShader    <API>Shader
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
				_lights[i].init(i+1, i);
				if (_lights[i].prepareForShader(_programID) != true) {
					printf("ERROR preparing light %d for blinnphong_reflection shader\n", i);
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

		void setLight(uint32_t numLight, Light &light)
		{
			if (numLight >= MAX_LIGHTS) {
				printf("ERROR light number %d higher than max. %d in blinnphong_reflection shader\n", numLight, MAX_LIGHTS);
				return;
			}
			_lights[numLight].copyLight(light);
		}

		void setMaterial(Material &material)
		{
			_material.copyMaterial(material);
		}

	private:
		OpenGLShaderMaterial _material;
		OpenGLShaderLight _lights[MAX_LIGHTS];
};
