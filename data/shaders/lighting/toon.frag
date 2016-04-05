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
	if (intensity < 0.0001) return 0.0;
	else if (intensity < 0.25) return 0.125;
	else if (intensity < 0.5) return 0.375;
	else if (intensity < 0.75) return 0.625;
	else if (intensity < 0.99) return 0.875;
	else return 1.0;
}

void main()
{
    float Ia, Id, Is;
    vec3 colorAmbient, colorDiffuse, colorSpecular;

    /* Texel color */
    vec3 texel = vec3(0.0f, 0.0f, 0.0f);

    /* Ambient */
    Ia = toonify(clamp(u_ambientK, 0.0, 1.0));

    /* Accumulates the final intensities for the texel */
    vec3 lightAcc = vec3(0.0);

    /* Dot product of view vector and fragment normal in view space. If
       result is close to 0 we decide it is an edge fragment and we paint it black */
	if (dot(io_viewNormal, io_viewVertex) >= 0.3) {
        texel = vec3(texture(u_diffuseMap, io_fragUVCoord));

        uint nLights = min(u_numLights, MAX_LIGHTS);

        for (int i=0; i<nLights; i++) {
            vec3 unnormL = u_light[i].position - io_fragVertex;

            /* Attenuation */
            float attenuation = 1.0 / (1.0 + 0.00001 * pow(length(unnormL), 2));

            /* Light vector to fragment */
            vec3 L = normalize(unnormL);

            /* Normalized half vector for Blinn-Phong */
            vec3 H = normalize(L + io_viewVertex);

            /* Diffuse + Specular */
            Id = toonify(clamp(dot(L, io_fragNormal), 0.0, 1.0));
            Is = toonify(clamp(pow(dot(io_fragNormal, H), u_material.shininess), 0.0, 1.0));

            colorAmbient   = u_light[i].ambient*u_material.ambient*Ia;
            colorDiffuse   = u_light[i].diffuse*u_material.diffuse*Id;
            colorSpecular  = u_light[i].specular*u_material.specular*Is;

            if (dot(L, io_fragNormal) <= 0) {
                colorSpecular = vec3(0.0);
            }

            /* Accumulate color components */
            lightAcc += colorAmbient + attenuation*(colorDiffuse + colorSpecular);
        }
    }

    o_color = vec4(texel * lightAcc, u_material.alpha);
}

