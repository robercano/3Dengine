/*
   Helps rendering a light position in the camera view for debugging

   @author Roberto Cano
*/
#version 330 core

uniform vec3 u_boxColor;
out vec4 o_color;

void main() { o_color = vec4(u_boxColor, 1.0f); }
