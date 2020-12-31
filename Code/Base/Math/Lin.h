
#ifndef NEWAGE_BASE_MATH_LIN_H
#define NEWAGE_BASE_MATH_LIN_H

#include <Base/Std/Std.h>

typedef struct Vec3Tag
{
	float x, y, z;
} Vec3;

typedef struct Mat33Tag
{
	Vec3 Row[3];
} Mat33;

typedef struct Mat34Tag
{
	Mat33 Rot;
	Vec3 Move;
} Mat34;

struct QuatTag;

extern float Vec3_dot(__reg("a1") Vec3* v0, __reg("a0") Vec3* v1);
extern float Vec3_crossZ(__reg("a2") Vec3* v0, __reg("a0") Vec3* v1);
extern float Vec3_length(__reg("a0") Vec3* v0);

extern void Vec3_copy(__reg("a1") Vec3* v, __reg("a0") Vec3* v0);
extern void Vec3_neg(__reg("a1") Vec3* v, __reg("a0") Vec3* v0);
extern void Vec3_add(__reg("a2") Vec3* v, __reg("a0") Vec3* v0, __reg("a1") Vec3* v1);
extern void Vec3_sub(__reg("a2") Vec3* v, __reg("a0") Vec3* v0, __reg("a1") Vec3* v1);
extern void Vec3_scale(__reg("a1") Vec3* v, __reg("a0") Vec3* v0, __reg("fp0") float scale);
extern void Vec3_scaleXY(__reg("a1") Vec3* v, __reg("a0") Vec3* v0, __reg("fp0") float xscale, __reg("fp1") float yscale);
extern void Vec3_scaleXYZ(__reg("a1") Vec3* v, __reg("a0") Vec3* v0, __reg("fp0") float xscale, __reg("fp1") float yscale, __reg("fp2") float zscale);
extern void Vec3_project2D(__reg("a1") Vec3* v, __reg("a0") Vec3* v0);
extern void Vec3_cross(__reg("a2") Vec3* v, __reg("a0") Vec3* v0, __reg("a1") Vec3* v1);
extern void Vec3_crossNormalize(__reg("a2") Vec3* v, __reg("a0") Vec3* v0, __reg("a1") Vec3* v1);
extern void Vec3_normalize(__reg("a1") Vec3* v, __reg("a0") Vec3* v0);
extern void Vec3_linearCombine1(__reg("a1") Vec3* v, __reg("a0") Vec3* v0, __reg("fp0") float scale);
extern void Vec3_linearCombine2(__reg("a2") Vec3* v, __reg("a0") Vec3* v0, __reg("a1") Vec3* v1, __reg("fp0") float s, __reg("fp1") float t);
extern void Vec3_linearCombine3(__reg("a3") Vec3* v, __reg("a0") Vec3* v0, __reg("a1") Vec3* v1, __reg("a2") Vec3* v2, __reg("fp0") float s, __reg("fp1") float t, __reg("fp2") float u);
extern void Vec3_linearCombine4(__reg("a4") Vec3* v, __reg("a0") Vec3* v0, __reg("a1") Vec3* v1, __reg("a2") Vec3* v2, __reg("a3") Vec3* v3,
					__reg("fp0") float s, __reg("fp1") float t, __reg("fp2") float u, __reg("fp3") float w);

extern void Vec3_setZero(__reg("a0") Vec3* v);
extern void Vec3_setXYZ(__reg("a0") Vec3* v, __reg("fp0") float x, __reg("fp1") float y, __reg("fp2") float z);
extern void Vec3_setMat33MulVec3(__reg("a2") Vec3* v, __reg("a0") Mat33* m0, __reg("a1") Vec3* v0);
extern void Vec3_setMat34RotVec3(__reg("a2") Vec3* v, __reg("a0") Mat34* t0, __reg("a1") Vec3* v0);
extern void Vec3_setMat34MulVec3(__reg("a2") Vec3* v, __reg("a0") Mat34* t0, __reg("a1") Vec3* v0);
extern void Vec3_setMat33CofactorRow(__reg("a2") Vec3* v, __reg("a0") Vec3* v0, __reg("a1") Vec3* v1);


extern float Mat33_determinant(__reg("a0") Mat33* m0);

extern void Mat33_copy(__reg("a1") Mat33* m, __reg("a0") Mat33* m0);
extern void Mat33_mul(__reg("a2") Mat33* m, __reg("a0") Mat33* m0, __reg("a1") Mat33* m1);
extern void Mat33_invertOrthogonal(__reg("a1") Mat33* m, __reg("a0") Mat33* m0);
extern void Mat33_transpose(__reg("a1") Mat33* m, __reg("a0") Mat33* m0);
extern void Mat33_invert(__reg("a1") Mat33* m, __reg("a0") Mat33* m0);

extern void Mat33_postScale(__reg("a1") Mat33* m, __reg("a0") Mat33* m0, __reg("fp0") float scale);
extern void Mat33_postScaleXYZ(__reg("a1") Mat33* m, __reg("a0") Mat33* m0, __reg("fp0") float scalex, __reg("fp1") float scaley, __reg("fp2") float scalez);

