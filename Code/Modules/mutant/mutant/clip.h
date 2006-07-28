#ifndef MUTANT_CLIP_H_
#define MUTANT_CLIP_H_

#include <map>
#include <string>
#include <algorithm>

//#include <boost/compose.hpp>

#include "knot_data.h"
#include "bundle.h"

namespace mutant
{
	// holds animation of node set by clip name (as in maya)
	struct anim_clip
	{
	public:
		typedef std::vector<holder*> resource_cont_t;
		typedef std::map<std::string,anim_bundle*> bundle_map_t;

		enum eFlags
		{
			flattened	= 0x01
		};

		anim_clip()
			:	mClipLength( 0.0f ) {
		}

		~anim_clip() {
			clear();
		}

		bool is_flag( eFlags fl ) const { return (mFlags & fl) != 0; }
		void set_flag( eFlags fl ) { mFlags |= fl; }
		void reset_flag( eFlags fl ) { mFlags &= ~fl; }

		unsigned flags() const { return mFlags; }
		void set_flags( unsigned fl ) { mFlags = fl; }

		// returns clip length in seconds
		float clip_length() const {
			return mClipLength;
		}

		void set_length( float l ) {
			if( l < 0.0f )
				mutant_throw( "anim_clip: Clip length must be a positive number" );
            mClipLength = l;
		}

		template<typename DataT>
		void registerData( std::auto_ptr<DataT> res ) {
			if( res.get() )
				mResources.push_back( new holder_impl<DataT>( res.release() ) );
			else
				mutant_throw( "anim_clip: Trying to register NULL ptr to data" );
		}

		// takes ownership of bundle
		void insertBundle( std::string const& bundle_name, std::auto_ptr<anim_bundle> bundle ) {
			mBundleMap.insert( std::make_pair( bundle_name, bundle.release() ) );
		}

		void clear() {
			mClipLength = 0.0f;

			std::for_each( mResources.begin(), mResources.end(),
				deleter<holder*>() );
			mResources.clear();

			
			// OLD: compose removed from boost v1.30.2
			std::for_each( mBundleMap.begin(), mBundleMap.end(),
				mutant::compose_f_gx(
					deleter<anim_bundle*>(),
					sel2nd<bundle_map_t::value_type>()
				) );

			// using bind insted of compose_f_gx
			// test( bind(f, bind(g, _1)) );


/*
			std::for_each( mBundleMap.begin(), mBundleMap.end(),
				boost::bind( 
					mutant::deleter<anim_bundle*>(),
					boost::bind( mutant::sel2nd<bundle_map_t::value_type>, _1 ) )
			);*/
			mBundleMap.clear();
		}

		anim_bundle const& operator[]( std::string const& bundle_name ) const {
			bundle_map_t::const_iterator it = mBundleMap.find( bundle_name );
			if( it == mBundleMap.end() )
				mutant_throw( std::string("No specified bundle `") + bundle_name + "' in clip" );
			return *(it->second);
		}

		anim_bundle& operator[]( std::string const& bundle_name ) {
			bundle_map_t::iterator it = mBundleMap.find( bundle_name );
			if( it == mBundleMap.end() )
				mutant_throw( std::string("No specified bundle `") + bundle_name + "' in clip" );
			return *(it->second);
		}

		bool has( std::string const& node_name ) const {
			return mBundleMap.find( node_name ) != mBundleMap.end();
		}

		// 
		typedef mutant::const_iterator<bundle_map_t> iterator_t;

		// returns number of nodes animated by this clip
		size_t size() const { return mBundleMap.size(); }
		iterator_t iterate() const { return iterator_t( mBundleMap.begin(), mBundleMap.end() ); }

		// info
		void info( std::ostream& os, std::string const& pre, int verbose ) {
			os << pre << "Length " << clip_length() << ", " << (int)mBundleMap.size() << " bundles" << " (" << (int)mResources.size() << " unique data vectors)\n";

			if( verbose >= 5 )
			{
				for( bundle_map_t::iterator it = mBundleMap.begin(); it != mBundleMap.end(); ++it ) {
					os << pre << "BUNDLE: " << it->first << "\n";
					it->second->info( os, pre + "  ", verbose );
				}
			}
		}

	private:
		anim_clip( anim_clip const& clip );
		anim_clip const& operator=( anim_clip const& clip );

	private:
		resource_cont_t		mResources;
        bundle_map_t		mBundleMap;
		float				mClipLength;
		unsigned			mFlags;
	};
}

#endif // MUTANT_CLIP_H_
