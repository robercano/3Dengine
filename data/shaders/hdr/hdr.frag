//
// Roberto Cano (http://www.robertocano.es)
//
#version 330 core

in vec2 f_texcoord;
out vec4 fragColor;

uniform sampler2D fbo_texture;
uniform float u_exposure;
uniform bool u_tonemapping;


void main(void) {
    float gamma = 2.2;

    vec4 hdrColor = texture(fbo_texture, f_texcoord);

    if (u_tonemapping == true) {
        // Reinhard tone mapping
        //vec3 mapped = hdrColor.rgb / (hdrColor.rgb + vec3(1.0));
        vec3 mapped = vec3(1.0) - exp(-hdrColor.rgb * u_exposure);

        // Gamma correction
        //mapped = pow(mapped, vec3(1.0 / gamma));
        //vec3 mapped = hdrColor.rgb;
        fragColor = vec4(mapped, hdrColor.a);
    } else {
        fragColor = hdrColor;
    }
}
