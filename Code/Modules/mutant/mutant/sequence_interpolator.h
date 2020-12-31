#ifndef MUTANT_SEQUENCE_INTERPOLATOR_H_
#define MUTANT_SEQUENCE_INTERPOLATOR_H_

#include "cfg.h"

#include "keysearch_algo.h"
#include "time_algo.h"
#include "event_algo.h"

namespace mutant
{
	////////////////////////////////////////////////
	template
	<
		typename DataT,
		typename ResultT = DataT::value_type,
		class TimeAlgoT = time_algo_constant1<float>
	>
	struct sequence_interpolator
	{
		typedef DataT data_t;
		typedef DataT value_vector_t;
		typedef ResultT result_type;

		typedef TimeAlgoT time_algo_t;

		sequence_interpolator( data_t const& data, unsigned lengthInMilis )
		:	mData( data ), mLength( lengthInMilis * float(0.001f) )
		{
		}

		~sequence_interpolator()
		{
		}

		result_type value( float t )
		{
			float goodTime = mTimeAlgo.value( 0, mLength, t );

            unsigned idx = float(mData.size()) * goodTime / mLength;
			if( idx >= mData.size() )
				return mData.back();

			assert( idx < mData.size() );

			return mData[ idx ];
		}

		unsigned cycle_number() const { return 0; }
		unsigned length() const { return mLength * 1000.0f; }

	private:
		time_algo_t		mTimeAlgo;
		data_t const&	mData;
		float			mLength;
	};
}

#endif // MUTANT_SEQUENCE_INTERPOLATOR_H_
