#ifndef MUTANT_INTERPOLATOR_H_
#define MUTANT_INTERPOLATOR_H_

#include "cfg.h"

#include "keysearch_algo.h"
#include "time_algo.h"
#include "event_algo.h"

// evaluator model.
// 
// class of evaluator model has the following methods
//
//	template<class KeysT, class ValuesT>
//	void init( KeysT const& keys, ValuesT const& values, int i, int component_size )
//
//	template<typename KeysT>
//	ValueT evaluate( float nonNormalizedTime, KeysT const& keys ) const
//
//	template<class ValuesT>
//	ValueT special( ValuesT const& values, int i, unsigned component_size, eSpecialEvaluate post_or_pre ) const
//

namespace mutant
{
	////////////////////////////////////////////////
	template
	<
		typename ReturnValueT,			// return value
		typename DataT,
		template<typename> class EvaluatorT,
		template<typename> class TimeAlgoT = time_algo_constant
	>
	struct interpolator
	{
		typedef DataT data_t;
		typedef typename DataT::key_vector_t	key_vector_t;
		typedef typename DataT::value_vector_t	value_vector_t;

		// concretes
		typedef TimeAlgoT<key_vector_t>			time_algo_t;
		typedef EvaluatorT<ReturnValueT>		evaluator_t;

		typedef key_search_binary<key_vector_t>	search_algo_t;

		interpolator( data_t const& data )
		:	mData( data )
		{
		}

		~interpolator()
		{
		}

		ReturnValueT value( float t )
		{
			float good_t = time_algo_t::value( mData.keys(), t );
			if( good_t >= mData.keys().back() )
				return mEvaluator.special( mData.values(), 0, mData.componentSize(), EVAL_POSTEND );
			else
                if( good_t <= mData.keys().front() )
					return mEvaluator.special( mData.values(), 0, mData.componentSize(), EVAL_PRESTART );

			if( mSearchAlgo.needNewSearch( good_t, mData.keys() ) ) {
				size_t sr = mSearchAlgo.search( mData.keys(), good_t );
				mEvaluator.init(
					mData.keys(),
					sr,
					mData.values(),
					sr * mData.componentSize(),
					mData.componentSize()
				);
			}

			return mEvaluator.evaluate( good_t, mData.keys(), mSearchAlgo.searchResult() );
		}

	private:
		evaluator_t		mEvaluator;
		search_algo_t	mSearchAlgo;
		data_t const&	mData;
	};

	////////////////////////////////////////////////
	template
	<
		typename DataT,
		class EvaluatorT,
		template<typename> class TimeAlgoT = time_algo_constant,
		class KeySearchAlgoT = key_search_binary<typename DataT::key_vector_t>
	>
	struct interpolator1
	{
		typedef DataT data_t;
		typedef typename DataT::key_vector_t	key_vector_t;
		typedef typename DataT::value_vector_t	value_vector_t;

		// concretes
		typedef TimeAlgoT<key_vector_t>			time_algo_t;
		typedef EvaluatorT						evaluator_t;
		typedef typename evaluator_t::result_type	result_type;
		typedef key_search_binary<key_vector_t>	search_algo_t;

		interpolator1( data_t const& data, evaluator_t const& eval = evaluator_t() )
		:	mEvaluator( eval ), mData( data )
		{
		}

		~interpolator1()
		{
		}

		result_type value( float t )
		{
			float good_t = mTimeAlgo.value( mData.keys(), t );

			if( good_t >= mData.keys().back() )
				return mEvaluator.special( mData.values(), 0, mData.componentSize(), mutant::EVAL_POSTEND ); 
			else
				if( good_t <= mData.keys().front() )
					return mEvaluator.special( mData.values(), 0, mData.componentSize(), mutant::EVAL_PRESTART );

			if( mSearchAlgo.needNewSearch( good_t, mData.keys() ) )
			{
				size_t sr = mSearchAlgo.search( mData.keys(), good_t );
				mEvaluator.init(
					mData.keys(),
					sr,
					mData.values(),
					sr * mData.componentSize(),
					mData.componentSize()
				);
			}

			return mEvaluator.evaluate( good_t, mData.keys(), (unsigned int)mSearchAlgo.searchResult() );
		}

//		time_algo_t const& time_algo() const { return mTimeAlgo; }
		unsigned cycle_number() const { return mTimeAlgo.cycle_count(); }

	private:
		evaluator_t		mEvaluator;
		search_algo_t	mSearchAlgo;
		time_algo_t		mTimeAlgo;
		data_t const&	mData;
	};

	////////////////////////////////////////////////
	template<typename ValueT>
	struct constant_interpolator
	{
		typedef ValueT	result_type;

		constant_interpolator( ValueT const& value )
			:	mValue( value ) {}

		result_type value( float t ) const {
			return mValue;
		}

	private:
		ValueT	mValue;		
	};
}

#endif // MUTANT_INTERPOLATOR_H_
