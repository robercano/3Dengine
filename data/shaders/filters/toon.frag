//
// Roberto Cano (http://www.robertocano.es)
//
#version 330 core

in vec2 f_texcoord;
uniform sampler2D fbo_texture;
uniform sampler2D depthTexture;

out vec4 fragColor;

#define KERNEL_SIZE 2
#define GRADIENT 0.000005

float LinearizeDepth(float zoverw)
{
//	if (zoverw < 0.9999) {
//		return 0.9999;
//	}
	return zoverw;
#if 0
	float n = 0.01; // You had to change this
	float f = 4000.0; // and this
	
	float depth = (2.0 * n) / (f + n - zoverw * (f - n));

	if (depth < 0.001) {
		return 0.001;
	}
	return depth;
#endif
}
void main(void) {
    vec4 texel = texture(fbo_texture, f_texcoord);
    float colorM = LinearizeDepth(textureOffset(depthTexture, f_texcoord, ivec2(0, 0)).r);
    float colorN = LinearizeDepth(textureOffset(depthTexture, f_texcoord, ivec2(0, -KERNEL_SIZE)).r);
    float colorS = LinearizeDepth(textureOffset(depthTexture, f_texcoord, ivec2(0, KERNEL_SIZE)).r);
    float colorE = LinearizeDepth(textureOffset(depthTexture, f_texcoord, ivec2(KERNEL_SIZE, 0)).r);
    float colorW = LinearizeDepth(textureOffset(depthTexture, f_texcoord, ivec2(-KERNEL_SIZE, 0)).r);
    float colorNW = LinearizeDepth(textureOffset(depthTexture, f_texcoord, ivec2(-KERNEL_SIZE, -KERNEL_SIZE)).r);
    float colorNE = LinearizeDepth(textureOffset(depthTexture, f_texcoord, ivec2(KERNEL_SIZE, -KERNEL_SIZE)).r);
    float colorSW = LinearizeDepth(textureOffset(depthTexture, f_texcoord, ivec2(-KERNEL_SIZE, KERNEL_SIZE)).r);
    float colorSE = LinearizeDepth(textureOffset(depthTexture, f_texcoord, ivec2(KERNEL_SIZE, KERNEL_SIZE)).r);

	if (abs(colorN - colorM) > GRADIENT ||
        abs(colorS - colorM) > GRADIENT ||
        abs(colorE - colorM) > GRADIENT ||
        abs(colorW - colorM) > GRADIENT ||
        abs(colorNW - colorM) > GRADIENT ||
        abs(colorNE - colorM) > GRADIENT ||
        abs(colorSW - colorM) > GRADIENT ||
        abs(colorSE - colorM) > GRADIENT) {
		texel = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	}
        
	fragColor = texel;
}
