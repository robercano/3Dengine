/* Phong reflection model implemented following the explanation
   at https://en.wikipedia.org/wiki/Phong_reflection_model and
   http://sunandblackcat.com/tipFullView.php?l=eng&topicid=30&topic=Phong-Lighting

   Fully implemented using the formulas, no code has been
   copied from any other source

   @author Roberto Cano
*/
#version 400 core

#define GLSL_VERSION 440
#define MAX_LIGHTS 10

/* Light definition */
layout (std140) uniform Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
} u_light[MAX_LIGHTS];

uniform uint u_numLights;

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
uniform mat4      u_viewMatrix;
uniform mat4      u_modelMatrix;

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

#define _ProcessLight(color, n, V)                                                    \
{                                                                                     \
    if (n < u_numLights) {                                                            \
        vec3 unnormL = u_light[n].position - io_fragVertex;                           \
                                                                                      \
        /* Attenuation */                                                             \
        float attenuation = 1.0 / (1.0 + 0.00001 * pow(length(unnormL), 2));          \
                                                                                      \
        /* Light vector to fragment */                                                \
        vec3 L = normalize(unnormL);                                                  \
                                                                                      \
        /* Normalized half vector for Blinn-Phong */                                  \
        vec3 H = normalize(L + V);                                                    \
                                                                                      \
        /* Ambient + Diffuse + Specular */                                            \
        float Ia = clamp(u_ambientK, 0.0, 1.0);                                       \
        float Id = clamp(dot(L, io_fragNormal), 0.0, 1.0);                            \
        float Is = clamp(pow(dot(io_fragNormal, H), u_material.shininess), 0.0, 1.0); \
                                                                                      \
        vec3 colorAmbient  = u_light[n].ambient*u_material.ambient*Ia;                \
        vec3 colorDiffuse  = u_light[n].diffuse*u_material.diffuse*Id;                \
        vec3 colorSpecular = u_light[n].specular*u_material.specular*Is;              \
                                                                                      \
        if (dot(L, io_fragNormal) <= 0) {                                             \
            colorSpecular = vec3(0.0);                                                \
        }                                                                             \
                                                                                      \
        /* Accumulate color components */                                             \
        color += colorAmbient + attenuation*(colorDiffuse + colorSpecular);           \
    }                                                                                 \
}

void main()
{
    /* Accumulates the final intensities for the texel */
    vec3 lightAcc = vec3(0.0);

    /* For shaders on version 3.3 and earlier the uniform
       block must be indexed by a constant integral expression, which
       in this case must be a harcoded index, thus the macro instead
       of a handy loop */
#if GLSL_VERSION >= 440
        uint nLights = min(u_numLights, MAX_LIGHTS);

        for (int i=0; i<nLights; ++i) {
            _ProcessLight(lightAcc, i, io_viewVertex);
        }
#else
        _ProcessLight(lightAcc, 0, io_viewVertex);
        _ProcessLight(lightAcc, 1, io_viewVertex);
        _ProcessLight(lightAcc, 2, io_viewVertex);
        _ProcessLight(lightAcc, 4, io_viewVertex);
        _ProcessLight(lightAcc, 5, io_viewVertex);
        _ProcessLight(lightAcc, 6, io_viewVertex);
        _ProcessLight(lightAcc, 7, io_viewVertex);
        _ProcessLight(lightAcc, 8, io_viewVertex);
        _ProcessLight(lightAcc, 9, io_viewVertex);
#endif

    o_color = vec4(vec3(texture(u_diffuseMap, io_fragUVCoord)) * lightAcc, u_material.alpha);
}

