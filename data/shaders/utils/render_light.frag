/*
   Helps rendering a light position in the camera view for debugging

   @author Roberto Cano
*/
#version 400 core

uniform vec3 u_lightColor;
out vec4 o_color;

void main()
{
	float intensity = length(gl_PointCoord - vec2(0.5f, 0.5f));
	intensity = clamp(1.0f - 4.0f*intensity*intensity, 0.0f, 1.0f);
	o_color = vec4(intensity * u_lightColor, 1.0f);
}
