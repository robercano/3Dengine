/* 
    Toon lighting shader. This shader does 2 things:

        * Creates a discreet lighting where groups of
          values are moved to fixed intensity values, thus
          creating a sort of flat lighting
        * Adds a black border to the mesh by checking the dot
          product of the camera-fragment vector with the normal
          of the fragment (all in view-space) and if the value is
          less than some constant, emitting a black fragment

    @author Roberto Cano
*/
#version 400 core

#define GLSL_VERSION 440
#define MAX_LIGHTS 4

/* Effect strength of the shader. It ranges from -1.0..1.0 and
   determines how much border is added to the model */
#define EFFECT_STRENGTH -1.1

/* Enables the discretization of the intensity values to
   achieve a toonified ilumination */
#define ENABLE_TOON_LIGHT 1

/* Direct light definition */
layout (std140) uniform DirectLight {
	vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
} u_DirectLight;

uniform sampler2DShadow u_shadowMapDirectLight;
in vec4 io_shadowCoordDirectLight;
uniform uint u_numDirectLights; /* 0 or 1 */

/* Light definition */
layout (std140) uniform PointLight {
    vec3  position;
	float attenuation;
	float cutoff;
    vec3  ambient;
    vec3  diffuse;
    vec3  specular;
} u_PointLight[MAX_LIGHTS];

uniform sampler2DShadow u_shadowMapPointLight[MAX_LIGHTS];
in vec4 io_shadowCoordPointLight[MAX_LIGHTS];
uniform uint u_numPointLights;

/* Spotlight definition */
layout (std140) uniform SpotLight {
    vec3  position;
    vec3  direction;
	float coneAngle;
	float conePenumbra;
	float attenuation;
	float cutoff;
    vec3  ambient;
    vec3  diffuse;
    vec3  specular;
} u_SpotLight[MAX_LIGHTS];

uniform sampler2DShadow u_shadowMapSpotLight[MAX_LIGHTS];
in vec4 io_shadowCoordSpotLight[MAX_LIGHTS];
uniform uint u_numSpotLights;

/* Global scene ambient constant */
uniform float u_ambientK;

/* Material definition for this geometry */
layout (std140) uniform Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float alpha;
    float shininess;
} u_material;

/* Texture and transformation matrices */
uniform sampler2D u_diffuseMap;

/* Input from vertex shader */
in vec3 io_fragVertex;
in vec3 io_fragNormal;
in vec2 io_fragUVCoord;
in vec3 io_viewNormal;
in vec3 io_viewVertex;

/* Output of this shader */
out vec4 o_color;

float sRGB2Linear(float c) {
    if (c <= 0.04045) {
        return c/12.92;
    }
    return pow((c + 0.055)/1.055, 2.4);
}

float toonify(float intensity)
{
#if ENABLE_TOON_LIGHT
	if (intensity < 0.0001) return 0.0;
	else if (intensity < 0.25) return 0.125;
	else if (intensity < 0.5) return 0.375;
	else if (intensity < 0.75) return 0.625;
	else if (intensity < 0.99) return 0.875;
	else return 1.0;
#else
	return intensity;
#endif
}

#define _ProcessPointLight(color, shadow, n, V)                                           \
{                                                                                         \
    if (n < u_numPointLights) {                                                           \
		vec3 unnormL = u_PointLight[n].position - io_fragVertex;                          \
		float distanceToLight = length(unnormL);                                          \
                                                                                          \
		if (distanceToLight <= u_PointLight[n].cutoff) {                                  \
			float attenuation = shadow / (1.0 + u_PointLight[n].attenuation * pow(length(unnormL), 2));\
                                                                                          \
			/* Light vector to fragment */                                                \
			vec3 L = normalize(unnormL);                                                  \
																						  \
			/* Normalized half vector for Blinn-Phong */                                  \
			vec3 H = normalize(L + V);                                                    \
																						  \
			/* Ambient + Diffuse + Specular */                                            \
			float Ia = toonify(clamp(u_ambientK, 0.0, 1.0));                                       \
			float Id = toonify(clamp(dot(L, io_fragNormal), 0.0, 1.0));                            \
			float Is = toonify(clamp(pow(dot(io_fragNormal, H), u_material.shininess), 0.0, 1.0)); \
																						  \
			vec3 colorAmbient  = u_PointLight[n].ambient*u_material.ambient*Ia;           \
			vec3 colorDiffuse  = u_PointLight[n].diffuse*u_material.diffuse*Id;           \
			vec3 colorSpecular = u_PointLight[n].specular*u_material.specular*Is;         \
																						  \
			if (dot(L, io_fragNormal) <= 0) {                                             \
				colorSpecular = vec3(0.0);                                                \
			}                                                                             \
																						  \
			/* Accumulate color components */                                             \
			color += colorAmbient + attenuation*(colorDiffuse + colorSpecular);           \
		}                                                                                 \
    }                                                                                     \
}

