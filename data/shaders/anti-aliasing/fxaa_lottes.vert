//
// Roberto Cano (http://www.robertocano.es)
//
#version 330 core

// Input parameters
layout(location = 0) in vec2 v_coord;

uniform mat4 f_scale;

// Output parameters for the fragment shader
out vec2 f_texcoord;

void main(void) {
  gl_Position = f_scale*vec4(v_coord, 0.0, 1.0);
  f_texcoord = (v_coord + 1.0) / 2.0;
}
