//
// Roberto Cano (http://www.robertocano.es)
//
#version 330 core

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uvcoord;

uniform mat4 MVP;
uniform mat4 view;
uniform mat4 model;

out vec3 fragment_vertex;
out vec3 fragment_normal;
out vec2 fragment_uvcoord;

void main()
{
	gl_Position    = MVP * vec4(vertex, 1);
    fragment_vertex = vec3(model * vec4(vertex, 1.0f));

	/* Calculate the normal matrix - account for non-uniform scale */
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    fragment_normal = normalize(normalMatrix * normal);
    fragment_uvcoord = uvcoord;
}
