/* Utility shader to show depth map on screen

   @author Roberto Cano
*/
#version 330 core

in vec2 f_texcoord;
uniform sampler2DShadow u_depthMap;

out vec4 fragColor;

float zFar  = 100.0f;
float zNear = 1.0f;

float LinearizeDepth(float z_b)
{
    float z_n = 2.0 * z_b - 1.0;
    float z_e = 2.0 * zNear * zFar / (zFar + zNear - z_n * (zFar - zNear));

    return z_e;
}

void main(void) {
	float shadow = texture(u_depthMap, vec3(f_texcoord, 1.0f));
	fragColor = vec4(shadow, shadow, shadow, 1.0f);
}
