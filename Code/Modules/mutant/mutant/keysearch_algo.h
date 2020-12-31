#ifndef MUTANT_KEYSEARCH_ALGO_H_
#define MUTANT_KEYSEARCH_ALGO_H_

namespace mutant
{
	////////////////////////////////////////////////
	template<typename KeysT>
	struct key_search_base
	{
		key_search_base() : mPrevSearch(size_t(~0)) {}

		template<typename TimeT>
		bool needNewSearch( TimeT t, KeysT const& data ) const
		{
			return
				mPrevSearch > data.size() ||
				!((data[mPrevSearch] <= t) && (data[mPrevSearch+1] > t));
		}

		size_t searchResult() const
		{
			return mPrevSearch;
		}

		template<typename TimeT>
		static float u( TimeT t, size_t i, KeysT const& data )
		{
			assert( i < data.size() );
			return (t - data[i]) / (data[i+1] - data[i]);
		}

		// maya uses this to optimize per-interpolated value calculations
		template<typename TimeT>
		static float uNonNormalized( TimeT t, size_t i, KeysT const& data )
		{
			assert( i < data.size() );
			return t - data[i];
		}

	protected:
		size_t	mPrevSearch;
	};

	////////////////////////////////////////////////
	template<typename KeysT>
	struct key_search_binary : public key_search_base<KeysT>
	{
		// t must fall into [0...data[size-1].key)
		template<typename TimeT>
		size_t search( KeysT const& data, TimeT t )
		{
			assert( data.size() >= 2 );
			assert( t >= data.front() && t < data.back() );

			const size_t n = data.size();

			size_t e = n-1;
			size_t b = 0;
			size_t m;

			do
			{
				m = (b + e) >> 1;

				if( t < data[m] )
					e = m;
				else
				{
					if( data[m+1] > t ) {
						this->mPrevSearch = m;
						return m;
					}

					b = m;
				}
			}  while( b != e );
            
			assert( "search_key::search - t should fall into data time interval" );
			return 0;
		}
	};

	////////////////////////////////////////////////
	template<typename KeysT>
	struct key_search_direct : public key_search_base<KeysT>
	{
		// t must fall into [0...data[size-1].key)
		template<typename TimeT>
		size_t search( KeysT const& data, TimeT t )
		{
			typedef typename KeysT::value_type value_type;

			assert( data.size() >= 2 );
			assert( t >= data.front() && t < data.back() );

			value_type deltaStep = data[ 1 ] - data[ 0 ];
			if( deltaStep <= std::numeric_limits<value_type>::epsilon() )
				this->mPrevSearch = 0;
			else
			{
				size_t idx0 = size_t( t / deltaStep );
				assert( idx0 < data.size() );
				this->mPrevSearch = idx0;
			}

			return this->mPrevSearch;
		}

		// todo: dunno if this is faster than result cacheing
		template<typename TimeT>
		bool needNewSearch( TimeT t, KeysT const& data ) const
		{
			return true;
		}
	};
}

#endif // MUTANT_KEYSEARCH_ALGO_H_
