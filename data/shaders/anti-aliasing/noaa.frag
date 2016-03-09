//
// Roberto Cano (http://www.robertocano.es)
//
#version 330 core

in vec2 f_texcoord;
uniform sampler2D fbo_texture;

out vec4 fragColor;

float sRGB2Linear(float c) {
    if (c <= 0.04045) {
        return c/12.92;
    }
    return pow((c + 0.055)/1.055, 2.4);
}

void main(void) {
    fragColor = texture(fbo_texture, f_texcoord);

#if 0
    /* sRGB to linear conversion, OpenGL does not support
       the shader outputting sRGB when GL_FRAMEBUFFER_SRGB is
       enabled  */
    fragColor= vec4(sRGB2Linear(fragColor.r),
                    sRGB2Linear(fragColor.g),
                    sRGB2Linear(fragColor.b),
                    fragColor.a);
#endif
}
