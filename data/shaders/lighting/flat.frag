/* Phong reflection model implemented following the explanation
   at https://en.wikipedia.org/wiki/Phong_reflection_model and
   http://sunandblackcat.com/tipFullView.php?l=eng&topicid=30&topic=Phong-Lighting

   Fully implemented using the formulas, no code has been
   copied from any other source

   The toonify flag enables creates a discreet lighting where
   groups of values are moved to fixed intensity values, thus
   creating a sort of flat lighting

   @author Roberto Cano
*/
#version 330 core

#define GLSL_VERSION 400
#define MAX_LIGHTS 4

/* Direct light definition */
layout(std140) uniform DirectLight
{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
}
u_DirectLight;

uniform sampler2DShadow u_shadowMapDirectLight;
in vec4 io_shadowCoordDirectLight;
uniform uint u_numDirectLights; /* 0 or 1 */

/* Point light definition */
layout(std140) uniform PointLight
{
    vec3 position;
    float attenuation;
    float cutoff;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
}
u_PointLight[MAX_LIGHTS];

uniform sampler2DShadow u_shadowMapPointLight[MAX_LIGHTS];
in vec4 io_shadowCoordPointLight[MAX_LIGHTS];
uniform uint u_numPointLights;

/* Spotlight definition */
layout(std140) uniform SpotLight
{
    vec3 position;
    vec3 direction;
    float coneAngle;
    float conePenumbra;
    float attenuation;
    float cutoff;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
}
u_SpotLight[MAX_LIGHTS];

uniform sampler2DShadow u_shadowMapSpotLight[MAX_LIGHTS];
in vec4 io_shadowCoordSpotLight[MAX_LIGHTS];
uniform uint u_numSpotLights;

/* Global scene ambient constant */
uniform float u_ambientK;

/* Material definition for this geometry */
layout(std140) uniform Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float alpha;
    float shininess;
}
u_material;

/* Texture and transformation matrices */
uniform sampler2D u_diffuseMap;
uniform mat4 u_viewMatrix;
uniform mat4 u_modelMatrix;

/* Flag to enable toon shadowing */
uniform uint u_enableToon;

/* Input from vertex shader */
in vec3 io_fragVertex;
flat in vec3 io_fragNormal;
in vec2 io_fragUVCoord;
flat in vec3 io_viewNormal;
in vec3 io_viewVertex;

/* Output of this shader */
out vec4 o_color;

float sRGB2Linear(float c)
{
    if (c <= 0.04045) {
        return c / 12.92;
    }
    return pow((c + 0.055) / 1.055, 2.4);
}

float toonify(float intensity)
{
    if (u_enableToon == 1u) {
        if (intensity < 0.0001)
            return 0.0;
        else if (intensity < 0.25)
            return 0.125;
        else if (intensity < 0.5)
            return 0.375;
        else if (intensity < 0.75)
            return 0.625;
        else if (intensity < 0.99)
            return 0.875;
        else
            return 1.0;
    } else {
        return intensity;
    }
}

#define _ProcessPointLight(color, n, V)                                                                                          \
    {                                                                                                                            \
        if (n < u_numPointLights) {                                                                                              \
            float shadow =                                                                                                       \
                texture(u_shadowMapPointLight[n], vec3(io_shadowCoordPointLight[n].xy / io_shadowCoordPointLight[n].w,           \
                                                       (io_shadowCoordPointLight[n].z + bias) / io_shadowCoordPointLight[n].w)); \
            vec3 unnormL = u_PointLight[n].position - io_fragVertex;                                                             \
            float distanceToLight = length(unnormL);                                                                             \
                                                                                                                                 \
            if (distanceToLight <= u_PointLight[n].cutoff) {                                                                     \
                float attenuation = shadow / (1.0 + u_PointLight[n].attenuation * pow(length(unnormL), 2));                      \
                                                                                                                                 \
                /* Light vector to fragment */                                                                                   \
                vec3 L = normalize(unnormL);                                                                                     \
                                                                                                                                 \
                /* Normalized half vector for Blinn-Phong */                                                                     \
                vec3 H = normalize(L + V);                                                                                       \
                                                                                                                                 \
                /* Ambient + Diffuse + Specular */                                                                               \
                float Ia = toonify(clamp(u_ambientK, 0.0, 1.0));                                                                 \
                float Id = toonify(clamp(dot(L, io_fragNormal), 0.0, 1.0));                                                      \
                float Is = toonify(clamp(pow(dot(io_fragNormal, H), u_material.shininess), 0.0, 1.0));                           \
                                                                                                                                 \
                vec3 colorAmbient = u_PointLight[n].ambient * u_material.ambient * Ia;                                           \
                vec3 colorDiffuse = u_PointLight[n].diffuse * u_material.diffuse * Id;                                           \
                vec3 colorSpecular = u_PointLight[n].specular * u_material.specular * Is;                                        \
                                                                                                                                 \
                if (dot(L, io_fragNormal) <= 0) {                                                                                \
                    colorSpecular = vec3(0.0);                                                                                   \
                }                                                                                                                \
                                                                                                                                 \
                /* Accumulate color components */                                                                                \
                color += colorAmbient + attenuation * (colorDiffuse + colorSpecular);                                            \
            }                                                                                                                    \
        }                                                                                                                        \
    }

