//
// Roberto Cano (http://www.robertocano.es)
//
#version 330 core

/* Phong reflection model implemented follwoing the explanation
   at https://en.wikipedia.org/wiki/Phong_reflection_model and
   http://sunandblackcat.com/tipFullView.php?l=eng&topicid=30&topic=Phong-Lighting

   Fully implemented using the formulas, no code has been
   copied from any other source

   @author Roberto Cano
*/

uniform struct Light {
    vec3 position;
    vec3 intensities;
} light;

uniform mat4 view;

in vec3 fragment_vertex;
in vec3 fragment_color;
in vec3 fragment_normal;

out vec4 color;

void main()
{
    float Ia, Id, Is;
    vec3 colorAmbient, colorDiffuse, colorSpecular;
    mat4 model = mat4(1.0);

    /* Light source */
    vec3 lightPos = vec3(3, 3, 3);
    vec3 lightAmbient = vec3(1.0);
    vec3 lightDiffuse = vec3(1.0);
    vec3 lightSpecular = vec3(1.0);

    /* Ambient light constant */
    float ambientK = 0.1;

    /* Material properties */
    vec3 materialAmbient = vec3(0.24725, 0.2245, 0.0645);
    vec3 materialDiffuse = vec3(0.34615, 0.3143, 0.0903);
    vec3 materialSpecular = vec3(0.797357, 0.723991, 0.208006);
    float materialAlpha = 1.0;
    float shininess = 83.2;

    /* Calculate fragment position in world coordinates */
    vec3 fragmentPos = vec3(model*vec4(fragment_vertex, 1));

    /* Light vector to fragment */
    vec3 L = normalize(lightPos - fragmentPos);

    /* Calculate the normal matrix (excluding scaling and translation, centerd in origin) */
    //mat3 normalMatrix = transpose(inverse(mat3(model)));

    /* And now calculate the final normal */
    /* TODO: this must be done in the vertex shader */
    vec3 N = normalize(fragment_normal);

    /* Vector to the camera */
    mat3 rotMatView = mat3(view);
    vec3 tmpCameraPos = vec3(view[3]);
    vec3 cameraPos = -tmpCameraPos * rotMatView;

    vec3 V = normalize(cameraPos - fragmentPos);

    /* Reflection vector for Phong */
    vec3 R = reflect(L, N);

    /*      Ambient + Diffuse          + Specular */
    Ia = clamp(ambientK, 0.0, 1.0);
    Id = clamp(dot(L, N), 0.0, 1.0);
    Is = clamp(pow(dot(R, V), shininess), 0.0, 1.0);

    colorAmbient  = lightAmbient*materialAmbient*Ia;
    colorDiffuse  = lightDiffuse*materialDiffuse*Id;
    colorSpecular  = lightSpecular*materialSpecular*Is;

    if (dot(L, N) <= 0) {
        colorSpecular = vec3(0.0);
    }

    /* Attenuation */
    float attenuation = 1.0 / (1.0 + 0.00001 * pow(length(lightPos - fragmentPos), 2));

    color = vec4(colorAmbient + attenuation*(colorDiffuse + colorSpecular), materialAlpha);
}

