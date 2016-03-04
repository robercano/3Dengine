//
// Roberto Cano (http://www.robertocano.es)
//
#version 330 core

in vec2 f_texcoord;
uniform sampler2D fbo_texture;

out vec4 fragColor;

void main(void) {
    vec2 texcoord = f_texcoord;
    fragColor = texture(fbo_texture, texcoord);
}
