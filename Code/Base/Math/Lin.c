
#include <Base/Math/Math.h>
#include <Base/Math/Quat.h>

float Vec3_dot(__reg("a1") Vec3* v0, __reg("a0") Vec3* v1)
{
	return v0->x * v1->x + v0->y * v1->y + v0->z * v1->z;
}

float Vec3_length(__reg("a0") Vec3* v0)
{
	return (float) mathSqrt(Vec3_dot(v0, v0));
}

void Vec3_copy(__reg("a1") Vec3* v, __reg("a0") Vec3* v0)
{
	v->x = v0->x;
	v->y = v0->y;
	v->z = v0->z;
}

void Vec3_neg(__reg("a1") Vec3* v, __reg("a0") Vec3* v0)
{
	v->x = -v0->x;
	v->y = -v0->y;
	v->z = -v0->z;
}

void Vec3_add(__reg("a2") Vec3* v, __reg("a0") Vec3* v0, __reg("a1") Vec3* v1)
{
	v->x = v0->x + v1->x;
	v->y = v0->y + v1->y;
	v->z = v0->z + v1->z;
}

void Vec3_sub(__reg("a2") Vec3* v, __reg("a0") Vec3* v0, __reg("a1") Vec3* v1)
{
	v->x = v0->x - v1->x;
	v->y = v0->y - v1->y;
	v->z = v0->z - v1->z;
}

void Vec3_scale(__reg("a1") Vec3* v, __reg("a0") Vec3* v0, __reg("fp0") float scale)
{
	v->x = scale * v0->x;
	v->y = scale * v0->y;
	v->z = scale * v0->z;
}

void Vec3_scaleXYZ(__reg("a1") Vec3* v, __reg("a0") Vec3* v0, __reg("fp0") float xscale, __reg("fp1") float yscale, __reg("fp2") float zscale)
{
	v->x = xscale * v0->x;
	v->y = yscale * v0->y;
	v->z = zscale * v0->z;
}

void Vec3_cross(__reg("a2") Vec3* v, __reg("a0") Vec3* v0, __reg("a1") Vec3* v1)
{
	Vec3 temp;
	temp.x = v0->y * v1->z - v0->z * v1->y;
	temp.y = v0->z * v1->x - v0->x * v1->z;
	temp.z = v0->x * v1->y - v0->y * v1->x;
	*v = temp;
}

void Vec3_normalize(__reg("a1") Vec3* v, __reg("a0") Vec3* v0)
{
	float len = Vec3_length(v0);
	if (len > 0.f)
		Vec3_scale(v, v0, 1.f / len);
}

void Vec3_setZero(__reg("a0") Vec3* v)
{
	v->x = 0;
	v->y = 0;
	v->z = 0;
}

void Vec3_setXYZ(__reg("a0") Vec3* v, __reg("fp0") float x, __reg("fp1") float y, __reg("fp2") float z)
{
	v->x = x;
	v->y = y;
	v->z = z;
}

void Vec3_setMat33MulVec3(__reg("a2") Vec3* v, __reg("a0") Mat33* m0, __reg("a1") Vec3* v0)
{
	Vec3 temp;
	temp.x = m0->Row[0].x * v0->x + m0->Row[0].y * v0->y + m0->Row[0].z * v0->z;
	temp.y = m0->Row[1].x * v0->x + m0->Row[1].y * v0->y + m0->Row[1].z * v0->z;
	temp.z = m0->Row[2].x * v0->x + m0->Row[2].y * v0->y + m0->Row[2].z * v0->z;
	*v = temp;
}

void Vec3_setMat34RotVec3(__reg("a2") Vec3* v, __reg("a0") Mat34* t0, __reg("a1") Vec3* v0)
{
	Vec3_setMat33MulVec3(v, &t0->Rot, v0);
}

void Vec3_setMat34MulVec3(__reg("a2") Vec3* v, __reg("a0") Mat34* t0, __reg("a1") Vec3* v0)
{
	Vec3 temp;
	Vec3_setMat33MulVec3(&temp, &t0->Rot, v0);
	Vec3_add(v, &temp, &t0->Move);
}

