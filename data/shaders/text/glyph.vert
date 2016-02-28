#version 330 core

// Input parameters
layout(location = 0) in vec2 v_coord;
layout(location = 1) in vec2 uv_coord;

// Output parameters for the fragment shader
out vec2 f_texcoord;
uniform mat4 glyphTransform;
uniform vec4 glyphPos;

void main(void) {
    mat4 translation = mat4(1.0);
    translation[3] = glyphPos;

    gl_Position = glyphTransform*translation*vec4(v_coord, 0.0, 1.0);
    f_texcoord = uv_coord;
}
