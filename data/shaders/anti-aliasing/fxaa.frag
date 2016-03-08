//
// Roberto Cano (http://www.robertocano.es)
//
#version 330 core

in vec2 f_texcoord;
uniform sampler2D fbo_texture;
uniform vec2 f_rpcFrame;

out vec4 fragColor;

//#define DEBUG_LOCAL_CONTRAST
//#define DEBUG_VERTHOR_TEST
//#define DEBUG_PIXEL_PAIR
//#define FXAA_DEBUG_NEGPOS
//#define FXAA_DEBUG_OFFSET

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

float FXAA_SEARCH_STEPS = 5;
float FXAA_SEARCH_ACCELERATION = 1;
float FXAA_SEARCH_THRESHOLD = 1.0f/4.0f;

float sRGB2Linear(float c) {
    if (c <= 0.04045) {
        return c/12.92;
    }
    return pow((c + 0.055)/1.055, 2.4);
}

float FxaaLuma(vec3 rgb) {
    return rgb.y * (0.587/0.299) + rgb.x;
}
vec3 FxaaLerp3(vec3 a, vec3 b, float amountOfA) {
    return (vec3(-amountOfA) * b) + ((a * vec3(amountOfA)) + b);
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

    /* Vertical/horizontal edge test */
    vec3 rgbNW = textureOffset(fbo_texture, f_texcoord, ivec2( -1, -1)).rgb;
    vec3 rgbNE = textureOffset(fbo_texture, f_texcoord, ivec2(  1, -1)).rgb;
    vec3 rgbSW = textureOffset(fbo_texture, f_texcoord, ivec2( -1,  1)).rgb;
    vec3 rgbSE = textureOffset(fbo_texture, f_texcoord, ivec2(  1,  1)).rgb;

    float lumaNW = FxaaLuma(rgbNW);
    float lumaNE = FxaaLuma(rgbNE);
    float lumaSW = FxaaLuma(rgbSW);
    float lumaSE = FxaaLuma(rgbSE);

    float edgeVert =
        abs((0.25 * lumaNW) + (-0.5 * lumaN) + (0.25 * lumaNE)) +
        abs((0.50 * lumaW ) + (-1.0 * lumaM) + (0.50 * lumaE )) +
        abs((0.25 * lumaSW) + (-0.5 * lumaS) + (0.25 * lumaSE));
    float edgeHorz =
        abs((0.25 * lumaNW) + (-0.5 * lumaW) + (0.25 * lumaSW)) +
        abs((0.50 * lumaN ) + (-1.0 * lumaM) + (0.50 * lumaS )) +
        abs((0.25 * lumaNE) + (-0.5 * lumaE) + (0.25 * lumaSE));
    bool horzSpan = edgeHorz >= edgeVert;

#ifdef DEBUG_VERTHOR_TEST
    if (horzSpan) {
        return vec3(1.0f, 0.75f, 0.0f);
    } else {
        return vec3(0.0f, 0.5f, 1.0f);
    }
#endif

    /* Check if span is horizontal or vertical and then
       reassign the variables so we don't have to worry
       about names */
    if (!horzSpan) {
        lumaN = lumaW;
        lumaS = lumaE;
    }

    /* Calculate the gradient */
    float gradientN = abs(lumaN - lumaM);
    float gradientS = abs(lumaS - lumaM);

    bool pairN = gradientN >= gradientS;

#ifdef DEBUG_PIXEL_PAIR
    if (pairN) {
        return vec3(0.0f, 0.0f, 1.0f);
    } else {
        return vec3(0.0f, 1.0f, 0.0f);
    }
#endif

    /* Assume advancing on the N or W direction */
    float lengthSign = horzSpan ? -f_rpcFrame.y : -f_rpcFrame.x;

    /* Now pick only the pixel with the highest gradient */
    if (!pairN) {
        lumaN = lumaS;
        gradientN = gradientS;
        lengthSign *= -1.0f;
    }

    /* Calculate the next position to check */
    vec2 posN, posP;
    posN.x = f_texcoord.x + (horzSpan ? 0.0f : lengthSign * 0.5f);
    posN.y = f_texcoord.y + (horzSpan ? lengthSign * 0.5f : 0.0f);
    posP = posN;

#if 0
    if (posN.x < f_texcoord.x) return vec3(1.0, 0.0, 0.0);
    if (posN.x > f_texcoord.x) return vec3(0.0, 1.0, 0.0);
    if (posN.y < f_texcoord.y) return vec3(0.0, 0.0, 1.0);
    if (posN.y > f_texcoord.y) return vec3(1.0, 0.0, 1.0);
    return vec3(1.0, 1.0, 1.0);
#endif

    /* Search limit: we stop searching when the gradient is
       greater than this meaning the edge ends there */
    gradientN *= FXAA_SEARCH_THRESHOLD;

    /* Calculate the advance in the negative and positive
       directions along the edge */
    vec2 offNP = horzSpan ? vec2(f_rpcFrame.x, 0.0f) : vec2(0.0f, f_rpcFrame.y);
    
    /* End-of-edge search */
    float lumaEndN = lumaN;
    float lumaEndP = lumaN;
    bool doneN = false;
    bool doneP = false;

    for (int i=0; i<FXAA_SEARCH_STEPS; ++i) {
        if (!doneN) {
            posN -= offNP;
            lumaEndN = FxaaLuma(texture(fbo_texture, posN).rgb);
            doneN = (lumaEndN - lumaN) >= gradientN;
        }
        if (!doneP) {
            posP += offNP;
            lumaEndP = FxaaLuma(texture(fbo_texture, posP).rgb);
            doneP = (lumaEndP - lumaN) >= gradientN;
        }
        if (doneN && doneP) {
            break;
        }
    }

    /* Calculate whether the center is in negative or in positive
       side of the search span */
    float dstN = horzSpan ? f_texcoord.x - posN.x : f_texcoord.y - posN.y;
    float dstP = horzSpan ? posP.x - f_texcoord.x : posP.y - f_texcoord.y;
    bool directionN = dstN < dstP;
#ifdef FXAA_DEBUG_NEGPOS
    if (directionN) return vec3(1.0f, 0.0f, 0.0f);
    else            return vec3(0.0f, 0.0f, 1.0f);
#endif

    lumaEndN = directionN ? lumaEndN : lumaEndP;

    /* Avoid filtering undesired cases */
    if (((lumaM - lumaN) < 0.0) == ((lumaEndN - lumaN) < 0.0)) {
        lengthSign = 0.0;
    }

    float spanLength = dstP + dstN;
    dstN = directionN ? dstN : dstP;

    /* Had to use 1.0 instead of 0.5 in the equation below, otherwise
       the antialiasing does not work properly. Maybe we need anisotropic
       filtering support? */
    float subPixelOffset = (1.0 + (dstN * (-1.0/spanLength))) * lengthSign;

#ifdef FXAA_DEBUG_OFFSET
    float ox = horzSpan ? 0.0 : subPixelOffset*2.0/f_rpcFrame.x;
    float oy = horzSpan ? subPixelOffset*2.0/f_rpcFrame.y : 0.0;
    float lumaO = lumaM/(1.0f + (0.587f/0.299f));
    if (ox < 0.0) return FxaaLerp3(vec3(lumaO), vec3(1.0f, 0.0f, 0.0f), -ox);
    if (ox > 0.0) return FxaaLerp3(vec3(lumaO), vec3(0.0f, 0.0f, 1.0f), ox);
    if (oy < 0.0) return FxaaLerp3(vec3(lumaO), vec3(1.0f, 0.6f, 0.2f), -oy);
    if (oy > 0.0) return FxaaLerp3(vec3(lumaO), vec3(0.2f, 0.6f, 1.0f), oy);
    return vec3(lumaO);
#endif

    vec3 rgbF = texture(fbo_texture, vec2(f_texcoord.x + (horzSpan ? 0.0f : subPixelOffset),
                                          f_texcoord.y + (horzSpan ? subPixelOffset : 0.0f))).rgb;
    return rgbF;
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