#define _ProcessSpotLight(color, shadow, n, V)                                                       \
{                                                                                                    \
    if (n < u_numSpotLights) {                                                                       \
		vec3 unnormL = u_SpotLight[n].position - io_fragVertex;                                      \
		float distanceToLight = length(unnormL);                                                     \
                                                                                                     \
		if (distanceToLight <= u_SpotLight[n].cutoff) {                                              \
			/* Light vector to fragment */                                                           \
			vec3 L = normalize(unnormL);                                                             \
                                                                                                     \
			float lightToSurfaceAngle = degrees(acos(dot(-L, normalize(u_SpotLight[n].direction)))); \
                                                                                                     \
            if (lightToSurfaceAngle <= u_SpotLight[n].coneAngle) {                                   \
                                                                                                     \
			    float attenuation = shadow / (1.0 + u_SpotLight[n].attenuation * pow(length(unnormL), 2));\
				attenuation *= (1.0f - lightToSurfaceAngle/u_SpotLight[n].coneAngle);                \
                                                                                                     \
			    /* Normalized half vector for Blinn-Phong */                                         \
			    vec3 H = normalize(L + V);                                                           \
																						             \
			    /* Ambient + Diffuse + Specular */                                                   \
			    float Ia = toonify(clamp(u_ambientK, 0.0, 1.0));                                      \
			    float Id = toonify(clamp(dot(L, io_fragNormal), 0.0, 1.0));                           \
			    float Is = toonify(clamp(pow(dot(io_fragNormal, H), u_material.shininess), 0.0, 1.0));\
																						             \
		    	vec3 colorAmbient  = u_SpotLight[n].ambient*u_material.ambient*Ia;                   \
			    vec3 colorDiffuse  = u_SpotLight[n].diffuse*u_material.diffuse*Id;                   \
			    vec3 colorSpecular = u_SpotLight[n].specular*u_material.specular*Is;                 \
																						             \
			    if (dot(L, io_fragNormal) <= 0) {                                                    \
				    colorSpecular = vec3(0.0);                                                       \
			    }                                                                                    \
																						             \
			    /* Accumulate color components */                                                    \
			    color += colorAmbient + attenuation*(colorDiffuse + colorSpecular);                  \
		    }                                                                                        \
        }                                                                                            \
    }                                                                                                \
}

