#ifndef MUTANT_SPECIAL_EVALUATOR_H_
#define MUTANT_SPECIAL_EVALUATOR_H_

namespace mutant
{
	////////////////////////////////////////////////
	template<typename ValueT>
	struct select_evaluator
	{
		typedef ValueT result_type;

		enum { inputs = 1 };

		// i - key index, such that t[i] <= t < t[i+1]
		template<class KeysT, class ValuesT>
		void init( KeysT const& keys, ValuesT const& values, size_t i, size_t component_size )
		{
			mData = &values[ i ];
		}

		template<typename KeysT>
		ValueT evaluate( float nonNormalizedTime, KeysT const& keys, size_t i ) const
		{
			return *mData;
		}

		template<class ValuesT>
		result_type special( ValuesT const& values, size_t offset, size_t component_size, eSpecialEvaluate spec ) const {
			if( spec == EVAL_POSTEND )
				return values[ values.size() - component_size + offset ];
			else
				return values[ offset ];
		}

	private:
		ValueT* mData;
	};

	////////////////////////////////////////////////
	template<typename ValueT>
	struct constant_evaluator
	{
		typedef ValueT result_type;

		enum { inputs = 1 };

		constant_evaluator( ValueT const& value )
			:	mData( value ) {}

		// i - key index, such that t[i] <= t < t[i+1]
		template<class KeysT, class ValuesT>
		void init( KeysT const& keys, ValuesT const& values, size_t i, size_t component_size )
		{
		}

		template<typename KeysT>
		ValueT evaluate( float nonNormalizedTime, KeysT const& keys, size_t i ) const
		{
			return mData;
		}

		template<class ValuesT>
		result_type special( ValuesT const& values, size_t offset, size_t component_size, eSpecialEvaluate spec ) const {
			return mData;
		}

	private:
		ValueT mData;
	};


	////////////////////////////////////////////////
	template<typename ValueT, int ValueC>
	struct convert_evaluator
	{
		typedef ValueT result_type;

		enum { inputs = ValueC };

		// i - key index, such that t[i] <= t < t[i+1]
		template<class KeysT, class ValuesT>
		void init( KeysT const& keys, ValuesT const& values, size_t i, size_t component_size ) {
			mData = compose( &values[ i ], int2type<inputs>() );
		}

		template<typename KeysT>
		ValueT evaluate( float nonNormalizedTime, KeysT const& keys, size_t i ) const {
			return mData;
		}

		template<class ValuesT>
		result_type special( ValuesT const& values, size_t offset, size_t component_size, eSpecialEvaluate spec ) const {
			if( spec == EVAL_POSTEND )
				return values[ values.size() - component_size + offset ];
			else
				return values[ offset ];
		}

	protected:
		template<typename ItT>
			result_type compose( ItT start, int2type<1> ) { return result_type( start ); }
		template<typename ItT>
			result_type compose( ItT start, int2type<2> ) { return result_type( start++, start ); }
		template<typename ItT>
			result_type compose( ItT start, int2type<3> ) { return result_type( start++, start++, start  ); }
		template<typename ItT>
			result_type compose( ItT start, int2type<4> ) { return result_type( start++, start++, start++, start ); }
		template<typename ItT>
			result_type compose( ItT start, int2type<5> ) { return result_type( start++, start++, start++, start++, start ); }
		template<typename ItT>
			result_type compose( ItT start, int2type<6> ) { return result_type( start++, start++, start++, start++, start++, start ); }
		template<typename ItT>
			result_type compose( ItT start, int2type<7> ) { return result_type( start++, start++, start++, start++, start++, start++, start ); }
		template<typename ItT>
			result_type compose( ItT start, int2type<8> ) { return result_type( start++, start++, start++, start++, start++, start++, start++, start ); }
		template<typename ItT>
			result_type compose( ItT start, int2type<9> ) { return result_type( start++, start++, start++, start++, start++, start++, start++, start++, start ); }
		template<typename ItT>
			result_type compose( ItT start, int2type<10> ) { return result_type( start++, start++, start++, start++, start++, start++, start++, start++, start ); }

	private:
		ValueT		mData;
	};
}

#endif // MUTANT_SPECIAL_EVALUATOR_H_
