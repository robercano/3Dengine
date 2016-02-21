#version 330 core

in vec2 f_texcoord;
uniform sampler2D fbo_texture;

out vec4 fragColor;

void main(void) {
    fragColor = texture(fbo_texture, f_texcoord);
}
