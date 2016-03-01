//
// Roberto Cano (http://www.robertocano.es)
//
#version 330 core

in vec2 f_texcoord;
uniform sampler2D fbo_texture;
uniform float offset;

out vec4 fragColor;

void main(void) {
    vec2 texcoord = f_texcoord;
    texcoord.x += sin(texcoord.y * 4.0*2.0*3.14159 + offset/10.0) / 100.0;
    fragColor = texture(fbo_texture, texcoord);
}
