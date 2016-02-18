#version 330 core

uniform struct Light {
    vec3 position;
    vec3 intensities;
} light;

in vec3 fragment_vertex;
in vec3 fragment_color;
in vec3 fragment_normal;

out vec3 color;

void main()
{
    mat4 model = mat4(1.0);
    vec3 light1Pos = vec3(2, 2, 2);
    vec3 light1Color = vec3(1, 1, 1);
    vec3 light2Pos = vec3(-5, 5, -5);
    vec3 light2Color = vec3(1, .3, .4);

    /* Calculate the normal matrix (excluding scaling and translation, centerd in origin) */
    //mat3 normalMatrix = transpose(inverse(mat3(model)));

    /* And now calculate the final normal */
    //vec3 normal = normalize(fragment_normal * normalMatrix);
vec3 normal = fragment_normal;

    /* Calculate fragment position in world coordinates */
    vec3 fragmentPos = vec3(model*vec4(fragment_vertex, 1));

    /* Light vector to fragment */
    vec3 surfaceToLight1 = light1Pos - fragmentPos;
    vec3 surfaceToLight2 = light2Pos - fragmentPos;

    /* Calculate the brightness */
    float brightness1 = dot(normal, surfaceToLight1) / (length(normal) * length(surfaceToLight1));
    float brightness2 = dot(normal, surfaceToLight2) / (length(normal) * length(surfaceToLight2));
    brightness1 = clamp(brightness1, 0, 1);
    brightness2 = clamp(brightness2, 0, 1);

    /* Calculate the final color based on the base color of the object (or texture color),
       the brightness and the color of the light */
    vec3 baseColor = vec3(1);

    /* 1.0 / (1.0 + a*dist + b*dist*dist)) */
    float dist1 = distance(fragment_vertex, light1Pos);
    float att1 = 1.0 / (1.0 + 0*dist1 + 0.1*dist1*dist1);
    float dist2 = distance(fragment_vertex, light2Pos);
    float att2 = 1.0 / (1.0 + 0*dist2 + 0.1*dist2*dist2);

    color = vec3(brightness1 * light1Color * baseColor.rgb * att1);
    color += vec3(brightness2 * light2Color * baseColor.rgb * att2);
}
