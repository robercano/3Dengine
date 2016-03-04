//
// Roberto Cano (http://www.robertocano.es)
//
#version 330 core

in vec2 f_texcoord;
uniform sampler2D fbo_texture;

out vec4 fragColor;

#define DEBUG_LOCAL_CONTRAST

/*
    The minimum amount of local contrast required to apply algorithm.
        1/3 – too little
        1/4 – low quality
        1/8 – high quality
        1/16 – overkill
*/
float FXAA_EDGE_THRESHOLD = 1.0/3.0;

/*
    Trims the algorithm from processing darks.
        1/32 – visible limit
        1/16 – high quality
        1/12 – upper limit (start of visible unfiltered edges)
*/
float FXAA_EDGE_THRESHOLD_MIN = 1.0/32.0;

float sRGB2Linear(float c) {
    if (c <= 0.04045) {
        return c/12.92;
    }
    return pow((c + 0.055)/1.055, 2.4);
}

float FxaaLuma(vec3 rgb) {
    return rgb.y * (0.587/0.299) + rgb.x;
}

vec3 FxaaFilter() {
    /* Local contrast check */
    vec3 rgbN = textureOffset(fbo_texture, f_texcoord, ivec2( 0, -1)).rgb;
    vec3 rgbW = textureOffset(fbo_texture, f_texcoord, ivec2(-1,  0)).rgb;
    vec3 rgbM = textureOffset(fbo_texture, f_texcoord, ivec2( 0,  0)).rgb;
    vec3 rgbE = textureOffset(fbo_texture, f_texcoord, ivec2( 1,  0)).rgb;
    vec3 rgbS = textureOffset(fbo_texture, f_texcoord, ivec2( 0,  1)).rgb;

    float lumaN = FxaaLuma(rgbN);
    float lumaW = FxaaLuma(rgbW);
    float lumaM = FxaaLuma(rgbM);
    float lumaE = FxaaLuma(rgbE);
    float lumaS = FxaaLuma(rgbS);

    float rangeMin = min(lumaM, min(min(lumaN, lumaW), min(lumaS, lumaE)));
    float rangeMax = max(lumaM, max(max(lumaN, lumaW), max(lumaS, lumaE)));

    float range = rangeMax - rangeMin;

    if (range < max(FXAA_EDGE_THRESHOLD_MIN, rangeMax * FXAA_EDGE_THRESHOLD)) {
        return rgbM;
    }

#ifdef DEBUG_LOCAL_CONTRAST
    return vec3(1.0, 0.0, 0.0);
#endif

}

void main(void) {
    vec3 color = FxaaFilter();

    /* sRGB to linear conversion, OpenGL does not support
       the shader outputting sRGB when GL_FRAMEBUFFER_SRGB is
       enabled  */
    fragColor.rgba = vec4(sRGB2Linear(color.r),
                          sRGB2Linear(color.g),
                          sRGB2Linear(color.b),
                          1.0);
}
