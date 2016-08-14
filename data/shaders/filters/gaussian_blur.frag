/*
	@author Roberto Cano (http://www.robertocano.es)
*/
#version 330 core

#define KERNEL_SIZE 5

/* I/O, uniforms */
in vec2 io_texcoord;

uniform sampler2D fbo_texture;
uniform bool u_horizontal;
uniform float u_texelSize;

out vec4 o_color;

float weight[KERNEL_SIZE] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main(void) {
    vec4 texel = texture(fbo_texture, io_texcoord);
    vec3 result = texel.rgb * weight[0];
    
    if (u_horizontal == true) {
        for (int i = 1; i<KERNEL_SIZE; ++i) {
            result += texture(fbo_texture, io_texcoord + vec2(u_texelSize*i, 0.0)).rgb * weight[i];
            result += texture(fbo_texture, io_texcoord - vec2(u_texelSize*i, 0.0)).rgb * weight[i];
        }
    } else {
        for (int i = 1; i<KERNEL_SIZE; ++i) {
            result += texture(fbo_texture, io_texcoord + vec2(0.0, u_texelSize*i)).rgb * weight[i];
            result += texture(fbo_texture, io_texcoord - vec2(0.0, u_texelSize*i)).rgb * weight[i];
        }
    }

    o_color = vec4(result, texel.a);
}
