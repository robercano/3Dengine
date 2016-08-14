//
// Roberto Cano (http://www.robertocano.es)
//
#version 330 core

// Input parameters
layout(location = 0) in vec2 in_coord;

out vec2 io_texcoord;

void main(void) {
    io_texcoord = (in_coord + 1.0) / 2.0;
    gl_Position = vec4(in_coord, 0.0, 1.0);
}
