#ifndef NEWAGE_TRANSFORM_H_
#define NEWAGE_TRANSFORM_H_

#include "cfg.h"

extern "C" {
	#include <Base/Math/Lin.h>
	#include <Base/Math/Quat.h>
}

//#define FORCE_D3DXMATH
#if defined(FORCE_D3DXMATH)
	typedef D3DXVECTOR3		xVec3;
	typedef D3DXQUATERNION	xQuat;

	struct xMat34 : public Mat34
	{
		xMat34();
		xMat34( float x0, float y0, float z0,
			float x1, float y1, float z1,
			float x2, float y2, float z2,
			float x3, float y3, float z3 );
		xMat34 operator*( xMat34 const& rhs ) const;
	};
#else
	struct xVec3 : public Vec3
	{
		xVec3( float x_ = 0.0f, float y_ = 0.0f, float z_ = 0.0f ) { x = x_; y = y_; z = z_; }
		xVec3( Vec3 const& v ) { x = v.x; y = v.y; z = v.z; }

		xVec3 operator+( xVec3 const& rhs ) const;
		xVec3 operator-( xVec3 const& rhs ) const;
		xVec3 operator*( float v ) const;
	};

	struct xQuat : public Quat
	{
		xQuat( float x_ = 0.0f, float y_ = 0.0f, float z_ = 0.0f, float w_ = 0.0f ) { x = x_; y = y_; z = z_; w = w_; }
		xQuat( Quat const& q ) { x = q.x; y = q.y; z = q.z; w = q.w; }
	};

	struct xMat34 : public Mat34
	{
		xMat34();
		xMat34( float x0, float y0, float z0,
			float x1, float y1, float z1,
			float x2, float y2, float z2,
			float x3, float y3, float z3 );
		xMat34 operator*( xMat34 const& rhs ) const;

		xMat34( xMat34 const& c );
		xMat34& operator=( xMat34 const& rhs );
	};
#endif

class CTransform
{
public:
	typedef xVec3	t_vector;
	typedef xQuat	t_quaternion;
	typedef xMat34	t_matrix;

	CTransform();
	CTransform( t_vector const& transl, t_quaternion const& rot, t_vector const& scale );
	CTransform( CTransform const& r );
	CTransform( t_matrix const& m );

	CTransform const& operator=( CTransform const& rh );
	CTransform const& operator=( t_matrix const& m );

	static CTransform const& identity() { return mIdentityTransform; }
	static t_matrix const& identityMatrix() { return mIdentityMatrix; }
	static t_vector const& nullTranslation() { return mIdentityTranslation; }
	static t_quaternion const& identityRotation() { return mIdentityRotation; }
	static t_vector const& identityScale() { return mIdentityScale; }

	static CTransform scaling( float s );
	static CTransform scaling( float x, float y, float z );
	static CTransform scaling( t_vector const& s );
	static CTransform translating( t_vector const& t );
	static CTransform translating( float x, float y, float z );
	static CTransform rotating( t_quaternion const& q );

	static t_quaternion rotationAxisAngle( t_vector const& vec, float angle );

	t_quaternion rotationConjugate() const;

	void identify();
	void translateBy( t_vector const& transl );
	void rotateBy( t_quaternion const& q );
	void scaleBy( t_vector const& scl );
	void scaleBy( float scl );

	t_vector orientVector( t_vector const& vec ) const;

	void setTranslation( t_vector const& v ) { mTranslation = v; }
	void setRotation( t_quaternion const& q ) { mRotation = q; }
	void setScale( t_vector const& s ) { mScale = s; }
	void setScale( float s ) { mScale.x = mScale.y = mScale.z = s; }

	t_vector& translation() { return mTranslation; }
	t_vector const& translation() const { return mTranslation; }
	t_quaternion& rotation() { return mRotation; }
	t_quaternion const& rotation() const { return mRotation; }
	t_vector& scale() { return mScale; }
	t_vector const& scale() const { return mScale; }

	t_matrix matrix() const;

//	CTransform operator*( CTransform const& rh ) const;
//	t_matrix operator*( t_matrix const& mat ) const;
	t_vector transformCoord( t_vector const& vec ) const;
	t_vector transformNormal( t_vector const& vec ) const;
	
	operator t_matrix() const { return matrix(); }

	friend t_matrix operator*( CTransform::t_matrix const& lh, CTransform const& rh );

private:
	t_vector		mTranslation;
	t_quaternion	mRotation;
	t_vector		mScale;

	static CTransform	mIdentityTransform;
	static t_vector		mIdentityScale;
	static t_quaternion	mIdentityRotation;
	static t_vector		mIdentityTranslation;
	static t_matrix		mIdentityMatrix;
};

CTransform::t_matrix operator*(
	CTransform::t_matrix const& lh, CTransform const& rh );

#endif // NEWAGE_TRANSFORM_H_