#define _ProcessSpotLight(color, n, V)                                                                                                   \
    {                                                                                                                                    \
        if (n < u_numSpotLights) {                                                                                                       \
            float shadow = texture(u_shadowMapSpotLight[n], vec3(io_shadowCoordSpotLight[n].xy / io_shadowCoordSpotLight[n].w,           \
                                                                 (io_shadowCoordSpotLight[n].z + bias) / io_shadowCoordSpotLight[n].w)); \
            vec3 unnormL = u_SpotLight[n].position - io_fragVertex;                                                                      \
            float distanceToLight = length(unnormL);                                                                                     \
                                                                                                                                         \
            if (distanceToLight <= u_SpotLight[n].cutoff) {                                                                              \
                /* Light vector to fragment */                                                                                           \
                vec3 L = normalize(unnormL);                                                                                             \
                                                                                                                                         \
                float lightToSurfaceAngle = degrees(acos(dot(-L, normalize(u_SpotLight[n].direction))));                                 \
                                                                                                                                         \
                if (lightToSurfaceAngle <= u_SpotLight[n].coneAngle) {                                                                   \
                    float attenuation = shadow / (1.0 + u_SpotLight[n].attenuation * pow(length(unnormL), 2));                           \
                    attenuation *= (1.0f - lightToSurfaceAngle / u_SpotLight[n].coneAngle);                                              \
                                                                                                                                         \
                    /* Normalized half vector for Blinn-Phong */                                                                         \
                    vec3 H = normalize(L + V);                                                                                           \
                                                                                                                                         \
                    /* Ambient + Diffuse + Specular */                                                                                   \
                    float Ia = toonify(clamp(u_ambientK, 0.0, 1.0));                                                                     \
                    float Id = toonify(clamp(dot(L, io_fragNormal), 0.0, 1.0));                                                          \
                    float Is = toonify(clamp(pow(dot(io_fragNormal, H), u_material.shininess), 0.0, 1.0));                               \
                                                                                                                                         \
                    vec3 colorAmbient = u_SpotLight[n].ambient * u_material.ambient * Ia;                                                \
                    vec3 colorDiffuse = u_SpotLight[n].diffuse * u_material.diffuse * Id;                                                \
                    vec3 colorSpecular = u_SpotLight[n].specular * u_material.specular * Is;                                             \
                                                                                                                                         \
                    if (dot(L, io_fragNormal) <= 0) {                                                                                    \
                        colorSpecular = vec3(0.0);                                                                                       \
                    }                                                                                                                    \
                                                                                                                                         \
                    /* Accumulate color components */                                                                                    \
                    color += colorAmbient + attenuation * (colorDiffuse + colorSpecular);                                                \
                }                                                                                                                        \
            }                                                                                                                            \
        }                                                                                                                                \
    }

#define _ProcessDirectLight(color, V)                                                                                                 \
    {                                                                                                                                 \
        if (u_numDirectLights > 0u) {                                                                                                 \
            float shadow = texture(u_shadowMapDirectLight, vec3(io_shadowCoordDirectLight.xy, (io_shadowCoordDirectLight.z + bias))); \
            /* Light vector to fragment */                                                                                            \
            vec3 L = normalize(-u_DirectLight.direction);                                                                             \
                                                                                                                                      \
            /* Normalized half vector for Blinn-Phong */                                                                              \
            vec3 H = normalize(L + V);                                                                                                \
                                                                                                                                      \
            /* Ambient + Diffuse + Specular */                                                                                        \
            float Ia = toonify(clamp(u_ambientK, 0.0, 1.0));                                                                          \
            float Id = toonify(clamp(dot(L, io_fragNormal), 0.0, 1.0));                                                               \
            float Is = toonify(clamp(pow(dot(io_fragNormal, H), u_material.shininess), 0.0, 1.0));                                    \
                                                                                                                                      \
            vec3 colorAmbient = u_DirectLight.ambient * u_material.ambient * Ia;                                                      \
            vec3 colorDiffuse = u_DirectLight.diffuse * u_material.diffuse * Id;                                                      \
            vec3 colorSpecular = u_DirectLight.specular * u_material.specular * Is;                                                   \
                                                                                                                                      \
            if (dot(L, io_fragNormal) <= 0) {                                                                                         \
                colorSpecular = vec3(0.0);                                                                                            \
            }                                                                                                                         \
                                                                                                                                      \
            /* Accumulate color components */                                                                                         \
            color += colorAmbient + shadow * (colorDiffuse + colorSpecular);                                                          \
        }                                                                                                                             \
    }

void main()
{
    /* Accumulates the final intensities for the texel */
    vec3 lightAcc = vec3(0.0);
    float shadow = 1.0f;
    float bias = 0.05f;

    /* Direct light */
    _ProcessDirectLight(lightAcc, io_viewVertex);

/* For shaders on version 3.3 and earlier the uniform
   block must be indexed by a constant integral expression, which
   in this case must be a harcoded index, thus the macro instead
   of a handy loop */
#if GLSL_VERSION >= 440
    uint nLights = min(u_numPointLights, MAX_LIGHTS);
    for (int i = 0; i < nLights; ++i) {
        _ProcessPointLight(lightAcc, i, io_viewVertex);
    }

    nLights = min(u_numSpotLights, MAX_LIGHTS);
    for (int i = 0; i < nLights; ++i) {
        _ProcessSpotLight(lightAcc, i, io_viewVertex);
    }
#else
    _ProcessPointLight(lightAcc, 0u, io_viewVertex);
    _ProcessPointLight(lightAcc, 1u, io_viewVertex);
    _ProcessPointLight(lightAcc, 2u, io_viewVertex);
    _ProcessPointLight(lightAcc, 3u, io_viewVertex);

    _ProcessSpotLight(lightAcc, 0u, io_viewVertex);
    _ProcessSpotLight(lightAcc, 1u, io_viewVertex);
    _ProcessSpotLight(lightAcc, 2u, io_viewVertex);
    _ProcessSpotLight(lightAcc, 3u, io_viewVertex);
#endif

    o_color = vec4(vec3(texture(u_diffuseMap, io_fragUVCoord)) * lightAcc, u_material.alpha);
}
