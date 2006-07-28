#ifndef MUTANT_TYPES_H_
#define MUTANT_TYPES_H_

#include "cfg.h"

#include <utility>
#include <iosfwd>
#include <functional>
#include <vector>
#include <stdexcept>
//#include <d3dx9.h>

// type output operators

namespace mutant
{
	enum eSpecialEvaluate {
        EVAL_POSTEND,
		EVAL_PRESTART
	};

//	typedef float			mFloat;
//	typedef D3DXVECTOR2		mVec2;
//	typedef D3DXVECTOR3		mVec3;
//	typedef D3DXQUATERNION	mQuat;

	// exceptions
	struct mutant_error : public std::runtime_error {
		mutant_error( std::string const& what ) : std::runtime_error( what ) {}
	};

	inline void mutant_throw( std::string const& msg ) {
		/*_ __no_exceptions throw mutant_error( msg );*/
	}

	// meta

	struct nil_t {};

	namespace detail
	{
		template<typename _T> struct is_nil { enum { value = false }; };
		template<> struct is_nil<nil_t> { enum { value = true }; };
	}

	template<int _T>
	struct int2type {
		enum { value = _T };
	};

	template<typename _T>
	struct type2type {
		typedef _T type;
	};

	struct pair_less {
		template<typename _T1, typename _T2>
			bool operator()( std::pair<_T1,_T2> const& p, _T1 const& v ) const { return p.first < v; }
		template<typename _T1, typename _T2>
			bool operator()( _T1 const& v, std::pair<_T1,_T2> const& p ) const { return v < p.first; }
	};

	struct pair_key_equal {
		template<typename _T1, typename _T2>
			bool operator()( std::pair<_T1,_T2> const& p, _T1 const& v ) const { return p.first == v; }
	};

	template<typename _PAIR>
	struct sel1st : public std::unary_function<_PAIR,typename _PAIR::first_type> {
		typename _PAIR::first_type const& operator()( _PAIR const& p ) const { return p.first; }
	};

	template<typename _PAIR>
	struct sel2nd : public std::unary_function<_PAIR,typename _PAIR::second_type> {
		typename _PAIR::second_type const& operator()( _PAIR const& p ) const { return p.second; }
	};

	struct mutlog_t {
		mutlog_t() : mEnabled( true ) {}

		template<typename _T>
			mutlog_t& operator<<( _T const& t ) {
/*_				if( enabled() )
					std::cout << t;
*/				return *this;
			}

		bool enabled() const { return mEnabled; }
		void setEnabled( bool e ) { mEnabled = e; }

	private:
		bool mEnabled;
	};

	template<class _ARG>
	struct deleter : public std::unary_function<_ARG,void> {
		void operator()( _ARG const& __x ) const { delete __x; }
	};

	struct holder
	{
		holder( void const* d ) :	mData( d ) {}
		virtual ~holder() /*_ _gcc= 0*/ {};
	protected:
		void const* mData;
	};

	template<class _D>
	struct holder_impl : public holder
	{
		holder_impl( _D const* d ) : holder( d ) {}
		~holder_impl() {
			delete reinterpret_cast<_D const*>(mData);
			mData = 0;
		}
	};

	template<class DataT>
	holder_impl<DataT>* new_holder( DataT const* data ) {
		return new holder_impl<DataT>( data );
	}
} // mutant

namespace mutant
{
	template <class OP1, class OP2>
	class compose_f_gx_t
	: public std::unary_function<typename OP2::argument_type,
								typename OP1::result_type>
	{
	private:
		OP1 op1;    // process: op1(op2(x))
		OP2 op2;
	public:
		// constructor
		compose_f_gx_t(const OP1& o1, const OP2& o2)
		: op1(o1), op2(o2) {
		}

		// function call
		typename OP1::result_type
		operator()(const typename OP2::argument_type& x) const {
			return op1(op2(x));
		}
	};


	/* convenience functions for the compose_f_gx adapter
	*/
	template <class OP1, class OP2>
	inline compose_f_gx_t<OP1,OP2>
	compose_f_gx (const OP1& o1, const OP2& o2) {
		return compose_f_gx_t<OP1,OP2>(o1,o2);
	}
}

#endif // MUTANT_TYPES_H_
