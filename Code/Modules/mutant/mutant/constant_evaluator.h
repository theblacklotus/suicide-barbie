#ifndef MUTANT_CONSTANT_EVALUATOR_H_
#define MUTANT_CONSTANT_EVALUATOR_H_

#include "types.h"
#include "keysearch_algo.h"

#include <vector>
#include <iterator>

namespace mutant {

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
			mData[0] = access_t::at( values, i );
		}

		template<typename KeysT>
		ValueT evaluate( float nonNormalizedTime, KeysT const& keys, size_t i ) const
		{
			return mData[0];
		}

		template<class ValuesT>
		result_type special( ValuesT const& values, size_t offset, size_t component_size, eSpecialEvaluate spec ) const {
			if( spec == EVAL_POSTEND )
				return access_t::at( values, values.size() - component_size + offset );
			else
				return access_t::at( values, offset );
		}

	private:
		ValueT	mData[1];
	};
}

#endif // MUTANT_CONSTANT_EVALUATOR_H_
