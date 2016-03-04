//
// Roberto Cano (http://www.robertocano.es)
//
#version 330 core

in vec2 f_texcoord;
uniform sampler2D fbo_texture;

out vec4 fragColor;

float sRGB_gamma_correct(float c)
{
    const float a = 0.055;
    if(c < 0.0031308)
        return 12.92*c;
    else
        return (1.0+a)*pow(c, 1.0/2.4) - a;
}

void main(void) {
    float gamma = 1/2.2;
    vec2 texcoord = f_texcoord;
    fragColor = texture(fbo_texture, texcoord);

/*
    if (texcoord.x<0.50) {
        fragColor.rgba = vec4(sRGB_gamma_correct(fragColor.r),
                              sRGB_gamma_correct(fragColor.g),
                              sRGB_gamma_correct(fragColor.b),
                              fragColor.a);
                              
    }
*/

}
