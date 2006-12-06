#ifndef MUTANT_HERMITE_H_
#define MUTANT_HERMITE_H_

#include "util.h"
#include <limits>

namespace mutant 
{
	namespace hermite
	{
		namespace detail
		{
			template<int _I> struct H;

			template<> struct H<0> { static double value( double u ) { return  2.0*u*u*u  - 3.0*u*u		+ 1.0	; } };
			template<> struct H<1> { static double value( double u ) { return -2.0*u*u*u  + 3.0*u*u				; } };
			template<> struct H<2> { static double value( double u ) { return       u*u*u - 2.0*u*u		+ u		; } };
			template<> struct H<3> { static double value( double u ) { return       u*u*u -      u*u			; } };

			template<typename R, typename P>
			R curve( P const& p, float u )
			{
				return 
					p[0] * H<0>::value( u ) +
					p[1] * H<1>::value( u ) +
					p[2] * H<2>::value( u ) +
					p[3] * H<3>::value( u );
			}
		}

		template<typename P, typename R>
		R curve( P const& p, float u )
		{
			return detail::curve<R>( p, u );
		}
	}

	// note: not tested (and basically should NOT work)
	template<typename ValueT>
	struct hermite_evaluator_basis
	{
		enum { inputs = 5 };
		typedef ValueT result_type;

		template<typename KeysT, typename ValuesT>
		void init( KeysT const& keys, size_t k, ValueT const& values, size_t i, size_t component_size )
		{
			typedef typename ValueT::value_type value_t;
			typedef typename KeysT::value_type key_t;

			float const oneThird = 1.0f / 3.0f;
			float const tanMax = 5729577.9485111479f;

			value_t const* v1 = &values[i];
			value_t const* v2 = &values[i + component_size];
			key_t const& k1 = keys[k];
			key_t const& k2 = keys[k+1];

			float	x0 = k1,
					x1 = k1 + v1[3] * oneThird,
					x2 = k2,
					x3 = k2 - v2[1] * oneThird;

			float	y0 = v1[0],
					y1 = v1[0] + v1[4] * oneThird,
					y2 = v2[0],
					y3 = v2[0] - v2[2] * oneThird;

			// todo: check dx not equal 0.0
			float	t1 = (y1 - y0) / (x1 - x0),
					t2 = (y3 - y2) / (x3 - x2);

			mData[0] = x0;
			mData[1] = t1;
			mData[2] = t2;
			mData[3] = x3;
		}

		template<typename KeysT>
		ValueT evaluate( float nonNormalizedTime, KeysT const& keys, size_t i ) const
		{
			return mutant::hermite::curve<ValueT>(
				mData,
				(nonNormalizedTime - keys[i]) / ( keys[i+1] - keys[i] )
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
		ValueT	mData[4];
	};

	////////////////////////////////////////////////
	template<typename ValueT>
	struct hermite_evaluator_cubic
	{
		enum { inputs = 5 };
		typedef ValueT result_type;

		// i - key index, such that t[i] <= t < t[i+1]
		template<class KeysT, class ValuesT>
		void init( KeysT const& keys, size_t k, ValuesT const& values, size_t i, size_t component_size )
		{
			typedef typename ValuesT::value_type value_t;
			typedef typename KeysT::value_type key_t;

			float const oneThird = 1.0f / 3.0f;
			float const tanMax = 5729577.9485111479f;

			value_t const* v1 = &values[i];
			value_t const* v2 = &values[i + component_size];
			key_t const& k1 = keys[k];
			key_t const& k2 = keys[k+1];

			// todo: change to traits<value_t>::is_zero
			// if out-tanyx == 0 && out-tan-y == 0, segment is step
			if( v1[3] == 0.0f && v1[4] == 0.0f )
			{
				// write value to c[3], as it's p0's index in coeficients array
				mData[3] = v1[0];
				mStep = true;
				return;
			}

			mStep = false;

			value_t	x0 = k1,
					x1 = k1 + v1[3] * oneThird,
					x2 = k2 - v2[1] * oneThird,
					x3 = k2;

			value_t	y0 = v1[0],
					y1 = v1[0] + v1[4] * oneThird,
					y2 = v2[0] - v2[2] * oneThird,
					y3 = v2[0];

			value_t	dx = x3-x0;
					//dy = y3-y0;

			// todo: check dx not equal 0.0
			// todo: make assignment with traits
			float t1 = tanMax, t2 = tanMax;

			// todo: make check with traits
			if( (x1 - x0) > std::numeric_limits<float>::epsilon() )
				t1 = (y1 - y0) / (x1 - x0);
			if( (x3 - x2) > std::numeric_limits<float>::epsilon() )
				t2 = (y3 - y2) / (x3 - x2);

			value_t
				p0 = y0, p1 = y3,
				vv0 = t1, //vv1 = t2,
				vl0 = t1*dx, vl1 = t2*dx;

			mData[0]=(	2.0f*p0 - 2.0f*p1	+      vl0	+ vl1	) / dx / dx / dx;
			mData[1]=( -3.0f*p0 + 3.0f*p1   - 2.0f*vl0	- vl1	) / dx / dx;
			mData[2]=(							   vv0			);
			mData[3]=(		 p0									);
		}

		template<typename KeysT>
		ValueT evaluate( float nonNormalizedTime, KeysT const& keys, unsigned i ) const
		{
			if( mStep )
				return mData[3];

			return mutant::util::cubic<ValueT>( mData, nonNormalizedTime - keys[i] );
		}

		template<class ValuesT>
		result_type special( ValuesT const& values, size_t offset, size_t component_size, eSpecialEvaluate spec ) const {
			if( spec == EVAL_POSTEND )
				return values[ values.size() - component_size + offset ];
			else
				return values[ offset ];
		}

	protected:
		bool isStep() const { return mStep; }

	private:
		ValueT	mData[4];
		bool	mStep;
	};
}

#endif // MUTANT_HERMITE_H_
