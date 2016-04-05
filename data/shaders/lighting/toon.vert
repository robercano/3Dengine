//
// Roberto Cano (http://www.robertocano.es)
//
#version 330 core

layout(location = 0) in vec3 in_vertex;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_uvcoord;

uniform mat4 u_MVPMatrix;
uniform mat4 u_normalMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_modelMatrix;

out vec3 io_fragVertex;
out vec3 io_fragNormal;
out vec2 io_fragUVCoord;

void main()
{
	gl_Position     = u_MVPMatrix * vec4(in_vertex, 1.0f);
    io_fragVertex = vec3(u_modelMatrix * vec4(in_vertex, 1.0f));

	/* Calculate the normal matrix - account for non-uniform scale */
    mat3 normalMatrix = transpose(inverse(mat3(u_modelMatrix)));
    io_fragNormal = normalize(normalMatrix * in_normal);
    io_fragUVCoord = in_uvcoord;
}
