#ifndef MUTANT_CATMULLROM_H_
#define MUTANT_CATMULLROM_H_

#include "types.h"
#include "keysearch_algo.h"

namespace mutant {
	namespace catmullrom {
		template<typename ReturnT, typename DataT>
		ReturnT curve( DataT const& data, float u )
		{
			float uu = u*u;
			float uuu = uu*u;
			float uuu3 = uuu*3.0f;

			return
			(
				data[0] * (       -u	+uu+uu		- uuu  ) +
				data[1] * ( 2.0f		-5.0f*uu	+ uuu3 ) +
				data[2] * (        u	+4.0f*uu	- uuu3 ) +
				data[3] * (				-uu			+ uuu  )
			) * 0.5f;
		}
	}

	// note: not tested (and basically should NOT work)
	template<typename ValueT>
	struct catmullrom_evaluator
	{
		typedef ValueT result_type;

		template<class KeysT, class ValuesT>
		void init( KeysT const& keys, size_t k, ValuesT const& values, size_t i, size_t component_size )
		{
			typedef typename ValuesT::value_type value_t;
			typedef typename KeysT::value_type key_t;

			assert( i >= 0 && i < values.size()	);

			if( i-1 < 0 )
				mData[0] = -( util::component( values, i + 1, component_size ) - util::component( values, i, component_size ) );
			else
				mData[0] = util::component( values, i - 1, component_size );

			mData[1] = util::component( values, i, component_size );
			mData[2] = util::component( values, i + 1, component_size );

			if( i+2 >= values.size() )
				mData[3] = -( util::component( values, i, component_size ) - util::component( values, i + 1, component_size ) );
			else
				mData[3] = util::component( values, i + 2, component_size );
		}

		template<typename KeysT>
		result_type evaluate( float nonNormalizedTime, KeysT const& keys, unsigned i ) const
		{
			return mutant::catmullrom::curve<ValueT>(
				mData,
				( nonNormalizedTime - keys[i] ) / ( keys[i+1] - keys[i] )
			);
		}

		template<class ValuesT>
		result_type special( ValuesT const& values, size_t offset, size_t component_size, eSpecialEvaluate spec ) const {
			if( spec == EVAL_POSTEND )
				return values[ values.size() - component_size + offset ];
			else
				return values[ offset ];
		}

	private:
		ValueT mData[4];
	};
}

#endif