extern void Mat33_applyPreScale(__reg("a0") Mat33* m, __reg("fp0") float scale);
extern void Mat33_applyPostScale(__reg("a0") Mat33* m, __reg("fp0") float scale);
extern void Mat33_applyPreScaleXYZ(__reg("a0") Mat33* m, __reg("fp0") float scalex, __reg("fp1") float scaley, __reg("fp2") float scalez);
extern void Mat33_applyPostScaleXYZ(__reg("a0") Mat33* m, __reg("fp0") float scalex, __reg("fp1") float scaley, __reg("fp2") float scalez);
extern void Mat33_applyPreRotX(__reg("a0") Mat33* m, __reg("fp0") float angle);
extern void Mat33_applyPostRotX(__reg("a0") Mat33* m, __reg("fp0") float angle);
extern void Mat33_applyPreRotY(__reg("a0") Mat33* m, __reg("fp0") float angle);
extern void Mat33_applyPostRotY(__reg("a0") Mat33* m, __reg("fp0") float angle);
extern void Mat33_applyPreRotZ(__reg("a0") Mat33* m, __reg("fp0") float angle);
extern void Mat33_applyPostRotZ(__reg("a0") Mat33* m, __reg("fp0") float angle);
extern void Mat33_applyPreRotH(__reg("a0") Mat33* m, __reg("fp0") float angle);
extern void Mat33_applyPostRotH(__reg("a0") Mat33* m, __reg("fp0") float angle);
extern void Mat33_applyPreRotP(__reg("a0") Mat33* m, __reg("fp0") float angle);
extern void Mat33_applyPostRotP(__reg("a0") Mat33* m, __reg("fp0") float angle);
extern void Mat33_applyPreRotB(__reg("a0") Mat33* m, __reg("fp0") float angle);
extern void Mat33_applyPostRotB(__reg("a0") Mat33* m, __reg("fp0") float angle);
extern void Mat33_applyPreRotXYZ(__reg("a0") Mat33* m, __reg("fp0") float anglex, __reg("fp1") float angley, __reg("fp2") float anglez);
extern void Mat33_applyPostRotXYZ(__reg("a0") Mat33* m, __reg("fp0") float anglex, __reg("fp1") float angley, __reg("fp2") float anglez);
extern void Mat33_applyPreRotHPB(__reg("a0") Mat33* m, __reg("fp0") float heading, __reg("fp1") float pitch, __reg("fp2") float bank);
extern void Mat33_applyPostRotHPB(__reg("a0") Mat33* m, __reg("fp0") float heading, __reg("fp1") float pitch, __reg("fp2") float bank);

extern void Mat33_setIdentity(__reg("a0") Mat33* m);
extern void Mat33_setScale(__reg("a0") Mat33* m, __reg("fp0") float scale);
extern void Mat33_setScaleXYZ(__reg("a0") Mat33* m, __reg("fp0") float scalex, __reg("fp1") float scaley, __reg("fp2") float scalez);
extern void Mat33_setRotX(__reg("a0") Mat33* m, __reg("fp0") float xangle);
extern void Mat33_setRotY(__reg("a0") Mat33* m, __reg("fp0") float yangle);
extern void Mat33_setRotZ(__reg("a0") Mat33* m, __reg("fp0") float zangle);
extern void Mat33_setRotXYZ(__reg("a0") Mat33* m, __reg("fp0") float xangle, __reg("fp1") float yangle, __reg("fp2") float zangle);
extern void Mat33_setRotHPB(__reg("a0") Mat33* m, __reg("fp0") float heading, __reg("fp1") float pitch, __reg("fp2") float bank);
extern void Mat33_setRotQuat(__reg("a0") Mat33* m, __reg("a1") struct QuatTag* q);
//extern void Mat33_setAxisAngle(__reg("a2") Mat33* m, __reg("a0") Vec3* axis, __reg("fp0") float angle);


extern float Mat34_determinant(__reg("a0") Mat34* m0);

extern void Mat34_copy(__reg("a1") Mat34* t, __reg("a0") Mat34* t0);
extern void Mat34_mul(__reg("a2") Mat34* t, __reg("a0") Mat34* t0, __reg("a1") Mat34* t1);
extern void Mat34_mulMat33(__reg("a2") Mat34* t, __reg("a0") Mat34* t0, __reg("a1") Mat33* t1);
extern void Mat34_invert(__reg("a1") Mat34* t, __reg("a0") Mat34* t0);
extern void Mat34_invertOrthogonal(__reg("a1") Mat34* t, __reg("a0") Mat34* t0);

