#ifndef MUTANT_COMPOSE_ALGO_H_
#define MUTANT_COMPOSE_ALGO_H_

#include <Base/Math/Quat.h>

namespace mutant
{
	template<typename R, typename A1, typename A2>
	struct comp_2to1
	{
		typedef R value_type;

		 R operator()( A1 const& a1, A2 const& a2 ) const {
			 Vec3 rV;
			 rV.x = a1;
			 rV.y = a2;
			 rV.z = 0;
			 return rV;
//			 return R( a1, a2 );
		 }
	};

	template<typename R, typename A1, typename A2, typename A3>
	struct comp_3to1
	{
		typedef R value_type;

		 R operator()( A1 const& a1, A2 const& a2, A3 const& a3 ) const {
			return R( a1, a2, a3 );
		 }
	};
	template<typename A1, typename A2, typename A3>
	struct comp_3to1_v
	{
		typedef Vec3 value_type;

		 Vec3 operator()( A1 const& a1, A2 const& a2, A3 const& a3 ) const {
			 Vec3 rV;
			 rV.x = a1;
			 rV.y = a2;
			 rV.z = a3;
			 return rV;
//			 return R( a1, a2, a3 );
		 }
	};

	enum eulerSequence
	{
		RPY,
		RYP,
		PYR,
		PRY,
		YRP,
		YPR
	};

	template<int _ESQ>
	struct make_quat_based_on_sequence;

	template<> struct make_quat_based_on_sequence<RPY> {
		template<typename R, typename A1, typename A2, typename A3, typename A4> R operator()( A1 const& a1, A2 const& a2, A3 const& a3, A4 const& a4 ) const { 
			Quat rQ;
			rQ.x = -a1;
			rQ.y = -a2;
			rQ.z = a3;
			rQ.w = -a4;
			return rQ;
		}
	};

	template<> struct make_quat_based_on_sequence<RYP> {
		template<typename R, typename A1, typename A2, typename A3, typename A4> R operator()( A1 const& a1, A2 const& a2, A3 const& a3, A4 const& a4 ) const {
			Quat rQ;
			rQ.x = a1;
			rQ.y = a2;
			rQ.z = a3;
			rQ.w = -a4;
			return rQ;
		}
	};

	// algos
	// http://www.mathworks.com/access/helpdesk/help/toolbox/aeroblks/euleranglestoquaternions.shtml
	// http://jsbsim.sourceforge.net/quaternions.html
	template<typename R, typename A1, typename A2, typename A3>
	struct comp_quaternion_from_euler
	{
		typedef R value_type;

		// YAW(X), PITCH(Y), ROLL(Z)
		 R operator()( A1 const& a1, A2 const& a2, A3 const& a3 ) const {

			 float yaw = a1, pitch = a2, roll = a3;
//             D3DXQuaternionRotationYawPitchRoll( &out, yaw, pitch, roll );

			 Quat rQ;
			 QuatSetFromEulerAngle( &rQ, pitch, yaw, roll );
			 return rQ;
		 }
	};
}

#endif // MUTANT_COMPOSE_ALGO_H_
