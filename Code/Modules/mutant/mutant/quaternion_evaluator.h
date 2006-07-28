#ifndef MUTANT_QUATERNION_EVALUATOR_H_
#define MUTANT_QUATERNION_EVALUATOR_H_

#include "access_policy.h"
#include <Base/Math/Quat.h>

namespace mutant
{
	////////////////////////////////////////////////
	template<typename ValueT, class AccessT = compose_access_policy<ValueT,4> >
	struct linear_quaternion_evaluator
	{
		typedef AccessT access_t;

		typedef ValueT result_type;

		// i - key index, such that t[i] <= t < t[i+1]
		template<class KeysT, class ValuesT>
		void init( KeysT const& keys, size_t k, ValuesT const& values, size_t i, size_t component_size )
		{
			mData[0] = access_t::at( values, i );
			mData[1] = access_t::at( values, i + component_size );
			mInvDt = 1.0f / ( keys[k+1] - keys[k] );
		}

		template<typename KeysT>
		ValueT evaluate( float nonNormalizedTime, KeysT const& keys, size_t i ) const
		{
			ValueT quat;
//			return *D3DXQuaternionSlerp( &quat, &mData[0], &mData[1],
//				(nonNormalizedTime - keys[i]) * mInvDt );

			QuatSLinearCombine( &quat, (ValueT*)&mData[0], (ValueT*)&mData[1],
				(nonNormalizedTime - keys[i]) * mInvDt );
			return quat;
		}

		template<class ValuesT>
		result_type special( ValuesT const& values, size_t offset, size_t component_size, eSpecialEvaluate spec ) const {
			if( spec == EVAL_POSTEND )
				return access_t::at( values, values.size() - component_size + offset );
			else
				return access_t::at( values, offset );
		}

	private:
		float	mInvDt;
		ValueT	mData[2];
	};
}

#endif // MUTANT_QUATERNION_EVALUATOR_H_

