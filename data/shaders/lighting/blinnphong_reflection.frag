//
// Roberto Cano (http://www.robertocano.es)
//
#version 400 core

/* Phong reflection model implemented following the explanation
   at https://en.wikipedia.org/wiki/Phong_reflection_model and
   http://sunandblackcat.com/tipFullView.php?l=eng&topicid=30&topic=Phong-Lighting

   Fully implemented using the formulas, no code has been
   copied from any other source

   @author Roberto Cano
*/

#define MAX_LIGHTS 10

/* Light definition */
layout (std140) uniform Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
} light[MAX_LIGHTS];

uniform uint numLights;

/* Global scene ambient constant */
uniform float ambientK;

/* Material definition for this geometry */
layout (std140) uniform Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float alpha;
    float shininess;
} material;

uniform sampler2D diffuseMap;
uniform mat4 view;
uniform mat4 model;

in vec3 fragment_vertex;
in vec3 fragment_normal;
in vec2 fragment_uvcoord;

out vec4 color;

float sRGB2Linear(float c) {
    if (c <= 0.04045) {
        return c/12.92;
    }
    return pow((c + 0.055)/1.055, 2.4);
}

void main()
{
    float Ia, Id, Is;
    vec3 colorAmbient, colorDiffuse, colorSpecular;

    /* Texel color */
    vec4 texel = texture(diffuseMap, fragment_uvcoord);

    /* Ambient */
    Ia = clamp(ambientK, 0.0, 1.0);

    /* Vector to the camera */
    vec3 cameraPos = -vec3(view * vec4(0.0, 0.0, 0.0, 1.0));
    vec3 V = normalize(cameraPos - fragment_vertex);

    vec3 acc = vec3(0.0);

	vec3 N = fragment_normal;

/* FOR */
	uint nLights = min(numLights, MAX_LIGHTS);

    for (int i=0; i<nLights; i++) {
        /* Attenuation */
        float attenuation = 1.0 / (1.0 + 0.00001 * pow(length(light[i].position - fragment_vertex), 2));

        /* Light vector to fragment */
        vec3 L = normalize(light[i].position - fragment_vertex);

        /* Normalized half vector for Blinn-Phong */
        vec3 H = normalize(L + V);

        /* Diffuse + Specular */
        Id = clamp(dot(L, N), 0.0, 1.0);
        Is = clamp(pow(dot(N, H), material.shininess), 0.0, 1.0);

        colorAmbient  = light[i].ambient*material.ambient*Ia;
        colorDiffuse  = light[i].diffuse*material.diffuse*Id;
        colorSpecular  = light[i].specular*material.specular*Is;

        if (dot(L, N) <= 0) {
            colorSpecular = vec3(0.0);
        }

        /* Accumulate color components */
        acc += colorAmbient + attenuation*(colorDiffuse + colorSpecular);
    }

    color = texel * vec4(acc, material.alpha);
}

