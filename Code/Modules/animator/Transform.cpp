#include "Transform.h"

#include <math.h>
//#include <iostream>

////////////////////////////////////////////////
xVec3 xVec3::operator+( xVec3 const& rhs ) const
{
	xVec3 rV;
	Vec3_add( &rV, const_cast<xVec3*>(this), const_cast<xVec3*>(&rhs) );
	return rV;
}
xVec3 xVec3::operator-( xVec3 const& rhs ) const
{
	xVec3 rV;
	Vec3_sub( &rV, const_cast<xVec3*>(this), const_cast<xVec3*>(&rhs) );
	return rV;
}

xVec3 xVec3::operator*( float v ) const
{
	xVec3 rV;
	Vec3_scale( &rV, const_cast<xVec3*>(this), v );
	return rV;
}

xMat34::xMat34()
{
	Mat34_setIdentity( this );
}

xMat34::xMat34(
	float x0, float y0, float z0,
	float x1, float y1, float z1,
	float x2, float y2, float z2,
	float x3, float y3, float z3 )
{
	Rot.Row[0].x = x0; Rot.Row[0].y = y0; Rot.Row[0].z = z0;
	Rot.Row[1].x = x1; Rot.Row[1].y = y1; Rot.Row[1].z = z1;
	Rot.Row[2].x = x2; Rot.Row[2].y = y2; Rot.Row[2].z = z2;
	Move.x = x3; Move.y = y3; Move.z = z3;
}

xMat34::xMat34( xMat34 const& c)
{
	Move = c.Move;
}
xMat34& xMat34::operator=( xMat34 const& rhs )
{
	return *this;
}

xMat34 xMat34::operator*( xMat34 const& rhs ) const
{
	xMat34 rM;
	Mat34_mul( &rM, const_cast<xMat34*>(this), const_cast<xMat34*>(&rhs) );
	return rM;

}


namespace {
	float& mat34_ij( CTransform::t_matrix const& m, int i, int j )
	{
		return ((float*)&m)[i * 3 + j];
	}
}

CTransform CTransform::mIdentityTransform =
CTransform
(
	CTransform::t_vector( 0.0f, 0.0f, 0.0f ),
	CTransform::t_quaternion( 0.0f, 0.0f, 0.0f, 1.0f ),
	CTransform::t_vector( 1.0f, 1.0f, 1.0f )
);

CTransform::t_vector CTransform::mIdentityScale( 1.0f, 1.0f, 1.0f );
CTransform::t_quaternion CTransform::mIdentityRotation( 0.0f, 0.0f, 0.0f, 1.0f );
CTransform::t_vector CTransform::mIdentityTranslation( 0.0f, 0.0f, 0.0f );

CTransform::t_matrix CTransform::mIdentityMatrix(
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 0.0f );

////////////////////////////////////////////////
CTransform::CTransform()
{
}

CTransform::CTransform( t_vector const& transl, t_quaternion const& rot, t_vector const& scale )
:	mTranslation( transl ),
	mRotation( rot ),
	mScale( scale )
{
}

CTransform::CTransform( CTransform const& r )
:	mTranslation( r.mTranslation ),
	mRotation( r.mRotation ),
	mScale( r.mScale )
{
}

CTransform::CTransform( t_matrix const& m )
{
	*this = m;
}

CTransform const& CTransform::operator=( CTransform const& rh )
{
	mTranslation = rh.mTranslation;
	mRotation = rh.mRotation;
	mScale = rh.mScale;
	return *this;
}

/*
http://skal.planet-d.net/demo/matrixfaq.htm#Q55
http://www.j3d.org/matrix_faq/

Q55. How do I convert a rotation matrix to a quaternion?
*/

