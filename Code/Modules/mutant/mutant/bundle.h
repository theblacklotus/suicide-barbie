#ifndef MUTANT_BUNDLE_H_
#define MUTANT_BUNDLE_H_

#include "types.h"
#include "knot_data.h"

#include <map>
#include <string>

namespace mutant
{
	// e.g.
	// c0			c1			c2
	// [f0 f1 f2]	[f0 f1 f2]	[f0 f1 f2]

	// holds set of various node animation curves, registered by names.
	// name correspond to maya keyed attribute name
	// 
	struct anim_bundle
	{
	public:
		// ANIM-ID : CURVE-DATA
		typedef std::map<std::string,knot_data<float,float> > float_float_map_t;
		typedef std::map<std::string,knot_data<float,std::string> > float_string_map_t;

		// todo: add another data if needed here
		void insertData( std::string const& typeName, knot_data<float,float> const& animData ) {
			mFloatFloat.insert( std::make_pair(typeName,animData) );
		}

		void insertData( std::string const& typeName, knot_data<float,std::string> const& animData ) {
			mFloatString.insert( std::make_pair(typeName,animData) );
		}

		knot_data<float,float> const& floatFloat( std::string const& typeName ) const
        {
			float_float_map_t::const_iterator it = mFloatFloat.find( typeName );
			if( it == mFloatFloat.end() )
				mutant_throw( "No specified float-float type animation in anim_bundle" );

			return it->second;
		}

		knot_data<float,std::string> const& floatString( std::string const& typeName ) const
        {
			float_string_map_t::const_iterator it = mFloatString.find( typeName );
			if( it == mFloatString.end() )
				mutant_throw( "No specified float-string type animation in anim_bundle" );

			return it->second;
		}

		bool has_ff( std::string const& typeName ) const {
			return mFloatFloat.find( typeName ) != mFloatFloat.end();
		}

		bool has_fs( std::string const& typeName ) const {
			return mFloatString.find( typeName ) != mFloatString.end();
		}

		// info
		typedef mutant::const_iterator<float_float_map_t> iterator_ff_t;
		size_t size_ff() const { return mFloatFloat.size(); }
		iterator_ff_t iterate_ff() const { return iterator_ff_t( mFloatFloat.begin(), mFloatFloat.end() ); }

		typedef mutant::const_iterator<float_string_map_t> iterator_fs_t;
		size_t size_fs() const { return mFloatString.size(); }
		iterator_fs_t iterate_fs() const { return iterator_fs_t( mFloatString.begin(), mFloatString.end() ); }

		void info( std::ostream& os, std::string const& pre, int verbose )
		{
			os << pre << (int)mFloatFloat.size() << " float-float curve(s):\n";
			if( verbose >= 5 ) {
				for( float_float_map_t::iterator it = mFloatFloat.begin(); it != mFloatFloat.end(); ++it ) {
					int sz = (int)(it->second.values().size() * sizeof(float_float_map_t::value_type::second_type::value_t));

					os	<< pre << it->first << " - "
						<< (int)it->second.keys().size() << " key, "
						<< (int)it->second.values().size() << " value, "
						<< (int)it->second.componentSize() << " per comp/, " << int(it->second.values().size() / it->second.componentSize()) << " comp/s, "
						<< sz << " bytes" 
						<< ": \n";
					it->second.info( os, pre + "  ", verbose );
				}
			}

			os << pre << (int)mFloatString.size() << " float-string curve(s):\n";
			if( verbose >= 5 ) {
				for( float_string_map_t::iterator it = mFloatString.begin(); it != mFloatString.end(); ++it )
				{
					int sz = (int)(it->second.values().size() * sizeof(float_string_map_t::value_type::second_type::value_t) );

					// iterate strings and accumulate theirs size
					for( float_string_map_t::value_type::second_type::value_vector_t::const_iterator sIt = it->second.values().begin(); sIt != it->second.values().end(); ++sIt )
						sz += (int)sIt->size() + 1;

					os	<< pre << it->first << " - "
						<< (int)it->second.keys().size() << " key, "
						<< (int)it->second.values().size() << " value, "
						<< (int)it->second.componentSize() << " per comp/, " << int(it->second.values().size() / it->second.componentSize()) << " comp/s, "
						<< "~" << sz << " bytes" 
						<< ": \n";

					it->second.info( os, pre + "  ", verbose );
				}
			}
		}

	private:
		float_float_map_t	mFloatFloat;
		float_string_map_t	mFloatString;
	};
}

#endif // MUTANT_BUNDLE_H_
