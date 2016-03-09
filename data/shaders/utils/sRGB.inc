/*
    Formulas implemented from the OpenGL standard

    @author Roberto Cano <http://www.robertocano.es>
*/
float sRGB2Linear(float c) {
    if (c <= 0.04045) {
        return c/12.92;
    }
    return pow((c + 0.055)/1.055, 2.4);
}

float Linear2sRGB(float c) {
    if (c < 0.0031308) {
        return c*12.92;
    }
    return 1.055*pow(c, 0.41666) - 0.055;
}