extern void Mat34_applyPreTranslation(__reg("a1") Mat34* m, __reg("a0") Vec3* v);
extern void Mat34_applyPostTranslation(__reg("a1") Mat34* m, __reg("a0") Vec3* v);
extern void Mat34_applyPreScale(__reg("a0") Mat34* m, __reg("fp0") float scale);
extern void Mat34_applyPostScale(__reg("a0") Mat34* m, __reg("fp0") float scale);
extern void Mat34_applyPreScaleXYZ(__reg("a0") Mat34* m, __reg("fp0") float scalex, __reg("fp1") float scaley, __reg("fp2") float scalez);
extern void Mat34_applyPostScaleXYZ(__reg("a0") Mat34* m, __reg("fp0") float scalex, __reg("fp1") float scaley, __reg("fp2") float scalez);
extern void Mat34_applyPreRotX(__reg("a0") Mat34* m, __reg("fp0") float angle);
extern void Mat34_applyPostRotX(__reg("a0") Mat34* m, __reg("fp0") float angle);
extern void Mat34_applyPreRotY(__reg("a0") Mat34* m, __reg("fp0") float angle);
extern void Mat34_applyPostRotY(__reg("a0") Mat34* m, __reg("fp0") float angle);
extern void Mat34_applyPreRotZ(__reg("a0") Mat34* m, __reg("fp0") float angle);
extern void Mat34_applyPostRotZ(__reg("a0") Mat34* m, __reg("fp0") float angle);
extern void Mat34_applyPreRotH(__reg("a0") Mat34* m, __reg("fp0") float angle);
extern void Mat34_applyPostRotH(__reg("a0") Mat34* m, __reg("fp0") float angle);
extern void Mat34_applyPreRotP(__reg("a0") Mat34* m, __reg("fp0") float angle);
extern void Mat34_applyPostRotP(__reg("a0") Mat34* m, __reg("fp0") float angle);
extern void Mat34_applyPreRotB(__reg("a0") Mat34* m, __reg("fp0") float angle);
extern void Mat34_applyPostRotB(__reg("a0") Mat34* m, __reg("fp0") float angle);
extern void Mat34_applyPreRotXYZ(__reg("a0") Mat34* m, __reg("fp0") float anglex, __reg("fp1") float angley, __reg("fp2") float anglez);
extern void Mat34_applyPostRotXYZ(__reg("a0") Mat34* m, __reg("fp0") float anglex, __reg("fp1") float angley, __reg("fp2") float anglez);
extern void Mat34_applyPreRotHPB(__reg("a0") Mat34* m, __reg("fp0") float heading, __reg("fp1") float pitch, __reg("fp2") float bank);
extern void Mat34_applyPostRotHPB(__reg("a0") Mat34* m, __reg("fp0") float heading, __reg("fp1") float pitch, __reg("fp2") float bank);

extern void Mat34_setIdentity(__reg("a0") Mat34* t);
extern void Mat34_setMat33Vec3(__reg("a0") Mat34* t, __reg("a0") Mat33* rot, __reg("a1") Vec3* move);
extern void Mat34_setRotXYZMove(__reg("a1") Mat34* t, __reg("fp1") float xangle, __reg("fp0") float yangle, __reg("fp2") float zangle, __reg("a0") Vec3* pos);
extern void Mat34_setRotHPBMove(__reg("a1") Mat34* t, __reg("fp0") float heading, __reg("fp1") float pitch, __reg("fp2") float bank, __reg("a0") Vec3* pos);
extern void Mat34_setMat33MulMat34(__reg("a2") Mat34* m, __reg("a0") Mat33* m0, __reg("a1") Mat34* m1);


extern Mat33 Mat33_sIdentity;
extern Mat34 Mat34_sIdentity;

static inline void Vec3_readBigEndian(Vec3* binary, Vec3* host)
{
	host->x = endianReadFloat32Big(&binary->x);
	host->y = endianReadFloat32Big(&binary->y);
	host->z = endianReadFloat32Big(&binary->z);
}

static inline void Vec3_writeBigEndian(Vec3* binary, Vec3* host)
{
	endianWriteFloat32Big(&binary->x, host->x);
	endianWriteFloat32Big(&binary->y, host->y);
	endianWriteFloat32Big(&binary->z, host->z);
}

static inline void Mat34_readBigEndian(Mat34* binary, Mat34* host)
{
	Vec3_readBigEndian(&binary->Rot.Row[0], &host->Rot.Row[0]);
	Vec3_readBigEndian(&binary->Rot.Row[1], &host->Rot.Row[1]);
	Vec3_readBigEndian(&binary->Rot.Row[2], &host->Rot.Row[2]);
	Vec3_readBigEndian(&binary->Move, &host->Move);
}

static inline void Mat34_writeBigEndian(Mat34* binary, Mat34* host)
{
	Vec3_writeBigEndian(&binary->Rot.Row[0], &host->Rot.Row[0]);
	Vec3_writeBigEndian(&binary->Rot.Row[1], &host->Rot.Row[1]);
	Vec3_writeBigEndian(&binary->Rot.Row[2], &host->Rot.Row[2]);
	Vec3_writeBigEndian(&binary->Move, &host->Move);
}

#endif
