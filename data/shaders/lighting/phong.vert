//
// Roberto Cano (http://www.robertocano.es)
//
#version 330 core

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 color;
layout(location = 2) in vec3 normal;

uniform mat4 MVP;
out vec3 fragment_vertex;
out vec3 fragment_color;
out vec3 fragment_normal;

void main()
{
	gl_Position    = MVP * vec4(vertex, 1);
    fragment_vertex = vertex;
	fragment_color = color;
    fragment_normal = normal;
}
