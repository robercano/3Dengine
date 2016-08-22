/*
    Utility shader to render just a solid color for the model

    @author Roberto Cano (http://www.robertocano.es)
*/
#version 330 core

/* Output color for the fragment */
uniform vec4 u_color;

out vec4 o_color;

void main() { o_color = u_color; }
