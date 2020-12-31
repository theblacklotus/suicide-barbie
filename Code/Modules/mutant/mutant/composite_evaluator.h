#ifndef MUTANT_COMPOSITE_EVALUATOR_H_
#define MUTANT_COMPOSITE_EVALUATOR_H_

namespace mutant
{
	////////////////////////////////////////////////
	template<typename ValueT, class ComposeAlgoT, class E0, class E1, class E2=nil_t, class E3=nil_t>
	struct composite_evaluator
	{
		enum {
			arg_count =
			(detail::is_nil<E2>::value ? 2 :
				(detail::is_nil<E3>::value ? 3 : 4 )
			)
		};

		typedef ValueT value_type;
		typedef typename ComposeAlgoT::value_type result_type;
		typedef ComposeAlgoT compose_algo_t;

		composite_evaluator( compose_algo_t const& ca, E0 const& e0, size_t o0, E1 const& e1, size_t o1 ) : mEvaluator0( e0 ), offset0( o0 ), mEvaluator1( e1 ), offset1( o1 ), mComposeAlgo( ca ) {}
		composite_evaluator( compose_algo_t const& ca, E0 const& e0, size_t o0, E1 const& e1, size_t o1, E2 const& e2, size_t o2 ) : mEvaluator0( e0 ), offset0( o0 ), mEvaluator1( e1 ), offset1( o1 ), mEvaluator2( e2 ), offset2( o2 ), mComposeAlgo( ca ) {}
		composite_evaluator( compose_algo_t const& ca, E0 const& e0, size_t o0, E1 const& e1, size_t o1, E2 const& e2, size_t o2, E2 const& e3, size_t o3 ) : mEvaluator0( e0 ), offset0( o0 ), mEvaluator1( e1 ), offset1( o1 ), mEvaluator2( e2 ), offset2( o2 ), mEvaluator3( e3 ), offset3( o3 ), mComposeAlgo( ca ) {}

		// i - key index, such that t[i] <= t < t[i+1]
		template<class KeysT, class ValuesT>
		void init( KeysT const& keys, size_t k, ValuesT const& values, size_t i, size_t component_size )
		{
			init( keys, k, values, i, component_size, int2type<arg_count>() );
		}

		template<typename KeysT>
		result_type evaluate( float nonNormalizedTime, KeysT const& keys, size_t i ) const
		{
			return evaluate( nonNormalizedTime, keys, i, int2type<arg_count>() );
		}

		template<class ValuesT>
		result_type special( ValuesT const& values, size_t offset, size_t component_size, eSpecialEvaluate spec ) const {
			return special( values, offset, component_size, spec, int2type<arg_count>() );
		}

	protected:
		// init
		template<class KeysT, class ValuesT>
			void init( KeysT const& keys, size_t k, ValuesT const& values, size_t i, size_t component_size, int2type<2> )
		{
			mEvaluator0.init( keys, k, values, i + offset0, component_size );
			mEvaluator1.init( keys, k, values, i + offset1, component_size );
		}

		template<class KeysT, class ValuesT>
			void init( KeysT const& keys, size_t k, ValuesT const& values, size_t i, size_t component_size, int2type<3> )
		{
			mEvaluator0.init( keys, k, values, i + offset0, component_size );
			mEvaluator1.init( keys, k, values, i + offset1, component_size );
			mEvaluator2.init( keys, k, values, i + offset2, component_size );
		}

		template<class KeysT, class ValuesT>
			void init( KeysT const& keys, size_t k, ValuesT const& values, size_t i, size_t component_size, int2type<4> )
		{
			mEvaluator0.init( keys, k, values, i + offset0, component_size );
			mEvaluator1.init( keys, k, values, i + offset1, component_size );
			mEvaluator2.init( keys, k, values, i + offset2, component_size );
			mEvaluator3.init( keys, k, values, i + offset3, component_size );
		}

		// evaluate
		template<typename KeysT>
		result_type	evaluate( float nonNormalizedTime, KeysT const& keys, size_t i, int2type<2> ) const
		{
			return mComposeAlgo(
				mEvaluator0.evaluate( nonNormalizedTime, keys, i ),
				mEvaluator1.evaluate( nonNormalizedTime, keys, i )
			);
		}

		template<typename KeysT>
		result_type evaluate( float nonNormalizedTime, KeysT const& keys, size_t i, int2type<3> ) const
		{
			return mComposeAlgo(
				mEvaluator0.evaluate( nonNormalizedTime, keys, i ),
				mEvaluator1.evaluate( nonNormalizedTime, keys, i ),
				mEvaluator2.evaluate( nonNormalizedTime, keys, i )
			);
		}

		template<typename KeysT>
		result_type evaluate( float nonNormalizedTime, KeysT const& keys, size_t i, int2type<4> ) const
		{
			return mComposeAlgo(
				mEvaluator0.evaluate( nonNormalizedTime, keys, i ),
				mEvaluator1.evaluate( nonNormalizedTime, keys, i ),
				mEvaluator2.evaluate( nonNormalizedTime, keys, i ),
				mEvaluator3.evaluate( nonNormalizedTime, keys, i )
			);
		}

		// special
		template<class ValuesT>
		result_type special( ValuesT const& values, size_t offset, size_t component_size, eSpecialEvaluate spec, int2type<2> ) const {
			return mComposeAlgo(
				mEvaluator0.special( values, offset + offset0, component_size, spec ),
				mEvaluator1.special( values, offset + offset1, component_size, spec )
			);
		}

		template<class ValuesT>
		result_type special( ValuesT const& values, size_t offset, size_t component_size, eSpecialEvaluate spec, int2type<3> ) const {
			return mComposeAlgo(
				mEvaluator0.special( values, offset + offset0, component_size, spec ),
				mEvaluator1.special( values, offset + offset1, component_size, spec ),
				mEvaluator2.special( values, offset + offset2, component_size, spec )
			);
		}

		template<class ValuesT>
		result_type special( ValuesT const& values, size_t offset, size_t component_size, eSpecialEvaluate spec, int2type<4> ) const {
			return mComposeAlgo(
				mEvaluator0.special( values, offset + offset0, component_size, spec ),
				mEvaluator1.special( values, offset + offset1, component_size, spec ),
				mEvaluator2.special( values, offset + offset2, component_size, spec ),
				mEvaluator3.special( values, offset + offset3, component_size, spec )
			);
		}

	private:
		E0	mEvaluator0;	size_t offset0;
		E1	mEvaluator1;	size_t offset1;
		E2	mEvaluator2;	size_t offset2;
		E3	mEvaluator3;	size_t offset3;
		ComposeAlgoT		mComposeAlgo;
	};
}

#endif // MUTANT_COMPOSITE_EVALUATOR_H_
