/* Phong reflection model implemented following the explanation
   at https://en.wikipedia.org/wiki/Phong_reflection_model and
   http://sunandblackcat.com/tipFullView.php?l=eng&topicid=30&topic=Phong-Lighting

   Fully implemented using the formulas, no code has been
   copied from any other source

   @author Roberto Cano
*/
#version 330 core

/* Input from vertex shader */
in vec3 io_fragVertex;

/* Output of this shader */
out vec4 o_color;

void main()
{
    o_color = vec3(io_fragVertex.z/1000.0f);
}

