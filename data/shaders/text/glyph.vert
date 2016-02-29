#version 330 core

// Input parameters
layout(location = 0) in vec2 v_coord;
layout(location = 1) in vec2 uv_coord;

// Output parameters for the fragment shader
out vec2 f_texcoord;
uniform mat4 glyphTransform;
uniform vec2 glyphPos;

void main(void) {
    gl_Position = glyphTransform*vec4(v_coord + glyphPos, 0.0, 1.0);
    f_texcoord = uv_coord;
}
