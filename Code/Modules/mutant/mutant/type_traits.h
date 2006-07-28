#ifndef MUTANT_TYPE_TRAITS_H_
#define MUTANT_TYPE_TRAITS_H_

#include "cfg.h"

#include <d3dx9.h>
#include "util.h"

namespace mutant
{
/*	template<typename ValueT>
	struct value_traits_base
	{
		// todo:
		static bool equal( ValueT const& a, ValueT const& b ) { return a == b; }
		static ValueT zero() { return 0.0; }
		static void identify( ValueT& a ) { a = 1.0; }
		static ValueT identity() { return 1.0; }

		static ValueT blend( ValueT const& a1, float w1, ValueT const& a2, float w2 ) {
			return a1*w1 + a2*w2;
		}
	};

	template<typename _T>
		struct value_traits : public value_traits_base<_T>
	{
	};

	struct value_traits<float> : public value_traits_base<float>
	{
		static bool equal( ValueT const& a, ValueT const& b ) { return a == b; }
		static ValueT zero() { return 0.0f; }
		static void identify( ValueT& a ) { a = 1.0f; }
		static ValueT identity() { return 1.0f; }

		static ValueT blend( ValueT const& a1, float w1, ValueT const& a2, float w2 ) {
			return a1*w1 + a2*w2;
		}
	};

	struct value_traits<D3DXVECTOR3> : public value_traits_base<D3DXVECTOR3>
	{
		static bool equal( ValueT const& a, ValueT const& b ) { return a == b; }
		static ValueT zero() { return D3DXVECTOR3(0.0f,0.0f,0.0f); }
		static void identify( ValueT& a ) { a = D3DXVECTOR3(1.0f,1.0f,1.0f); }
		static ValueT identity() { return D3DXVECTOR3(1.0f,1.0f,1.0f); }

		static ValueT blend( ValueT const& a1, float w1, ValueT const& a2, float w2 ) {
			return a1*w1 + a2*w2;
		}
	};

	struct value_traits<D3DXQUATERNION> : public value_traits_base<D3DXQUATERNION>
	{
		static bool equal( ValueT const& a, ValueT const& b ) { return a == b; }
		static ValueT zero() { return D3DXQUATERNION(0.0f,0.0f,0.0f,0.0f); }
		static ValueT identity() { return D3DXQUATERNION(0.0f,0.0f,0.0f,1.0f); }

		static ValueT blend( ValueT const& a1, float w1, ValueT const& a2, float w2 ) {
			// assert( w1 + w2 == 1 );
			D3DXQUATERNION qo;
			return *D3DXQuaternionSlerp( &qo, &a1, &a2, w1 );
		}
	};
*/
}

#endif // MUTANT_TYPE_TRAITS_H_
