//
// Roberto Cano (http://www.robertocano.es)
//
#version 330 core

in vec2 f_texcoord;
uniform sampler2D fbo_texture;
uniform sampler2D depthTexture;

float zNear = 0.1f;
float zFar  = 1000.0f;
float gradient = 2.0 * (zFar - zNear) / 1000.0;
float border = 8.0*gradient;
vec4  borderColor = vec4(0.0, 0.0, 1.0, 1.0);

out vec4 fragColor;

#define KERNEL_SIZE 2

float LinearizeDepth(float z_b)
{
    float z_n = 2.0 * z_b - 1.0;
    float z_e = 2.0 * zNear * zFar / (zFar + zNear - z_n * (zFar - zNear));

    return z_e;
}

void main(void) {
    float depthM = LinearizeDepth(textureOffset(depthTexture, f_texcoord, ivec2(0, 0)).r);
    float depthN = LinearizeDepth(textureOffset(depthTexture, f_texcoord, ivec2(0, -KERNEL_SIZE)).r);
    float depthS = LinearizeDepth(textureOffset(depthTexture, f_texcoord, ivec2(0, KERNEL_SIZE)).r);
    float depthE = LinearizeDepth(textureOffset(depthTexture, f_texcoord, ivec2(KERNEL_SIZE, 0)).r);
    float depthW = LinearizeDepth(textureOffset(depthTexture, f_texcoord, ivec2(-KERNEL_SIZE, 0)).r);
    float depthNW = LinearizeDepth(textureOffset(depthTexture, f_texcoord, ivec2(-KERNEL_SIZE, -KERNEL_SIZE)).r);
    float depthNE = LinearizeDepth(textureOffset(depthTexture, f_texcoord, ivec2(KERNEL_SIZE, -KERNEL_SIZE)).r);
    float depthSW = LinearizeDepth(textureOffset(depthTexture, f_texcoord, ivec2(-KERNEL_SIZE, KERNEL_SIZE)).r);
    float depthSE = LinearizeDepth(textureOffset(depthTexture, f_texcoord, ivec2(KERNEL_SIZE, KERNEL_SIZE)).r);

    vec4 colorM = textureOffset(fbo_texture, f_texcoord, ivec2(0, 0));
    vec4 colorN = textureOffset(fbo_texture, f_texcoord, ivec2(0, -KERNEL_SIZE));
    vec4 colorS = textureOffset(fbo_texture, f_texcoord, ivec2(0, KERNEL_SIZE));
    vec4 colorE = textureOffset(fbo_texture, f_texcoord, ivec2(KERNEL_SIZE, 0));
    vec4 colorW = textureOffset(fbo_texture, f_texcoord, ivec2(-KERNEL_SIZE, 0));
    vec4 colorNW = textureOffset(fbo_texture, f_texcoord, ivec2(-KERNEL_SIZE, -KERNEL_SIZE));
    vec4 colorNE = textureOffset(fbo_texture, f_texcoord, ivec2(KERNEL_SIZE, -KERNEL_SIZE));
    vec4 colorSW = textureOffset(fbo_texture, f_texcoord, ivec2(-KERNEL_SIZE, KERNEL_SIZE));
    vec4 colorSE = textureOffset(fbo_texture, f_texcoord, ivec2(KERNEL_SIZE, KERNEL_SIZE));

    int numColors = 0;
    vec4 aaColor = vec4(0.0, 0.0, 0.0, 0.0);

#if 0
	if ((depthM - depthN) > border) {
        numColors++;
        aaColor += colorN;
        aaColor = vec4(1.0, 0.0, 0.0, 1.0);
    }
#endif
	if ((depthM - depthS) > border) {
        numColors++;
        aaColor += colorS;
        aaColor = vec4(1.0, 0.0, 0.0, 1.0);
    }
#if 0
	if ((depthM - depthE) > border) {
        numColors++;
        aaColor += colorE;
    }
	if ((depthM - depthW) > border) {
        numColors++;
        aaColor += colorW;
    }
	if ((depthM - depthNE) > border) {
        numColors++;
        aaColor += colorNE;
    }
	if ((depthM - depthNW) > border) {
        numColors++;
        aaColor += colorNW;
    }
	if ((depthM - depthSE) > border) {
        numColors++;
        aaColor += colorSE;
    }
	if ((depthM - depthSW) > border) {
        numColors++;
        aaColor += colorSW;
    }
#endif
    if (numColors == 0 &&
         (abs(depthN - depthM) > gradient ||
          abs(depthS - depthM) > gradient ||
          abs(depthE - depthM) > gradient ||
          abs(depthW - depthM) > gradient ||
          abs(depthNW - depthM) > gradient ||
          abs(depthNE - depthM) > gradient ||
          abs(depthSW - depthM) > gradient ||
          abs(depthSE - depthM) > gradient)) {
        fragColor = borderColor;
    } else {
        if (numColors == 0) {
            fragColor = colorM;
        } else {
            fragColor = mix(borderColor, aaColor, numColors/9.0f);
            fragColor.a = colorM.a;
            fragColor = aaColor;
        }
    }
}