CTransform const& CTransform::operator=( t_matrix const& m )
{
	mTranslation = m.Move;

	Vec3 v1 = m.Rot.Row[0];
	Vec3 v2 = m.Rot.Row[1];
	Vec3 v3 = m.Rot.Row[2];

	mScale.x = Vec3_length( &v1 );
	mScale.y = Vec3_length( &v2 );
	mScale.z = Vec3_length( &v3 );

	Vec3_scale( &v1, &v1, 1.0f / mScale.x );
	Vec3_scale( &v2, &v2, 1.0f / mScale.y );
	Vec3_scale( &v3, &v3, 1.0f / mScale.z );

	// build rotation matrix without scaling information
	t_matrix rotM(
		v1.x, v1.y, v1.z,
		v2.x, v2.y, v2.z,
		v3.x, v3.y, v3.z,
		0.0f, 0.0f, 0.0f );

	typedef float float_t;

	float_t T = 1.0 + mat34_ij( rotM, 0, 0 ) + mat34_ij( rotM, 1, 1 ) + mat34_ij( rotM, 2, 2 );
	float_t S, X, Y, Z, W;

	if( T >= 0.000001 )
	{
		S = sqrt(T) * 2;
		X = ( mat34_ij( rotM, 1, 2 /*9*/) - mat34_ij( rotM, 2, 1 /*6*/) ) / S;
		Y = ( mat34_ij( rotM, 2, 0 /*2*/) - mat34_ij( rotM, 0, 2 /*8*/) ) / S;
		Z = ( mat34_ij( rotM, 0, 1 /*4*/) - mat34_ij( rotM, 1, 0 /*1*/) ) / S;
		W = 0.25 * S;
	} else
	{
		if( mat34_ij( rotM, 0, 0 ) > mat34_ij( rotM, 1, 1 ) && 
			mat34_ij( rotM, 0, 0 ) > mat34_ij( rotM, 2, 2 ) )  {	// Column 0: 
			S  = sqrt( 1.0 + mat34_ij( rotM, 0, 0 ) - mat34_ij( rotM, 1, 1 ) - mat34_ij( rotM, 2, 2 ) ) * 2;
			X = 0.25 * S;
			Y = (mat34_ij( rotM, 0, 1 ) + mat34_ij( rotM, 1, 0 ) ) / S;
			Z = (mat34_ij( rotM, 2, 0 ) + mat34_ij( rotM, 0, 2 ) ) / S;
			W = (mat34_ij( rotM, 1, 2 ) - mat34_ij( rotM, 2, 1 ) ) / S;
		} else if ( mat34_ij( rotM, 1, 1 ) > mat34_ij( rotM, 2, 2 ) ) {			// Column 1: 
			S  = sqrt( 1.0 + mat34_ij( rotM, 1, 1 ) - mat34_ij( rotM, 0, 0 ) - mat34_ij( rotM, 2, 2 ) ) * 2;
			X = (mat34_ij( rotM, 0, 1 ) + mat34_ij( rotM, 1, 0 ) ) / S;
			Y = 0.25 * S;
			Z = (mat34_ij( rotM, 1, 2 ) + mat34_ij( rotM, 2, 1 ) ) / S;
			W = (mat34_ij( rotM, 2, 0 ) - mat34_ij( rotM, 0, 2 ) ) / S;
		} else {						// Column 2:
			S  = sqrt( 1.0 + mat34_ij( rotM, 2, 2 ) - mat34_ij( rotM, 0, 0 ) - mat34_ij( rotM, 1, 1 ) ) * 2;
			X = (mat34_ij( rotM, 2, 0 ) + mat34_ij( rotM, 0, 2 ) ) / S;
			Y = (mat34_ij( rotM, 1, 2 ) + mat34_ij( rotM, 2, 1 ) ) / S;
			Z = 0.25 * S;
			W = (mat34_ij( rotM, 0, 1 ) - mat34_ij( rotM, 1, 0 ) ) / S;
		}
	}

	mRotation.x = X;
	mRotation.y = Y;
	mRotation.z = Z;
	mRotation.w = W;

	return *this;
}

/*CTransform CTransform::operator*( CTransform const& rh ) const
{
	t_matrix m0 = matrix();
	t_matrix m1 = rh.matrix();

	t_matrix rm;
	Mat34_mul( &rm, &m0, &m1 );
	return CTransform( rm );
}

CTransform::t_matrix CTransform::operator*( t_matrix const& rh ) const
{
	t_matrix rm, m0 = matrix();
	Mat34_mul( &rm, &m0, (t_matrix*)&rh );
	return rm;
}
*/
CTransform::t_matrix operator*( CTransform::t_matrix const& lh, CTransform const& rh )
{
	CTransform::t_matrix rm, m1 = rh.matrix();
	Mat34_mul( &rm, (CTransform::t_matrix*)&lh, &m1 );
	return rm;
}

