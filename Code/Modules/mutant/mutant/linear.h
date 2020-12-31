#ifndef MUTANT_LINEAR_H_
#define MUTANT_LINEAR_H_

#include "util.h"
#include "access_policy.h"

namespace mutant
{
	////////////////////////////////////////////////
	template<typename ValueT, class AccessT = straight_access_policy<ValueT> >
	struct linear_evaluator
	{
		typedef AccessT access_t;
		typedef typename AccessT::result_type value_t;

		typedef ValueT result_type;

		enum { inputs = 1 };

		// i - first value index
		// k - key index, such that t[k] <= t < t[k+1]
		template<class KeysT, class ValuesT>
		void init( KeysT const& keys, size_t k, ValuesT const& values, size_t i, size_t component_size )
		{
			// normalized u version
//			mData[0] = util::component( values, i, component_size );
//			mData[1] = util::component( values, i + 1, component_size );

			mData[0] = access_t::at( values, i );
			mData[1] = access_t::at( values, i + component_size );
			mInvDt = 1.0f / ( keys[k+1] - keys[k] );
			// todo: check if step interpolation mode enabled on this segment
		}

		template<typename KeysT>
		ValueT evaluate( float nonNormalizedTime, KeysT const& keys, size_t i ) const
		{
			return mutant::util::lerp<ValueT>(
				mData[0],
				mData[1],
//				(nonNormalizedTime - keys[i]) / ( keys[i+1] - keys[i] )
				(nonNormalizedTime - keys[i]) * mInvDt
			);
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

#endif // MUTANT_LINEAR_H_
