#ifndef MUTANT_TIME_ALGO_H_
#define MUTANT_TIME_ALGO_H_

#include <math.h>

namespace mutant
{
	// for separate keys
	template<typename KeysT> struct time_algo_straight {
		float value( KeysT const& data, float t ) {
			assert( t >= data.front() && t <= data.back() );
			return t;
		}
		unsigned cycle_count() const { return 0; }
	};

	template<typename TimeT> struct time_algo_straight1 {
		float value( TimeT a, TimeT b, TimeT t ) {
			assert( t >= a && t <= b );
			return t;
		}
		unsigned cycle_count() const { return 0; }
	};

	//
	template<typename KeysT> struct time_algo_constant {
		float value( KeysT const& data, float t ) {
			float mn = data.front();
			float mx = data.back();
			return ( (t<mn) ? mn : ((t>mx) ? mx : t ) );
		}

		unsigned cycle_count() const { return 0; }
	};

	template<typename TimeT> struct time_algo_constant1 {
		float value( TimeT a, TimeT b, TimeT t ) {
			return ( (t<a) ? a : ((t>b) ? b : t ) );
		}
		unsigned cycle_count() const { return 0; }
	};

	//
	template<typename KeysT> struct time_algo_cycle {
		time_algo_cycle() : mCycleCount( 0 ) {}

		float value( KeysT const& data, float t ) {
			float mn = data.front();
			float mx = data.back();
			float const len = mx - mn;
			mCycleCount = static_cast<unsigned>(t / len);
			return mn + fmodf( t, len );
		}

		unsigned cycle_count() const { return mCycleCount; }

	private:
		unsigned	mCycleCount;
	};

	template<typename TimeT> struct time_algo_cycle1 {
		time_algo_cycle1() : mCycleCount( 0 ) {}

		float value( TimeT a, TimeT b, TimeT t ) {
			TimeT const len = a - b;
			mCycleCount = t / len;
			return a + fmodf( t, len );
		}

		unsigned cycle_count() const { return mCycleCount; }

	private:
		unsigned	mCycleCount;
	};

	//
	template<typename KeysT> struct time_algo_oscillate {
		float value( KeysT const& data, float t ) {
			float mn = data.front();
			float mx = data.back();
			float t1 = fmodf( t, 2.0*(mx - mn) );
			float t2 = fmodf( t, mx - mn );
			mCycleCount = t / (2.0*(mx - mn));

			if( t1 > t2 )
				return mx - t2;
			else
				return t2;
		}

		unsigned cycle_count() const { return mCycleCount; }

	private:
		unsigned	mCycleCount;
	};

	template<typename TimeT> struct time_algo_oscillate1 {
		time_algo_oscillate1() : mCycleCount( 0 ) {}

		float value( TimeT mn, TimeT mx, TimeT t ) {
			TimeT t1 = fmodf( t, 2.0f * (mx - mn) );
			TimeT t2 = fmodf( t, mx - mn );
			mCycleCount = static_cast<unsigned>(t / (2.0f * (mx - mn)));

			if( t1 > t2 )
				return mx - t2;
			else
				return t2;
		}

		unsigned cycle_count() const { return mCycleCount; }

	private:
		unsigned	mCycleCount;
	};

	//
	template<typename TimeT = float>
	struct time_controller
	{
		enum time_mode
		{
			time_constant,
			time_loop,
			time_oscillate
		};

		typedef TimeT time_type;

		time_controller()
		:	mCycleCount( 0 ),
			mBegin( 0.0 ),
			mLength( 1.0 ),
			mOriginalLength( 1.0 ),
			mSpeed( 1.0 ),
			mTimeMode( time_constant )
		{
		}

		time_controller( time_type length, time_type speed, time_mode mode = time_constant, time_type begin = 0.0 )
		:	mCycleCount( 0 ),
			mBegin( begin ),
			mLength( length / speed ),
			mOriginalLength( length ),
			mSpeed( speed ),
			mTimeMode( mode )
		{
		}

		time_type processed_time() const { return mTime; }		
		unsigned cycle_count() const { return mCycleCount; }

		time_type begin() const { return mBegin; }
		time_type length() const { return mLength; }
		time_type speed() const { return mSpeed; }
		time_mode mode() const { return mTimeMode; }

		void set_time( time_type val ) { mTime = value( val ); }
		void set_begin( time_type val ) { mBegin = val; }
		void set_length( time_type val ) { mOriginalLength = val; mLength = val / mSpeed; }
		void set_speed( time_type speed ) { mSpeed = speed; mLength = mOriginalLength / speed; }
		void set_mode( time_mode mode ) { mTimeMode = mode; }

	protected:
		float value( float t )
		{
			if( mLength <= 0.0f )
				return 0.0f;
//			assert( mLength > 0.0 );

			float ts = (t - mBegin) * mSpeed;

			switch( mTimeMode )
			{
			default:
			case time_constant:
				{
					mCycleCount = ( t < mBegin ) ? 0 : (unsigned)(ts / mOriginalLength);
//					mCycleCount = 0;
					// TODO: curves' tracks may start at non-zero time
					if( t < mBegin )
						return 0.0f;
					else
						if( ts > mLength )
							return mLength + mBegin;
						else
							return ts + mBegin;
					break;
				}
			case time_loop:
				{
					time_type tt = fmodf( ts, mOriginalLength );

					mCycleCount = ( t < mBegin ) ? 0 : (unsigned)(ts / mOriginalLength);
					return tt + mBegin;
					break;
				}
			case time_oscillate:
				{
					time_algo_oscillate1<time_type> tos;
					time_type tt = tos.value( mBegin, mBegin + mOriginalLength, ts );
					mCycleCount = tos.cycle_count();
					return tt;
					break;
				}
			};
			return 0.0f;
		}

	private:
		unsigned	mCycleCount;
		time_type	mTime;
		time_type	mBegin;
		time_type	mLength;
		time_type	mOriginalLength;
		time_type	mSpeed;
		time_mode	mTimeMode;
	};
}

#endif // MUTANT_TIME_ALGO_H_
