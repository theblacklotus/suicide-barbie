
#ifndef NEWAGE_BASE_MATH_QUAT_H
#define NEWAGE_BASE_MATH_QUAT_H

#include <Base/Common/Common.h>

typedef float float_t;
typedef struct QuatTag
{
	float_t x, y, z, w;
} Quat;


extern __asm float_t LengthQuat(__reg("a0") Quat *q0);
extern __asm float_t NormQuat(__reg("a0") Quat *q0);
extern __asm float_t DotQuat(__reg("a0") Quat *q0, __reg("a1") Quat *q1);

extern __asm float_t QuatLength(__reg("a5") Quat *q0);
extern __asm float_t QuatNorm(__reg("a5") Quat *q0);
extern __asm float_t QuatDot(__reg("a5") Quat *q0, __reg("a0") Quat *q1);

extern __asm void QuatCopy(__reg("a5") Quat *q, __reg("a0") Quat *q0);
extern __asm void QuatConjugate(__reg("a5") Quat *q, __reg("a0") Quat *q0);
extern __asm void QuatNegate(__reg("a5") Quat *q, __reg("a0") Quat *q0);
extern __asm void QuatAdd(__reg("a5") Quat *q, __reg("a0") Quat *q0, __reg("a1") Quat *q1);
extern __asm void QuatSub(__reg("a5") Quat *q, __reg("a0") Quat *q0, __reg("a1") Quat *q1);
extern __asm void QuatScale(__reg("a5") Quat *q, __reg("a0") Quat *q0, __reg("fp0") float_t scale);
extern __asm void QuatLinearCombine1(__reg("a5") Quat *q, __reg("a0") Quat *q0, __reg("fp0") float scale);
extern __asm void QuatLinearCombine2(__reg("a5") Quat *q, __reg("a0") Quat *q0, __reg("a1") Quat *q1, __reg("fp0") float s, __reg("fp1") float t);
extern __asm void QuatLinearCombine3(__reg("a5") Quat *q, __reg("a0") Quat *q0, __reg("a1") Quat *q1, __reg("a2") Quat *q2, __reg("fp0") float s, __reg("fp1") float t, __reg("fp2") float u);
extern __asm void QuatLinearCombine4(__reg("a5") Quat *q, __reg("a0") Quat *q0, __reg("a1") Quat *q1, __reg("a2") Quat *q2, __reg("a3") Quat *q3,
					__reg("fp0") float s, __reg("fp1") float t, __reg("fp2") float u, __reg("fp3") float v);
extern __asm void QuatSLinearCombine(__reg("a5") Quat *q, __reg("a0") Quat *q0, __reg("a0") Quat *q1, __reg("fp0") float t);
extern __asm void QuatMul(__reg("a5") Quat *q, __reg("a0") Quat *q0, __reg("a1") Quat *q1);
extern __asm void QuatDiv(__reg("a5") Quat *q, __reg("a0") Quat *q0, __reg("a1") Quat *q1);
extern __asm void QuatInvert(__reg("a5") Quat *q, __reg("a0") Quat *q0);
extern __asm void QuatNormalize(__reg("a5") Quat *q, __reg("a0") Quat *q0);

extern __asm void QuatSetZero(__reg("a5") Quat *q);
extern __asm void QuatSetOne(__reg("a5") Quat *q);
extern __asm void QuatSetIdentity(__reg("a5") Quat *q);
extern __asm void QuatSetFromScalars(__reg("a5") Quat *q, __reg("fp0") float_t x, __reg("fp1") float_t y, __reg("fp2") float_t z, float_t w);
extern __asm void QuatSetFromVec3Scalar(__reg("a5") Quat *q, __reg("a0") Vec3 *v0, __reg("fp0") float_t s);
extern __asm void QuatSetFromAxisAngle(__reg("a5") Quat *q, __reg("a0") Vec3 *v0, __reg("fp0") float_t angle);
extern __asm void QuatSetFromEulerAngle(__reg("a5") Quat* q, __reg("fp0") float xangle, __reg("fp1") float yangle, __reg("fp2") float zangle);


extern Quat Quat_sIdentity;


#endif
