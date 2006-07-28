#ifndef MUTANT_BEZIER_H_
#define MUTANT_BEZIER_H_

#include "types.h"
#include "keysearch_algo.h"

#include <vector>
#include <iterator>

namespace mutant
{
	namespace bezier {

		//
		// B[i,n](t) = (n!/(i!(n-i)!)) t**i (1-t)**(n-i)
		//

		template<int b,int i> struct B; 

		template<> struct B<3,0> { static float value( float u ) { return (1.0f - u) * (1.0f - u) * (1.0f - u); }	};
		template<> struct B<3,1> { static float value( float u ) { return 3.0f * u * (1.0f - u) * (1.0f - u); } };
		template<> struct B<3,2> { static float value( float u ) { return 3.0f * u * u * (1.0f - u); } };
		template<> struct B<3,3> { static float value( float u ) { return u * u * u; } };
		
		// internal
		namespace detail
		{
			template<typename ReturnT, typename DataT>
			ReturnT curve( int2type<3>, DataT const& data, float u )
			{
				typedef ::mutant::bezier::detail::B B;

				return 
					data[0] * B<3,0>::value( u ) +
					data[1] * B<3,1>::value( u ) +
					data[2] * B<3,2>::value( u ) +
					data[3] * B<3,3>::value( u );	
			}
		}
		
		template<int DegreeT, typename ReturnT, typename DataT>
		ReturnT curve( DataT const& data, float u )
		{
			return detail::curve<ReturnT>( int2type<DegreeT>(), data, u );
		}
	}

	template<typename ValueT>
	struct bezier_evaluator
	{
/*		template<class KeysItT, class ValuesItT>
		void init( KeysItT keys, ValuesItT values )
		{
			mData = *values;
		}*/

		typedef ValueT result_type;

		template<class KeysT, class ValuesT>
		void init( KeysT const& keys, ValuesT const& values, size_t i, size_t component_size )
		{
			typedef typename ValuesT::value_type value_t;
			typedef typename KeysT::value_type key_t;

			mData = &util::component( values, i, component_size );
		}

		template<typename KeysT>
		ValueT evaluate( float nonNormalizedTime, KeysT const& keys, unsigned i ) const
		{
			return mutant::bezier::curve<3,ValueT>(
				*mData,
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
		ValueT const* mData;
	};
}

#endif
