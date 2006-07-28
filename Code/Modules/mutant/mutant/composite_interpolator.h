#ifndef MUTANT_COMPOSITE_INTERPOLATOR_H_
#define MUTANT_COMPOSITE_INTERPOLATOR_H_

#include "types.h"

// todo: specialize dependant includes. may remove
#include "compose_algo.h"

namespace mutant
{

	template<typename R, class C, class T1, class T2, class T3 = nil_t, class T4 = nil_t, class T5 = nil_t, class T6 = nil_t, class T7 = nil_t, class T8 = nil_t, class T9 = nil_t, class T10 = nil_t>
	struct composite_interpolator
	{
		typedef R return_type;
		typedef C compose_algo_t;

		enum { in_count =
			(detail::is_nil<T3>::value ? 2 :
				(detail::is_nil<T4>::value ? 3 :
					(detail::is_nil<T5>::value ? 4 :
						(detail::is_nil<T6>::value ? 5 :
							(detail::is_nil<T7>::value ? 6 :
								(detail::is_nil<T8>::value ? 7 :
									(detail::is_nil<T9>::value ? 8 :
										(detail::is_nil<T10>::value ? 9 : 10 )
									)
								)
							)
						)
					)
				)
			)
		};

		composite_interpolator( T1 const& i1 ) : mInterpolator1( i1 ) {}
		composite_interpolator( T1 const& i1, T2 const& i2 ) : mInterpolator1( i1 ), mInterpolator2( i2 ) {}
		composite_interpolator( T1 const& i1, T2 const& i2, T3 const& i3 ) : mInterpolator1( i1 ), mInterpolator2( i2 ), mInterpolator3( i3 ) {}

		return_type value( float t ) {
			return compose( int2type<in_count>(), t );
		}

	protected:
		return_type compose( int2type<2>, float t ) { return mComposeAlgo( mInterpolator1.value( t ), mInterpolator2.value( t ) ); }
		return_type compose( int2type<3>, float t ) { return mComposeAlgo( mInterpolator1.value( t ), mInterpolator2.value( t ), mInterpolator3.value( t ) ); }

	private:
		T1	mInterpolator1;
		T2	mInterpolator2;
		T3	mInterpolator3;
/*		T4	mInterpolator4;
		T5	mInterpolator5;
		T6	mInterpolator6;
		T7	mInterpolator7;
		T8	mInterpolator8;
		T9	mInterpolator9;*/

		compose_algo_t	mComposeAlgo;
	};

	template<typename R, class C, class T1, class T2, class T3 = nil_t, class T4 = nil_t>
	struct composite_interpolator_small
	{
		typedef R return_type;
		typedef C compose_algo_t;

		enum { in_count =
			(detail::is_nil<T3>::value ? 2 :
				(detail::is_nil<T4>::value ? 3 : 4 )
			)
		};

		composite_interpolator_small( T1 const& i1 ) : mInterpolator1( i1 ) {}
		composite_interpolator_small( T1 const& i1, T2 const& i2 ) : mInterpolator1( i1 ), mInterpolator2( i2 ) {}
		composite_interpolator_small( T1 const& i1, T2 const& i2, T3 const& i3 ) : mInterpolator1( i1 ), mInterpolator2( i2 ), mInterpolator3( i3 ) {}
		composite_interpolator_small( T1 const& i1, T2 const& i2, T3 const& i3, T4 const& i4 ) : mInterpolator1( i1 ), mInterpolator2( i2 ), mInterpolator3( i3 ), mInterpolator4( i4 ) {}

		return_type value( float t ) {
			return compose( int2type<in_count>(), t );
		}

	protected:
		return_type compose( int2type<2>, float t ) { return mComposeAlgo( mInterpolator1.value( t ), mInterpolator2.value( t ) ); }
		return_type compose( int2type<3>, float t ) { return mComposeAlgo( mInterpolator1.value( t ), mInterpolator2.value( t ), mInterpolator3.value( t ) ); }
		return_type compose( int2type<4>, float t ) { return mComposeAlgo( mInterpolator1.value( t ), mInterpolator2.value( t ), mInterpolator3.value( t ), mInterpolator4.value( t ) ); }

	private:
		T1	mInterpolator1;
		T2	mInterpolator2;
		T3	mInterpolator3;
		T4	mInterpolator4;

		compose_algo_t	mComposeAlgo;
	};
}

#endif
