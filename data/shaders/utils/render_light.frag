/*
   Helps rendering a light position in the camera view for debugging

   @author Roberto Cano
*/
#version 330 core

uniform vec3 u_lightColor;
uniform uint u_lightNumber;

in vec2 io_intensity;
out vec4 o_color;

/* Rendered area is a 1.0x1.0 square, with the center at (0.5, 0.5),
   the radius of the circle enclosed in that square is 0.5 */
#define MAX_RADIUS 0.5f

void main()
{
    /* Linear intensity is the length of the fragment with respect
       to the circle center */
    float intensity = length(io_intensity - vec2(MAX_RADIUS, MAX_RADIUS)) / MAX_RADIUS;

    /* If outside of the light circle, just output alpha 0.0,
       otherwise alpha is equivalent to the loss of light power
       along the direction of the ray of light, which is quadratic.
       Because we limit the linear intensity to the maximum radius,
       the maximum value on the numerator is MAX_RADIUS*MAX_RADIUS.
       Clamping the value to the range [0.0f, 1.0f] will make all the
       values of intensity bigger than MAX_RADIUS have an alpha of 0.0f
     */
    float alpha = clamp(1.0f - intensity * intensity, 0.0f, 1.0f);

    o_color = vec4(u_lightColor, alpha);

    gl_FragDepth = gl_FragCoord.z - (u_lightNumber * gl_FragCoord.w * 0.01);
}
