
#include <Base/Math/Math.h>

#include <math.h>

s32 mathISqrt(__reg("d0") s32 x)
{
	return (s32) sqrtf((float) x);
}

float mathSqrt(__reg("fp0") float x)
{
	return (float) sqrtf(x);
}
