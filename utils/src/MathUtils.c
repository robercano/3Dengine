/**
 * Misc funtions to aid different parts of the engine
 *
 * @author Roberto Cano
 */
#include "MathUtils.h"

uint32_t clp2(uint32_t x)
{
    x--;
    x |= (x>>1);
    x |= (x>>2);
    x |= (x>>4);
    x |= (x>>8);
    x |= (x>>16);
    return x+1;
}

float restrictAngle(float angle)
{
	if (angle >= 360.0)   angle -= 360.0;
	else if (angle < 0.0) angle += 360.0;
	return angle;
}
