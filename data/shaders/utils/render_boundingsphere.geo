/*
   Helps rendering the normals of the model

   @author Roberto Cano
*/
#version 150 core

#define NUM_VERTICES 40

// Only 1 point is in the input for the center
layout(points) in;
 
// Generate a circle around the center of 20 vertices,
// the first one is repeated to close the circle
layout(line_strip, max_vertices=NUM_VERTICES) out;

uniform float u_radius;
uniform mat4 u_projectionMatrix;

#define M_PI 3.1415926535897932384626433832795

void main()
{
    vec4 screenVertex = gl_in[0].gl_Position;

    int i;
    for(i=0; i<NUM_VERTICES; i++)
    {
        vec4 newVertex = u_projectionMatrix * vec4(u_radius * cos(i*2.0*M_PI/(NUM_VERTICES-1)), u_radius * sin(i*2.0*M_PI/(NUM_VERTICES-1)), 0.0, 1.0);

        gl_Position = vec4(screenVertex.x + newVertex.x, screenVertex.y + newVertex.y, screenVertex.z, screenVertex.w);
        EmitVertex();
    }
    EndPrimitive();
}
