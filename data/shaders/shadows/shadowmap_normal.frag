/* Depth map generation

   @author Roberto Cano
*/
#version 330 core

void main()
{
	gl_FragDepth = gl_FragCoord.z;
}