#define _ProcessDirectLight(color, shadow, V)                                         \
{                                                                                     \
	if  (u_numDirectLights > 0) {                                                     \
		/* Light vector to fragment */                                                \
		vec3 L = normalize(u_DirectLight.direction);                                  \
																					  \
		/* Normalized half vector for Blinn-Phong */                                  \
		vec3 H = normalize(L + V);                                                    \
																					  \
		/* Ambient + Diffuse + Specular */                                            \
		float Ia = toonify(clamp(u_ambientK, 0.0, 1.0));                                       \
		float Id = toonify(clamp(dot(L, io_fragNormal), 0.0, 1.0));                            \
		float Is = toonify(clamp(pow(dot(io_fragNormal, H), u_material.shininess), 0.0, 1.0)); \
																					  \
		vec3 colorAmbient  = u_DirectLight.ambient*u_material.ambient*Ia;             \
		vec3 colorDiffuse  = u_DirectLight.diffuse*u_material.diffuse*Id;             \
		vec3 colorSpecular = u_DirectLight.specular*u_material.specular*Is;           \
																					  \
		if (dot(L, io_fragNormal) <= 0) {                                             \
			colorSpecular = vec3(0.0);                                                \
		}                                                                             \
																					  \
		/* Accumulate color components */                                             \
		color += colorAmbient + shadow*(colorDiffuse + colorSpecular);                \
	}                                                                                 \
}
void main()
{
    float Ia, Id, Is;
	float shadow = 1.0f;
	float bias = 0.05f;
    vec3 colorAmbient, colorDiffuse, colorSpecular;

    /* Texel color */
    vec3 texel = vec3(0.0f, 0.0f, 0.0f);

    /* Ambient */
    Ia = toonify(clamp(u_ambientK, 0.0, 1.0));

    /* Accumulates the final intensities for the texel */
    vec3 lightAcc = vec3(0.0);

    /* Dot product of view vector and fragment normal in view space. If
       result is close to 0 we decide it is an edge fragment and we paint it black */
	if (dot(io_viewNormal, io_viewVertex) >= EFFECT_STRENGTH) {
        texel = vec3(texture(u_diffuseMap, io_fragUVCoord));

		/* Direct light */
		shadow = texture(u_shadowMapDirectLight, vec3(io_shadowCoordDirectLight.xy, (io_shadowCoordDirectLight.z + bias)));
		_ProcessDirectLight(lightAcc, shadow, io_viewVertex);

        /* For shaders on version 3.3 and earlier the uniform
           block must be indexed by a constant integral expression, which
           in this case must be a harcoded index, thus the macro instead
           of a handy loop */
#if GLSL_VERSION >= 440
		uint nLights = min(u_numPointLights, MAX_LIGHTS);

		for (int i=0; i<nLights; ++i) {
			shadow = texture(u_shadowMapPointLight[i], vec3(io_shadowCoordPointLight[i].xy, (io_shadowCoordPointLight[i].z + bias)));
			_ProcessPointLight(lightAcc, shadow, i, io_viewVertex);
		}

		nLights = min(u_numSpotLights, MAX_LIGHTS);

        for (int i=0; i<nLights; ++i) {
			shadow = texture(u_shadowMapSpotLight[i], vec3(io_shadowCoordSpotLight[i].xy/io_shadowCoordSpotLight[i].w, (io_shadowCoordSpotLight[i].z + bias)/io_shadowCoordSpotLight[i].w));
            _ProcessSpotLight(lightAcc, shadow, i, io_viewVertex);
		}
#else
        _ProcessPointLight(lightAcc, 0, io_viewVertex);
        _ProcessPointLight(lightAcc, 1, io_viewVertex);
        _ProcessPointLight(lightAcc, 2, io_viewVertex);
        _ProcessPointLight(lightAcc, 4, io_viewVertex);
        _ProcessPointLight(lightAcc, 5, io_viewVertex);
        _ProcessPointLight(lightAcc, 6, io_viewVertex);
        _ProcessPointLight(lightAcc, 7, io_viewVertex);
        _ProcessPointLight(lightAcc, 8, io_viewVertex);
        _ProcessPointLight(lightAcc, 9, io_viewVertex);

        _ProcessSpotLight(lightAcc, shadow, 0, io_viewVertex);
        _ProcessSpotLight(lightAcc, shadow, 1, io_viewVertex);
        _ProcessSpotLight(lightAcc, shadow, 2, io_viewVertex);
        _ProcessSpotLight(lightAcc, shadow, 4, io_viewVertex);
        _ProcessSpotLight(lightAcc, shadow, 5, io_viewVertex);
        _ProcessSpotLight(lightAcc, shadow, 6, io_viewVertex);
        _ProcessSpotLight(lightAcc, shadow, 7, io_viewVertex);
        _ProcessSpotLight(lightAcc, shadow, 8, io_viewVertex);
        _ProcessSpotLight(lightAcc, shadow, 9, io_viewVertex);
#endif
    }

    o_color = vec4(texel * lightAcc, u_material.alpha);
}

