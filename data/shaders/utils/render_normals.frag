/*
   Helps rendering the normals of the model

   @author Roberto Cano
*/
#version 330 core

in vec4 io_vertexColor;
out vec4 o_color;

void main() { o_color = io_vertexColor; }
