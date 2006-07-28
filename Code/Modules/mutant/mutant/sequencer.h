#ifndef MUTANT_SEQUENCER_H_
#define MUTANT_SEQUENCER_H_

#include "keysearch_algo.h"
#include "time_algo.h"
#include "event_algo.h"

namespace mutant
{
	////////////////////////////////////////////////
	// x fire algo
	//   o fire all that passed
	//   o fire last passed
	template
	<
		typename DataT,
		class ListenerT, // should have operator()( FireEventAlgoT::access_policy::value_type const& data )
		class FireEventAlgoT,
		class KeySearchAlgoT = key_search_binary<typename DataT::key_vector_t>,
		template<typename> class TimeAlgoT = time_algo_constant
	>
	struct sequencer
	{
		typedef DataT data_t;
		typedef typename DataT::key_vector_t	key_vector_t;
		typedef typename DataT::value_vector_t	value_vector_t;
		typedef typename DataT::value_t			value_t;
		typedef ListenerT						listener_t;

		// concretes
		typedef TimeAlgoT<key_vector_t>			time_algo_t;
		typedef KeySearchAlgoT					search_algo_t;
		typedef std::vector<ListenerT>			listeners_t;
		typedef FireEventAlgoT					fire_event_algo_t;

		sequencer( data_t const& data )
		:	mData( data )
		{
			mEventAlgo.rewind();
		}

		~sequencer()
		{
		}

		void add_listener( listener_t const& lst ) {
			mListeners.push_back( lst );
		}

		// note: it may not function as expected if operator == is not well-defined
		void remove_listener( listener_t const& lst ) {
			mListeners.erase( std::find( mListeners.begin(), mListeners.end(), lst ) );
		}

		void clear_listeners() {
			mListeners.clear();
		}

		void process( float t )
		{
			float good_t = mTimeAlgo.value( mData.keys(), t );

			if( good_t < mData.keys().front() )
				return;

			if( good_t >= mData.keys().back() )
				mEventAlgo.process( mData, mListeners, unsigned(mData.values().size() - 1), good_t );
			else {
				if( mSearchAlgo.needNewSearch( good_t, mData.keys() ) )
					mSearchAlgo.search( mData.keys(), good_t );

				mEventAlgo.process( mData, mListeners, unsigned(mSearchAlgo.searchResult()), good_t );
			}
		}

		void rewind()
		{
			mEventAlgo.rewind();
		}

//		time_algo_t const& time_algo() const { return mTimeAlgo; }

	private:
		search_algo_t		mSearchAlgo;
		fire_event_algo_t	mEventAlgo;
		listeners_t			mListeners;
		data_t const&		mData;
		time_algo_t			mTimeAlgo;
	};
}

#endif // MUTANT_SEQUENCER_H_
