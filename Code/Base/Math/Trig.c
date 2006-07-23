
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
	return (float) asin(f);
}

float mathACos(__reg("fp0") float f)
{
	return (float) acos(f);
}

float mathATan(__reg("fp0") float f)
{
	return (float) atan(f);
}

float mathATan2(__reg("fp0") float x, __reg("fp1") float y)
{
	return (float) atan2(y, x);
}

