#ifndef MUTANT_EVENT_ALGO_H_
#define MUTANT_EVENT_ALGO_H_

#include "access_policy.h"

namespace mutant
{
	////////////////////////////////////////////////
	template<typename ValueT, class DataAccessT = straight_access_policy<ValueT> >
	struct fire_segment_event
	{
		typedef DataAccessT access_t;

		fire_segment_event()
			:	mLastFired( -1 ) {}

		template<class DataT, class ListenersT>
		void process( DataT const& data, ListenersT const& lstrs, unsigned eventId, float t )
		{
			assert( eventId < data.size() );

			if( mLastFired < (int)eventId )
			{
				mLastFired = eventId;

				typedef typename ListenersT::const_iterator t_it;
				for( t_it it = lstrs.begin(); it != lstrs.end(); ++it )
					(*it)( t, access_t::at( data.values(), data.componentSize() * eventId ) );
			}
		}
	            
		void rewind()
		{
			mLastFired = -1;
		}

	private:
		int		mLastFired;
	};

	////////////////////////////////////////////////
	template<typename ValueT, class DataAccessT = straight_access_policy<ValueT> >
	struct fire_consistent_event
	{
		typedef DataAccessT access_t;
		typedef std::vector<bool> flags_t;

		fire_consistent_event()
		{
			rewind();
		}

		template<class DataT, class ListenersT>
		void process( DataT const& data, ListenersT const& lstrs, unsigned eventId, float t )
		{
			assert( eventId < data.size() );

			if( mNotFiredBegin <= eventId )
			{
				if( mFlags.size() != data.size() )
				{
					mFlags.resize( data.size() );
					rewind();                    
				}

				for( unsigned i=mNotFiredBegin; i<=eventId; ++i )
				{
					if( !mFlags[i] )
					{
						mFlags[i] = true;

						typedef typename ListenersT::const_iterator t_it;
						for( t_it it = lstrs.begin(); it != lstrs.end(); ++it )
							(*it)( t, access_t::at( data.values(), data.componentSize() * i ) );
					}
				}

				mNotFiredBegin = eventId;
			}
		}

		struct gen_false {
			bool operator()() const { return false; }
		};

		void rewind()
		{
			mNotFiredBegin = 0;
			std::generate( mFlags.begin(), mFlags.end(), gen_false() );
		}

	private:
		unsigned	mNotFiredBegin;
		flags_t		mFlags;
	};
}

#endif // MUTANT_EVENT_ALGO_H_
