#ifndef MUTANT_KNOT_DATA_H_
#define MUTANT_KNOT_DATA_H_

#include <vector>
#include <cassert>
#include <ostream>

#include "types.h"
#include "iterator.h"

namespace mutant
{
	// keys:
	//  one key per value component index

	// values:
	//  N values per component, same number of components as key count
	template<typename KeyT, typename ValueT>
	struct knot_data
	{
	public:
		typedef KeyT key_t;
		typedef ValueT value_t;
		typedef std::vector<KeyT> key_vector_t;
		typedef std::vector<ValueT> value_vector_t;

		knot_data( key_vector_t const& keys, value_vector_t const& values, size_t componentSize )
			:	mKeys( keys ), mValues( values ), mComponentSize( componentSize )
		{
			assert( mComponentSize >= 1 && "Component has to have at least one value" );
//			assert( mKeys.size() * mComponentSize == mValues.size() && "Keys count should be equal to component count" );
		}

		key_t length() const { return mKeys.back() - mKeys.front(); }
		key_t start() const { return mKeys.front(); }

		size_t size() const
		{
//			assert( mKeys.size()*mComponentSize == mValues.size() );
			return mKeys.size();
		}
		size_t componentSize() const { return mComponentSize; }

		ValueT* component_modify( unsigned i ) const { return &values(true)[ i * mComponentSize ]; }

		ValueT const* component( unsigned i ) const { return &mValues[ i * mComponentSize ]; }
		ValueT const* lastComponent() const { return &mValues[ mValues.size()-mComponentSize ]; }

		key_vector_t const& keys() const { return mKeys; }
		value_vector_t const& values() const { return mValues; }
		
		// hack:
		value_vector_t& values(bool) const { return const_cast<value_vector_t&>( mValues ); }

		typedef mutant::const_iterator<key_vector_t> iterator_keys_t;
		typedef mutant::const_iterator<value_vector_t> iterator_values_t;
		iterator_keys_t iterate_keys() const { return iterator_keys_t( mKeys.begin(), mKeys.end() ); }
		iterator_values_t iterate_values() const { return iterator_values_t( mValues.begin(), mValues.end() ); }

		// info
		void info( std::ostream& os, std::string const& pre, int verbose )
		{
			if( verbose >= 20 )
			{
				unsigned n = (unsigned)mKeys.size();
				for( unsigned i=0; i<n; ++i )
				{
					os << pre;
					if( i < mKeys.size() )
						os << mKeys[i];
					else
						os << "--";

					os << " : ";

					if( i * componentSize() < mValues.size() ) {
						os << "{ ";
						ValueT const* pc = component( i );
						os << pc[0];
						for( unsigned q=1; q<componentSize(); ++q )
							os << "," << pc[q];
						os << " }";
					} else
						os << "--";

					os << std::endl;
				}
			}
		}

	private:
		knot_data& operator=(knot_data const& c); 

	private:
		key_vector_t const&		mKeys;
		value_vector_t const&	mValues;
		size_t					mComponentSize;
	};
}

#endif // MUTANT_KNOT_DATA_H_
