/*
	Toon filter

	This filter can be applied to the whole screen to "toonify" the models.
	In order to do this the final z-buffer needs to be provided with the information
	of the rendered geometry on the screen. This information is used to detect borders

	In order to do that along the z-buffer the near and far planes must be provided
	in order to linearize the buffer properly

	The border color can be also provided as a uniform

	For the static configuration check the section below

	@author Roberto Cano (http://www.robertocano.es)
*/
#version 330 core

/* Configuration */

/* Enables/disables the built-in anti-alias. The anti-alias checks
   if a fragment belongs to the inner or outer side of an edge. If so
   the color on the other side (outer side for inner fragment, and
   viceversa) is accumulated for all the fragments surrounding the
   current fragment and an average is emmited */
#define ENABLE_ANTI_ALIAS 1

/* Size in pixels of the toon border. Be aware this border applies
   to all fragments at 'nearFragment' distance in the z-buffer, meaning
   the border can extend beyond the indicated value, but it is an estimation
   of the thickness of the average border */
#define BORDER_SIZE 2

/* Number of texels in the depth-buffer to offset to detect a true edge of the model.
   A true edge is defined as an edge of the model which is drawn agains a very far
   background. This very far background is defined by 'distantFragment'. If the
   fragment is considered as part of a true edge, it will be used to anti-alias
   the edges of the model
*/
#define ALIAS_SIZE  1

/* Toon level indicates the quality of the generated border. Allowed values are
   0, 1 and 2, the higher the better. 0 disables the border completely */
#define TOON_LEVEL  2

/* I/O, uniforms */
in vec2 f_texcoord;

uniform sampler2D fbo_texture;
uniform sampler2D depthTexture;
uniform float zNear;
uniform float zFar;
uniform float nearFrag;
uniform float distantFrag;
uniform vec4 borderColor;

out vec4 fragColor;

#if ENABLE_ANTI_ALIAS
#define CheckBorder(db, daa, dm, cm) \
	if (db - dm > nearFrag) { \
		fragColor = borderColor; \
		if (abs(daa - dm) > distantFrag) { \
			numColors++; \
			aaColor += cm; \
		} \
	}
#else
#define CheckBorder(db, daa, dm, cm) \
    if (db - dm > nearFrag) { \
		fragColor = borderColor;  \
	}
#endif // ENABLE_ANTI_ALIAS

float LinearizeDepth(float z_b)
{
    float z_n = 2.0 * z_b - 1.0;
    float z_e = 2.0 * zNear * zFar / (zFar + zNear - z_n * (zFar - zNear));

    return z_e;
}

void main(void) {
#if ENABLE_ANTI_ALIAS
    int numColors = 0;
    vec4 aaColor = vec4(0.0, 0.0, 0.0, 0.0);

    float depthN1 = LinearizeDepth(textureOffset(depthTexture, f_texcoord, ivec2(0, -ALIAS_SIZE)).x);
    float depthS1 = LinearizeDepth(textureOffset(depthTexture, f_texcoord, ivec2(0, ALIAS_SIZE)).x);
    float depthE1 = LinearizeDepth(textureOffset(depthTexture, f_texcoord, ivec2(ALIAS_SIZE, 0)).x);
    float depthW1 = LinearizeDepth(textureOffset(depthTexture, f_texcoord, ivec2(-ALIAS_SIZE, 0)).x);
    float depthNW1 = LinearizeDepth(textureOffset(depthTexture, f_texcoord, ivec2(-ALIAS_SIZE, -ALIAS_SIZE)).x);
    float depthNE1 = LinearizeDepth(textureOffset(depthTexture, f_texcoord, ivec2(ALIAS_SIZE, -ALIAS_SIZE)).x);
    float depthSW1 = LinearizeDepth(textureOffset(depthTexture, f_texcoord, ivec2(-ALIAS_SIZE, ALIAS_SIZE)).x);
    float depthSE1 = LinearizeDepth(textureOffset(depthTexture, f_texcoord, ivec2(ALIAS_SIZE, ALIAS_SIZE)).x);

    vec4 colorN = textureOffset(fbo_texture, f_texcoord, ivec2(0, -1));
    vec4 colorS = textureOffset(fbo_texture, f_texcoord, ivec2(0, 1));
    vec4 colorE = textureOffset(fbo_texture, f_texcoord, ivec2(1, 0));
    vec4 colorW = textureOffset(fbo_texture, f_texcoord, ivec2(-1, 0));
    vec4 colorNW = textureOffset(fbo_texture, f_texcoord, ivec2(-1, -1));
    vec4 colorNE = textureOffset(fbo_texture, f_texcoord, ivec2(1, -1));
    vec4 colorSW = textureOffset(fbo_texture, f_texcoord, ivec2(-1, 1));
    vec4 colorSE = textureOffset(fbo_texture, f_texcoord, ivec2(1, 1));
#endif
    float depthM = LinearizeDepth(textureOffset(depthTexture, f_texcoord, ivec2(0, 0)).x);
    float depthN = LinearizeDepth(textureOffset(depthTexture, f_texcoord, ivec2(0, -BORDER_SIZE)).x);
    float depthS = LinearizeDepth(textureOffset(depthTexture, f_texcoord, ivec2(0, BORDER_SIZE)).x);
    float depthE = LinearizeDepth(textureOffset(depthTexture, f_texcoord, ivec2(BORDER_SIZE, 0)).x);
    float depthW = LinearizeDepth(textureOffset(depthTexture, f_texcoord, ivec2(-BORDER_SIZE, 0)).x);
    float depthNW = LinearizeDepth(textureOffset(depthTexture, f_texcoord, ivec2(-BORDER_SIZE, -BORDER_SIZE)).x);
    float depthNE = LinearizeDepth(textureOffset(depthTexture, f_texcoord, ivec2(BORDER_SIZE, -BORDER_SIZE)).x);
    float depthSW = LinearizeDepth(textureOffset(depthTexture, f_texcoord, ivec2(-BORDER_SIZE, BORDER_SIZE)).x);
    float depthSE = LinearizeDepth(textureOffset(depthTexture, f_texcoord, ivec2(BORDER_SIZE, BORDER_SIZE)).x);

    fragColor = textureOffset(fbo_texture, f_texcoord, ivec2(0, 0));

#if TOON_LEVEL >= 1
	CheckBorder(depthN, depthN1, depthM, colorN);
	CheckBorder(depthS, depthS1, depthM, colorS);
	CheckBorder(depthE, depthE1, depthM, colorE);
	CheckBorder(depthW, depthW1, depthM, colorW);
#endif
#if TOON_LEVEL >= 2
	CheckBorder(depthNE, depthNE1, depthM, colorNE);
	CheckBorder(depthNW, depthNW1, depthM, colorNW);
	CheckBorder(depthSE, depthSE1, depthM, colorSE);
	CheckBorder(depthSW, depthSW1, depthM, colorSW);
#endif

#if ENABLE_ANTI_ALIAS
	if (numColors > 0) {
		fragColor = aaColor*numColors/16.0;
		fragColor.a = 1.0;
	}
#endif
}