CTransform::t_matrix CTransform::matrix() const
{
	typedef float float_t;

	t_matrix m;
	float_t sx = scale().x, sy = scale().y, sz = scale().z;
	float_t tx = translation().x, ty = translation().y, tz = translation().z;

	Mat33_setRotQuat( &m.Rot, (t_quaternion*)&mRotation );

	mat34_ij( m, 0, 0 ) *= sx;	mat34_ij( m, 0, 1 ) *= sx;	mat34_ij( m, 0, 2 ) *= sx;
	mat34_ij( m, 1, 0 ) *= sy;	mat34_ij( m, 1, 1 ) *= sy;	mat34_ij( m, 1, 2 ) *= sy;
	mat34_ij( m, 2, 0 ) *= sz;	mat34_ij( m, 2, 1 ) *= sz;	mat34_ij( m, 2, 2 ) *= sz;

	mat34_ij( m, 3, 0 ) = tx;
	mat34_ij( m, 3, 1 ) = ty;
	mat34_ij( m, 3, 2 ) = tz;

	return m;
}

void CTransform::identify()
{
	mTranslation = t_vector( 0.0f, 0.0f, 0.0f );
	mRotation = t_quaternion( 0.0f, 0.0f, 0.0f, 1.0f );
	mScale = t_vector( 1.0f, 1.0f, 1.0f );
}

CTransform::t_quaternion CTransform::rotationConjugate() const
{
	t_quaternion q;
	QuatConjugate( &q, (t_quaternion*)&mRotation );
	return q;
}

CTransform::t_vector CTransform::orientVector( t_vector const& vec ) const
{
	t_quaternion tmpQ, rQ = rotationConjugate(), vQ( vec.x, vec.y, vec.z, 0.0f );
	QuatMul( &tmpQ, &rQ, &vQ );
	QuatMul( &rQ, &tmpQ, (t_quaternion*)&rotation() );
	return t_vector( rQ.x, rQ.y, rQ.z );
}

CTransform::t_vector CTransform::transformCoord( t_vector const& vec ) const
{
	t_vector rV;
	t_matrix m = matrix();

	Vec3_setMat34MulVec3( &rV, &m, (t_vector*)&vec );
	return rV;
}

CTransform::t_vector CTransform::transformNormal( t_vector const& vec ) const
{
	t_vector rV;
	t_matrix m = matrix();

	Vec3_setMat34RotVec3( &rV, &m, (t_vector*)&vec );
	return rV;
}

////////////////////////////////////////////////
CTransform CTransform::scaling( float s ) {
	return CTransform( nullTranslation(), identityRotation(), t_vector( s, s, s ) );
}

CTransform CTransform::scaling( float x, float y, float z )
{
	return CTransform( nullTranslation(), identityRotation(), t_vector( x, y, z ) );
}

CTransform CTransform::scaling( t_vector const& s )
{
	return CTransform( nullTranslation(), identityRotation(), s );
}

CTransform CTransform::translating( t_vector const& t )
{
	return CTransform( t, identityRotation(), identityScale() );
}

CTransform CTransform::translating( float x, float y, float z )
{
	return CTransform( t_vector( x, y, z ), identityRotation(), identityScale() );
}

CTransform CTransform::rotating( t_quaternion const& q )
{
	return CTransform( nullTranslation(), q, identityScale() );
}

void CTransform::translateBy( t_vector const& transl )
{
	Vec3_add( &mTranslation, &mTranslation, (t_vector*)&transl );
}

void CTransform::rotateBy( t_quaternion const& q )
{
	t_quaternion rQ;
	QuatMul( &rQ, &mRotation, (t_quaternion*)&q );
	mRotation = rQ;
}

void CTransform::scaleBy( t_vector const& scl )
{
	Vec3_scaleXYZ( &mScale, &mScale, scl.x, scl.y, scl.z );
}

void CTransform::scaleBy( float scl )
{
	Vec3_scale( &mScale, &mScale, scl );
}

////////////////////////////////////////////////
CTransform::t_quaternion CTransform::rotationAxisAngle( CTransform::t_vector const& vec, float angle )
{
	t_quaternion q;
	QuatSetFromAxisAngle( &q, (t_vector*)&vec, angle );
	return q;
}
/*
std::ostream& operator<<( std::ostream& os, CTransform const& rh )
{
	os << rh.translation() << rh.rotation() << rh.scale();
	return os;
}

std::ostream& operator<<( std::ostream& os, CTransform::t_vector const& rh )
{
	os << "(" << rh.x << " " << rh.y << " " << rh.z << ")";
	return os;
}

std::ostream& operator<<( std::ostream& os, CTransform::t_quaternion const& rh )
{
	os << "(" << rh.x << " " << rh.y << " " << rh.z << " " << rh.w << ")";
	return os;
}*/
