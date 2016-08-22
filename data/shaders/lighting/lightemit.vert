//
// Roberto Cano (http://www.robertocano.es)
//
#version 330 core

#define GLSL_VERSION 400
#define MAX_LIGHTS 4u

layout(location = 0) in vec3 in_vertex;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_uvcoord;

uniform mat4 u_MVPMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_modelMatrix;
uniform mat3 u_normalMatrix;

out vec3 io_fragVertex;
out vec3 io_fragNormal;
out vec2 io_fragUVCoord;
out vec3 io_viewNormal;
out vec3 io_viewVertex;

uniform uint u_numPointLights;
out vec4 io_shadowCoordPointLight[MAX_LIGHTS];
uniform mat4 u_shadowMVPPointLight[MAX_LIGHTS];

uniform uint u_numSpotLights;
out vec4 io_shadowCoordSpotLight[MAX_LIGHTS];
uniform mat4 u_shadowMVPSpotLight[MAX_LIGHTS];

out vec4 io_shadowCoordDirectLight;
uniform mat4 u_shadowMVPDirectLight;

#define _CalculatePointLight(n)                                                             \
    {                                                                                       \
        if (n < u_numPointLights) {                                                         \
            io_shadowCoordPointLight[n] = u_shadowMVPPointLight[n] * vec4(in_vertex, 1.0f); \
        }                                                                                   \
    }

#define _CalculateSpotLight(n)                                                            \
    {                                                                                     \
        if (n < u_numSpotLights) {                                                        \
            io_shadowCoordSpotLight[n] = u_shadowMVPSpotLight[n] * vec4(in_vertex, 1.0f); \
        }                                                                                 \
    }

void main()
{
    /* Calculate the normal matrix - account for non-uniform scale */
    mat3 normalMatrix = transpose(inverse(mat3(u_modelMatrix)));

    /* World-space coordinates */
    io_fragVertex = vec3(u_modelMatrix * vec4(in_vertex, 1.0f));
    io_fragNormal = -normalize(u_normalMatrix * in_normal);
    io_fragUVCoord = in_uvcoord;

    /* View-space coordinates */
    io_viewVertex = normalize(-vec3(u_viewMatrix * vec4(io_fragVertex, 1.0)));
    io_viewNormal = normalize(vec3(u_viewMatrix * vec4(io_fragNormal, 0.0)));

    /* Clip-space coordinates */
    gl_Position = u_MVPMatrix * vec4(in_vertex, 1.0f);

    /* Shadow-map coordinate */
    io_shadowCoordDirectLight = u_shadowMVPDirectLight * vec4(in_vertex, 1.0f);

#if GLSL_VERSION >= 400
    uint nLights = min(u_numPointLights, MAX_LIGHTS);

    for (uint i = 0u; i < nLights; ++i) {
        io_shadowCoordPointLight[i] = u_shadowMVPPointLight[i] * vec4(in_vertex, 1.0f);
    }

    nLights = min(u_numSpotLights, MAX_LIGHTS);

    for (uint i = 0u; i < nLights; ++i) {
        io_shadowCoordSpotLight[i] = u_shadowMVPSpotLight[i] * vec4(in_vertex, 1.0f);
    }
#else
    _CalculatePointLight(0u);
    _CalculatePointLight(1u);
    _CalculatePointLight(2u);
    _CalculatePointLight(3u);

    _CalculateSpotLight(0u);
    _CalculateSpotLight(1u);
    _CalculateSpotLight(2u);
    _CalculateSpotLight(3u);
#endif
}
