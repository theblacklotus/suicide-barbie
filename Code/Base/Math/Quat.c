
#include <Base/Math/Math.h>
#include <Base/Math/Quat.h>

float_t QuatDot(__reg("a5") Quat *q0, __reg("a0") Quat *q1)
{
	return q0->x * q1->x + q0->y * q1->y + q0->z * q1->z + q0->w * q1->w;
}

void QuatConjugate(__reg("a5") Quat *q, __reg("a0") Quat *q0)
{
	q->x = -q0->x;
	q->y = -q0->y;
	q->z = -q0->z;
	q->w =  q0->w;
}

void QuatNegate(__reg("a5") Quat *q, __reg("a0") Quat *q0)
{
	q->x = -q0->x;
	q->y = -q0->y;
	q->z = -q0->z;
	q->w = -q0->w;
}

void QuatSetFromAxisAngle(__reg("a5") Quat *q, __reg("a0") Vec3 *v0, __reg("fp0") float_t angle)
{
	float_t l = v0->x*v0->x + v0->x*v0->x + v0->z*v0->z;
	if( l > (float_t)0.0 )
	{
		angle *= (float_t)0.5;
		q->w = (float_t)mathCos(angle);
		l = (float_t)mathSin(angle) * (float_t)1.0 / mathSqrt(l);
		q->x = v0->x * l;
		q->y = v0->y * l;
		q->z = v0->z * l;
	}
	else
	{
		q->x = (float_t)0;
		q->y = (float_t)0;
		q->z = (float_t)0;
		q->w = (float_t)1;
	}
}

void QuatSetFromEulerAngle(__reg("a5") Quat* q, __reg("fp0") float xangle, __reg("fp1") float yangle, __reg("fp2") float zangle)
{
	float_t yaw = yangle;
	float_t pitch = xangle;
	float_t roll = zangle;

	float_t cr, cp, cy, sr, sp, sy, cpcy, spsy;

	// float_t dToR = Math_PI / (float_t)180;

	// calculate trig identities
	cr = mathCos ( roll  / (float_t)2 );
	cp = mathCos ( pitch / (float_t)2 );
	cy = mathCos ( yaw   / (float_t)2 );

	sr = mathSin ( roll  / (float_t)2 );
	sp = mathSin ( pitch / (float_t)2 );
	sy = mathSin ( yaw   / (float_t)2 );
	                            
	cpcy = cp * cy;
	spsy = sp * sy;

	// now create the scalar
	q->w = cr * cpcy + sr * spsy;

	// and the vector part
	q->x = sr * cpcy - cr * spsy;
	q->y = cr * sp * cy + sr * cp * sy;
	q->z = cr * cp * sy - sr * sp * cy;
}

void QuatSLinearCombine(__reg("a5") Quat *q, __reg("a0") Quat *q0, __reg("a0") Quat *q1, __reg("fp0") float t)
{
	Quat e;
	float_t cosom, t0, t1;
	
	cosom = QuatDot(q0, q1);

	e = *q1;
    if( cosom < (float_t)0.0 )
    {
		cosom = -cosom;
		QuatNegate(&e, &e);
    }
 
    if( cosom < (float_t)0.9999 )
    {
        float_t omega = mathACos(cosom);
        float_t rcpSinom = (float_t)1.0 / mathSin(omega);
        t0 = mathSin((1.0f - (float)t) * omega) * rcpSinom;
        t1 = mathSin((float)t * omega) * rcpSinom;
    }
    else
    {
		t0 = (float_t)1.0 - t;
		t1 = t;
    }

	q->x = q0->x * t0 + e.x * t1;
	q->y = q0->y * t0 + e.y * t1;
	q->z = q0->z * t0 + e.z * t1;
	q->w = q0->w * t0 + e.w * t1;
}

void QuatMul(__reg("a5") Quat *q, __reg("a0") Quat *q0, __reg("a1") Quat *q1)
{
	q->x = q0->w*q1->x + q0->x*q1->w + q0->y*q1->z - q0->z*q1->y;
	q->y = q0->w*q1->y + q0->y*q1->w + q0->z*q1->x - q0->x*q1->z;
	q->z = q0->w*q1->z + q0->z*q1->w + q0->x*q1->y - q0->y*q1->x;
	q->w = q0->w*q1->w - q0->x*q1->x - q0->y*q1->y - q0->z*q1->z; 
}
