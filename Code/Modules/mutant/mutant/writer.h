#ifndef MUTANT_WRITER_H_
#define MUTANT_WRITER_H_

#include "cfg.h"

#include "binary_io.h"
#include "data.h"

#include <memory>
#include <string>

namespace mutant
{
	struct anim_character_set;
	struct anim_character;
	struct anim_clip;
	struct anim_bundle;

	struct binary_output_utils
	{
		binary_output_utils( std::auto_ptr<binary_output> output )
			:	mOutput( output ) {
		}
		
		void writeString( std::string const& str ) {
			writeDword( (unsigned)str.size() );
			mOutput->write( str.c_str(), (int)str.size(), 0 );
		}
		
		void writeDword( unsigned int dw )
		{	mOutput->write( &dw, sizeof(dw), 0 ); }
		
		void writeWord( unsigned short int w )
		{	mOutput->write( &w, sizeof(w), 0 ); }
		
		void writeByte( unsigned char c )
		{	mOutput->write( &c, sizeof(c), 0 ); }

		void writeBool( bool b )
		{	writeByte( (b? 0xff: 0x00) ); }

		
		template<typename _T>
		void writeType( _T const& d )
		{	mOutput->write( &d, sizeof(d), 0 ); }


/*_	_gcc	template<>
		void writeType<std::string>( std::string const& str )*/
		void writeType( std::string const& str )
		{	writeString( str ); }

		std::auto_ptr<binary_output> release_output()
		{	return std::auto_ptr<binary_output>( mOutput.release() ); }

		void assign_output( std::auto_ptr<binary_output> output )
		{	mOutput = output; }
		
	private:
		std::auto_ptr<binary_output>	mOutput;
	};

	struct mutant_writer
		:	public binary_output_utils
	{
	public:
		mutant_writer( std::auto_ptr<binary_output> output );
		~mutant_writer();

		void write( anim_character_set& char_set );

/*		void write( simple_scene const& scene );
		void write( data::base_mesh const& mesh );
		void write( data::dx9_mesh const& mesh );
		void write( data::skin_info const& skin );
*/

		void writeCharacterData( std::string const& char_name, anim_character& anim_char );
		void writeHierarchyData( anim_hierarchy& hier );
		void writeClipData( std::string const& clip_name, anim_clip& clip );
		void writeBundleData( std::string const& bundle_name, anim_bundle& bundle );

		template<typename _T>
		void writeAnimationData( std::string const& typeName, _T const& data, eAnimType elementType )
		{
			writeByte( elementType );
			writeByte( data.componentSize() );
			writeString( typeName );
			writeDword( data.size() );

			writeData( data.iterate_keys() );
			writeData( data.iterate_values() );
		}

		template<typename _I>
		void writeData( _I it )
		{
			while( it )
			{
				writeType( *it );
				++it;
			}
		}

		template<typename _I>
		void writeData( _I from, _I end )
		{
			while( from != end )
			{
				writeType( *from );
				++from;
			}
		}

		template<typename _I>
		void writeData( _I it, size_t count )
		{
			while( count-- )
			{
				writeType( *it );
				++it;
			}
		}
	};
}

#endif
