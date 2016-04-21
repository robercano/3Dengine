/* Phong reflection model implemented following the explanation
   at https://en.wikipedia.org/wiki/Phong_reflection_model and
   http://sunandblackcat.com/tipFullView.php?l=eng&topicid=30&topic=Phong-Lighting

   Fully implemented using the formulas, no code has been
   copied from any other source

   @author Roberto Cano
*/
#version 330 core

/* Output of this shader */
out vec4 o_color;

float zNear = 0.1f;
float zFar = 1000.0f;

float LinearizeDepth(float z_b)
{
    float z_n = 2.0 * z_b - 1.0;
    float z_e = 2.0 * zNear * zFar / (zFar + zNear - z_n * (zFar - zNear));

    return z_e/(zFar-zNear);
}

void main()
{
    o_color = vec4(vec3(LinearizeDepth(gl_FragCoord.z)), 1.0);
}

