#version 330 core

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uvcoord;

uniform mat4 MVP;

out vec3 fragment_vertex;
out vec3 fragment_normal;
out vec2 fragment_uvcoord;

void main()
{
	gl_Position    = MVP * vec4(vertex, 1);
    fragment_vertex = vertex;
    fragment_normal = normal;
    fragment_uvcoord = uvcoord;
}
