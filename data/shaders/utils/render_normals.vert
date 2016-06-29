//
// Roberto Cano (http://www.robertocano.es)
//
#version 330 core

layout(location = 0) in vec3 in_vertex;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_uvcoord;

out Vertex
{
    vec3 normal;
    vec4 color;
} io_vertex;

void main()
{
	gl_Position = vec4(in_vertex, 1.0f);
    io_vertex.normal = in_normal;
    io_vertex.color = vec4(1.0f, 0.647f, 0.0f, 1.0f);
}
