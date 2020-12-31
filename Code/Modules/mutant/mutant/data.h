#ifndef MUTANT_DATA_H_
#define MUTANT_DATA_H_

#include "types.h"
#include "iterator.h"
#include "clip.h"
#include "bundle.h"
#include "character.h"
#include "hierarchy.h"
#include "simple_skinned.h"

//#include <boost/compose.hpp>
#include <cassert>

#include <vector>
#include <map>
#include <set>
#include <string>
#include <algorithm>
#include <functional>

/* MUTANT file format:

[header]
<d>	MAGIC (=MUT1)
<d> CHARACTER-COUNT

	[character]
	<s> CHARACTER-NAME
	<d> HIERARCHY-COUNT
	<d>	CLIP COUNT
		[hier]
		<s> hierarchy-name
		<d> node-count
			[node]
			<s> string
			<d> child count
				<d> child ids

		[clip]
		<s> CLIP-NAME
		<d> clip_flags
		<f> clip_length
		<d> marker-count
			<f> position in clip time
			<s> marker-name
		<d>	BUNDLES-COUNT
			[bundle]
			<s>	BUNDLE-NAME
			<d> ANIMS-COUNT
			[<params> ...................]
				[anim-curve-content]:
				<b> ELEMENT-TYPE (float/string/vector)
				<b>	COMPONENT-SIZE
				<s>	SEMANTIC (TYPE)
				<d>	KEY_COUNT
//				<b> params size (to skip if needed)
//				 <b> bool: weighted keys
//				 <b> bool: 
//				 <b> pre-infinity-type
//				 <b> post-infinity-type
				[<params> ..... curve type/tangent type and so on]
				<DATA>
				[ key-vector ]
				[ data-vector ]
*	
*/

// info
// >=5	- traverse contained children
// >=10	- show contained data (less-equal than 5 lines)
// >=20 - show contained data (everything)

namespace mutant
{
	#define FOURCC(a,b,c,d) ((a << 24) + (b << 16) + (c << 8) + d)

	// number of components per channel
	enum eAnimType
	{
		ANIM_UNKNOWN = 0,

		ANIM_FLOAT,
		ANIM_STRING,
		ANIM_VECTOR3,

		ANIM_MAX,

		ANIM_MAGIC = FOURCC('1','T','U','M'),
		ANIM_COMPR_MAGIC = FOURCC('1','c','T','M')
	};

	////////////////////////////////////////////////
	template<typename ValueT>
	struct property
	{
		property( bool valid, ValueT const& value = ValueT() )
			:	mValue( value ), mValid( valid ) {}

		operator bool() const { return mValid; }
		ValueT const& get() const { return mValue; }
		void set( ValueT const& v ) { mValue = v; }

	private:
		ValueT	mValue;
		bool	mValid;
	};	

	////////////////////////////////////////////////
	struct property_sheet
	{
		typedef std::map<std::string, std::string> properties_t;

		void insert( std::string const& prop, std::string const& value )
		{
			mProperties.insert( std::make_pair( prop, value ) );
		}

		void remove( std::string const& prop )
		{
			mProperties.erase( prop );
		}

		// todo:
		// make checks on pull functions to find invalid values
		template<typename ValueT>
		property<ValueT> as( std::string const& prop )
		{
			typedef property<ValueT> t_value;

			properties_t::iterator it = mProperties.find( prop );
			if( it == mProperties.end() )
				return t_value( false );
			else
				return pull( it->second, type2type<ValueT>() );
		}

	private:
		property<std::string> pull( std::string const& asString, type2type<std::string> ) const {
			return property<std::string>( true, asString );
		}

		property<int> pull( std::string const& asString, type2type<int> ) const {
			return property<int>( true, atoi( asString.c_str() ) );
		}

		property<float> pull( std::string const& asString, type2type<float> ) const {
			return property<float>( true, (float)atof( asString.c_str() ) );
		}

	private:
        properties_t	mProperties;
	};	

	////////////////////////////////////////////////
	// holds all characters read from file
	struct anim_character_set
	{
	public:
		typedef std::pair<std::string,anim_character*> char_pair_t;
		typedef std::vector<char_pair_t> char_vector_t;
		typedef mutant::iterator<char_vector_t> char_it_t;

		~anim_character_set()
		{
			std::for_each(
				mChars.begin(),
				mChars.end(),
				mutant::compose_f_gx(
					mutant::deleter<anim_character*>(),
					mutant::sel2nd<char_pair_t>()
				)
			);
		}

		void insertCharacter( std::string const& name, std::auto_ptr<anim_character> character ) {
			mChars.push_back(
				std::make_pair( name, character.release() )
			);
		}

		anim_character& operator[]( std::string const& name ) const {
			char_vector_t::const_iterator it = findCharacter( name );
			if( it == mChars.end() )
				mutant_throw( std::string("Cannot find '") + name + "' character" );

			return *it->second;
		}

		anim_character& operator[]( unsigned int index ) const {
			assert( index < mChars.size() );
			return *mChars[ index ].second;
		}

		char_pair_t const& charPair( unsigned int index ) const {
			assert( index < mChars.size() );
			return mChars[ index ];
		}

		size_t size() const { return mChars.size(); }
		bool has( std::string const& character_name ) const {
			return findCharacter( character_name ) != mChars.end();
		}

		char_it_t iterate() { return char_it_t( mChars.begin(), mChars.end() ); }

		void info( std::ostream& os, std::string const& pre, int verbose = 5 ) {
			if( verbose > 0 )
				os << (int)mChars.size() << " characters" << std::endl;

			if( verbose >= 5 )
			{
				for( char_it_t it = iterate(); it; ++it )
				{
					os << "Character `" << it->first << "'" << std::endl;
					it->second->info( os, pre, verbose );
				}
			}
		}

	protected:
		char_vector_t::const_iterator findCharacter( std::string const& char_name ) const
		{
			return std::find_if(
				mChars.begin(),
				mChars.end(),
				mutant::compose_f_gx(
					std::bind2nd( std::equal_to<std::string>(), char_name ),
					sel1st<char_pair_t>()
				)
			);
		}

	private:
		char_vector_t	mChars;
	};
}

#endif
