/* Utility shader to show depth map on screen

   @author Roberto Cano
*/
#version 330 core

in vec2 f_texcoord;
uniform sampler2D u_depthMap;

out vec4 fragColor;

float zFar  = 1.0f;
float zNear = 0.2f;

float LinearizeDepth(float z_b)
{
    float z_n = 2.0 * z_b - 1.0;
    float z_e = 2.0 * zNear * zFar / (zFar + zNear - z_n * (zFar - zNear));

    return z_e;
}

void main(void) {
    fragColor = vec4( vec3( LinearizeDepth(texture(u_depthMap, f_texcoord).x) ), 1.0f);
}
