/*
    Toon effect to add a border to the model. The vertex
    shader will apply a scale to the vertex before transforming
    it through MVP. Then the fragment shader will emit all fragments
    with the same color passed as a uniform

    @author Roberto Cano
*/
#version 330 core

#define MAX_LIGHTS 10

/* Light definition */
layout(std140) uniform Light
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
}
u_light[MAX_LIGHTS];

uniform uint u_numLights;

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

uniform vec4 u_color;

out vec4 o_color;

void main() { o_color = vec4(0.0, 0.0, 0.0, 1.0); }
