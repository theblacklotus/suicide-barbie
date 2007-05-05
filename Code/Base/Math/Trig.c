
#include <Base/Math/Math.h>

#include <math.h>

float mathSin(__reg("fp0") float f)
{
	return sinf(f);
}

float mathCos(__reg("fp0") float f)
{
	return cosf(f);
}

float mathTan(__reg("fp0") float f)
{
	return tanf(f);
}

float mathASin(__reg("fp0") float f)
{
	return asinf(f);
}

float mathACos(__reg("fp0") float f)
{
	return acosf(f);
}

float mathATan(__reg("fp0") float f)
{
	return atanf(f);
}

float mathATan2(__reg("fp0") float x, __reg("fp1") float y)
{
	return atan2f(y, x);
}

