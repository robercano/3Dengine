/*
    Utility shader to render a solid color for the model

    @author Roberto Cano (http://www.robertocano.es)
*/
#version 400 core

#define GLSL_VERSION 400
#define MAX_LIGHTS 4u

layout(location = 0) in vec3 in_vertex;

uniform mat4 u_MVPMatrix;

void main()
{
	/* Calculate the normal matrix - account for non-uniform scale */
    mat3 normalMatrix = transpose(inverse(mat3(u_modelMatrix)));

    /* Clip-space coordinates */
	gl_Position = u_MVPMatrix * vec4(in_vertex, 1.0f);
}