void Mat33_copy(__reg("a1") Mat33* m, __reg("a0") Mat33* m0)
{
	Vec3_copy(&m->Row[0], &m0->Row[0]);
	Vec3_copy(&m->Row[1], &m0->Row[1]);
	Vec3_copy(&m->Row[2], &m0->Row[2]);
}

void Mat33_mul(__reg("a2") Mat33* m, __reg("a0") Mat33* m0, __reg("a1") Mat33* m1)
{
	Mat33 temp;
	int row;
	for (row = 0; row < 3; row++)
	{
		temp.Row[row].x = m0->Row[row].x * m1->Row[0].x + m0->Row[row].y * m1->Row[1].x + m0->Row[row].z * m1->Row[2].x;
		temp.Row[row].y = m0->Row[row].x * m1->Row[0].y + m0->Row[row].y * m1->Row[1].y + m0->Row[row].z * m1->Row[2].y;
		temp.Row[row].z = m0->Row[row].x * m1->Row[0].z + m0->Row[row].y * m1->Row[1].z + m0->Row[row].z * m1->Row[2].z;
	}

	Mat33_copy(m, &temp);
}

void Mat33_invertOrthogonal(__reg("a1") Mat33* m, __reg("a0") Mat33* m0)
{
	Mat33_transpose(m, m0);
}

void Mat33_transpose(__reg("a1") Mat33* m, __reg("a0") Mat33* m0)
{
	Mat33 temp;
	temp.Row[0].x = m0->Row[0].x;
	temp.Row[0].y = m0->Row[1].x;
	temp.Row[0].z = m0->Row[2].x;
	temp.Row[1].x = m0->Row[0].y;
	temp.Row[1].y = m0->Row[1].y;
	temp.Row[1].z = m0->Row[2].y;
	temp.Row[2].x = m0->Row[0].z;
	temp.Row[2].y = m0->Row[1].z;
	temp.Row[2].z = m0->Row[2].z;
	*m = temp;
}

void Mat33_setIdentity(__reg("a0") Mat33* m)
{
	Vec3_setXYZ(&m->Row[0], 1.f, 0.f, 0.f);
	Vec3_setXYZ(&m->Row[1], 0.f, 1.f, 0.f);
	Vec3_setXYZ(&m->Row[2], 0.f, 0.f, 1.f);
}

void Mat33_setRotQuat(__reg("a0") Mat33* m, __reg("a1") Quat* q)
{
// from ODE
// equations in "An Introduction
// to Physically Based Modeling: Rigid Body Simulation - 1: Unconstrained
// Rigid Body Dynamics" by David Baraff, Robotics Institute, Carnegie Mellon
// University, 1997.
	float qq1 = 2.0f * q->x * q->x;
	float qq2 = 2.0f * q->y * q->y;
	float qq3 = 2.0f * q->z * q->z;
	m->Row[0].x = 1 - qq2 - qq3;
	m->Row[0].y = 2*(q->x * q->y - q->w * q->z);
	m->Row[0].z = 2*(q->x * q->z + q->w * q->y);
	m->Row[1].x = 2*(q->x * q->y + q->w * q->z);
	m->Row[1].y = 1 - qq1 - qq3;
	m->Row[1].z = 2*(q->y * q->z - q->w * q->x);
	m->Row[2].x = 2*(q->x * q->z - q->w * q->y);
	m->Row[2].y = 2*(q->y * q->z + q->w * q->x);
	m->Row[2].z = 1 - qq1 - qq2;
}

void Mat34_mul(__reg("a2") Mat34* t, __reg("a0") Mat34* t0, __reg("a1") Mat34* t1)
{
	Vec3_setMat34MulVec3(&t->Move, t0, &t1->Move);
	Mat33_mul(&t->Rot, &t0->Rot, &t1->Rot);
}

void Mat34_invertOrthogonal(__reg("a1") Mat34* t, __reg("a0") Mat34* t0)
{
	Mat33_invertOrthogonal(&t->Rot, &t0->Rot);
	Vec3_neg(&t->Move, &t0->Move);
	Vec3_setMat33MulVec3(&t->Move, &t->Rot, &t->Move);
}

void Mat34_setIdentity(__reg("a0") Mat34* t)
{
	Mat33_setIdentity(&t->Rot);
	Vec3_setZero(&t->Move);
}
