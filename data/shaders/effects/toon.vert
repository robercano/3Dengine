//
// Roberto Cano (http://www.robertocano.es)
//
#version 330 core

layout(location = 0) in vec3 in_vertex;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_uvcoord;

uniform mat4 u_MVPMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_modelMatrix;
uniform mat3 u_normalMatrix;
//uniform vec3 u_scale;

void main()
{
    vec3 scale = vec3(1.05, 1.05, 1.05);
	//gl_Position = u_MVPMatrix * scale * vec4(in_vertex, 1.0f);
	gl_Position = u_MVPMatrix * vec4(scale * in_vertex, 1.0f);
}
