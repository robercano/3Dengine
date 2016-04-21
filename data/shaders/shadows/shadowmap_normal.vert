//
// Roberto Cano (http://www.robertocano.es)
//
#version 330 core

layout(location = 0) in vec3 in_vertex;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_uvcoord;

uniform mat4 u_MVPMatrix;

void main()
{
    /* Clip-space coordinates */
	gl_Position = u_MVPMatrix * vec4(in_vertex, 1.0f);
}
