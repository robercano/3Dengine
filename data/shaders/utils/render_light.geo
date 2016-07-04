/*
   Helps rendering the normals of the model

   @author Roberto Cano
*/
#version 150 core

layout(points) in;
 
uniform mat4 u_PMatrix;

// Three lines will be generated: 6 vertices
layout(triangle_strip, max_vertices=6) out;

out vec2 io_intensity;

#define BBOARD_SIZE 10.0f

void main()
{
    vec4 position = gl_in[0].gl_Position;
    vec4 offset = u_PMatrix * vec4(BBOARD_SIZE, BBOARD_SIZE, 0.0f, 1.0f);

    gl_Position = vec4(position.x - offset.x, position.y + offset.y, position.z, position.w);
    io_intensity = vec2(0.0f, 1.0f);
    EmitVertex();
    gl_Position = vec4(position.x + offset.x, position.y - offset.y, position.z, position.w);
    io_intensity = vec2(1.0f, 0.0f);
    EmitVertex();
    gl_Position = vec4(position.x + offset.x, position.y + offset.y, position.z, position.w);
    io_intensity = vec2(1.0f, 1.0f);
    EmitVertex();

    EndPrimitive();

    gl_Position = vec4(position.x - offset.x, position.y + offset.y, position.z, position.w);
    io_intensity = vec2(0.0f, 1.0f);
    EmitVertex();
    gl_Position = vec4(position.x - offset.x, position.y - offset.y, position.z, position.w);
    io_intensity = vec2(0.0f, 0.0f);
    EmitVertex();
    gl_Position = vec4(position.x + offset.x, position.y - offset.y, position.z, position.w);
    io_intensity = vec2(1.0f, 0.0f);
    EmitVertex();

    EndPrimitive();
}
