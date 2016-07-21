/*
   Helps rendering the normals of the model

   @author Roberto Cano
*/
#version 150 core

layout(triangles) in;
 
// Three lines will be generated: 6 vertices
layout(line_strip, max_vertices=6) out;

uniform mat4 u_MVPMatrix;
uniform float u_normalSize;

in Vertex {
    vec3 normal;
    vec4 color;
} io_vertex[];

out vec4 io_vertexColor;

void main()
{
    int i;
    for(i=0; i<gl_in.length(); i++)
    {
        vec3 position = gl_in[i].gl_Position.xyz;
        vec3 normal = io_vertex[i].normal;

        gl_Position = u_MVPMatrix * vec4(position, 1.0f);
        io_vertexColor = io_vertex[i].color;
        EmitVertex();

        gl_Position = u_MVPMatrix * vec4(position + normal * u_normalSize, 1.0f);
        io_vertexColor = io_vertex[i].color;
        EmitVertex();

        EndPrimitive();
    }
}
