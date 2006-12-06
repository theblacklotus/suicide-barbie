#ifndef MUTANT_UTIL_H_
#define MUTANT_UTIL_H_

#include "cfg.h"

#include <string>

namespace mutant { namespace util {

	template<typename R, typename A, typename T>
	inline R const lerp( A const& a1, A const& a2, T const& t )
	{
		return R( a1 + (a2 - a1)*t );
	}

	template<typename R, typename A, typename T>
	R const catmull_rom( A const& a0, A const& a1, A const& a2, A const& a3, T const& t )
	{
		T tt = t*t;
		T ttt = t*tt;
		return R( (a0 * ( -t     + tt+tt   - ttt      ) +
					a1 * ( 2.0f   - 5.0f*tt + ttt*3.0f ) +
					a2 * (  t     + 4.0f*tt - ttt*3.0f ) +
					a3 * (        - tt      + ttt      ) ) * 0.5f );
	}

	template<typename R, typename D>
	R cubic( D const& data, float u )
	{
		return 
			data[0]*u*u*u	+
			data[1]*u*u		+
			data[2]*u		+
			data[3];
	}

	template<typename R, typename A, typename T>
	R const bezier( A const& a0, A const& a1, A const& t0, A const& t1, T const& t )
	{
		return R();
	}

	/*inline std::string& lower( std::string& s ) {
		if( !s.empty() )
			for( unsigned int i=0; i<s.size(); ++i )
				if( s[i] >= 'A' && s[i] <= 'Z' )
					s[i] += 'a'-'A';
		return s;
	}

	inline std::string& upper( std::string& s ) {
		if( !s.empty() )
			for( unsigned int i=0; i<s.size(); ++i )
				if( s[i] >= 'a' && s[i] <= 'z' )
					s[i] -= 'a'-'A';
		return s;
	}*/

	template<typename ValueContainerT>
		typename ValueContainerT::value_type const& component( ValueContainerT const& data, int component_index, int component_size ) {
			return data[ component_index * component_size ];
		}

	template<typename ValueContainerT>
		typename ValueContainerT::value_type const& component( ValueContainerT const& data, int component_index, int component_size, int offset ) {
			return data[ component_index * component_size + offset ];
		}

	template<typename ValueT>
	struct value_iterator
	{
		typedef ValueT const*	const_pointer;

		value_iterator( const_pointer b, const_pointer e ) : mCur( b ), mEnd( e ) {}

		bool good() { return mCur != mEnd; }
		value_iterator const& operator++() { mCur++; return *this; }
		ValueT operator*() { return *mCur; }

	private:
		const_pointer	mCur, mEnd;
	};
}

}

#endif // MUTANT_UTIL_H_
