#ifndef MUTANT_ACCESS_POLICY_H_
#define MUTANT_ACCESS_POLICY_H_

#include "types.h"

namespace mutant
{
	// todo:
	// Evaluator should store value returned be access_policy as result_type,
	// a type that is embedded in access_policy. That way references to data could be
	// held if necessary instead of always storing data by value as it is currently
	// implemented in evaluator

	////////////////////////////////////////////////
	template<typename ValueT>
	struct straight_access_policy
	{
		typedef ValueT result_type;
		
		template<class ValuesT>
		static result_type at( ValuesT const& values, size_t i ) {
			return values[ i ];
		}
	};

	////////////////////////////////////////////////
	template<typename ValueT>
	struct cast_access_policy
	{
		typedef ValueT const& result_type;
/*		template<typename DataN>
		ValueT& get( ValuesT const& values, result_type result ) {
			return values[ result[DataN] ];
		}*/

		template<class ValuesT>
		static result_type at( ValuesT const& values, size_t i ) {
			return *((ValueT*)(&(values[ i ])));
		}
	};

	////////////////////////////////////////////////
	// todo: add compose_algo
	template<typename ValueT, size_t DataC>
	struct compose_access_policy
	{
		typedef ValueT result_type;

		template<class ValuesT>
		static result_type at( ValuesT const& values, size_t i ) {
			return compose( &(values[i]), int2type<DataC>() );
		}

	private:
		template<typename ItT> static result_type compose( ItT start, int2type<1> ) { return result_type( *(start + 0) ); }
		template<typename ItT> static result_type compose( ItT start, int2type<2> ) { return result_type( *(start + 0), *(start + 1) ); }
		template<typename ItT> static result_type compose( ItT start, int2type<3> ) { return result_type( *(start + 0), *(start + 1), *(start + 2) ); }
		template<typename ItT> static result_type compose( ItT start, int2type<4> ) { return result_type( *(start + 0), *(start + 1), *(start + 2), *(start + 3) ); }
		template<typename ItT> static result_type compose( ItT start, int2type<5> ) { return result_type( *(start + 0), *(start + 1), *(start + 2), *(start + 3), *(start + 4) ); }
		template<typename ItT> static result_type compose( ItT start, int2type<6> ) { return result_type( *(start + 0), *(start + 1), *(start + 2), *(start + 3), *(start + 4), *(start + 5) ); }
		template<typename ItT> static result_type compose( ItT start, int2type<7> ) { return result_type( *(start + 0), *(start + 1), *(start + 2), *(start + 3), *(start + 4), *(start + 5), *(start + 6) ); }
		template<typename ItT> static result_type compose( ItT start, int2type<8> ) { return result_type( *(start + 0), *(start + 1), *(start + 2), *(start + 3), *(start + 4), *(start + 5), *(start + 6), *(start + 7) ); }
		template<typename ItT> static result_type compose( ItT start, int2type<9> ) { return result_type( *(start + 0), *(start + 1), *(start + 2), *(start + 3), *(start + 4), *(start + 5), *(start + 6), *(start + 7), *(start + 8) ); }
		template<typename ItT> static result_type compose( ItT start, int2type<10> ) { return result_type(*(start + 0), *(start + 1), *(start + 2), *(start + 3), *(start + 4), *(start + 5), *(start + 6), *(start + 7), *(start + 8), *(start + 9) ); }
	};
}

#endif // MUTANT_ACCESS_POLICY_H_
