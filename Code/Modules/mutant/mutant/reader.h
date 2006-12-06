#ifndef MUTANT_READER_H_
#define MUTANT_READER_H_

#include "cfg.h"

#include "binary_io.h"
#include "data.h"
#include "types.h"

namespace mutant
{
	struct binary_input_utils
	{
	public:
		binary_input_utils( std::auto_ptr<binary_input> input )
		:	mInput( input )
		{
		}

		~binary_input_utils()
		{
		};

		std::string readString();
		std::string& readString( std::string& str );

/*		std::string readString()
		{
			std::string str;
			unsigned char c;
			int rd = 0;
			unsigned size = readDword();

			while( size-- ) {
				mInput->read( &c, sizeof(c), &rd );
				if( !rd )
					break;
//				if( c == '\x0' )
//					break;

				str += c;
			}
			return str;
		}

		std::string& readString2(std::string& str)
		{
			unsigned char c;
			int rd = 0;
			unsigned size = readDword();

			while( size-- ) {
				mInput->read( &c, sizeof(c), &rd );
				if( !rd )
					break;
				str += c;
			}
			return str;
		}


		std::string readStringOld()
		{
			std::string str;
			unsigned char c;
			int rd = 0;

			while( 1 ) {
				mInput->read( &c, sizeof(c), &rd );
				if( !rd )
					break;
				if( c == '\x0' )
					break;

				str += c;
			}
			return str;
		}*/

		unsigned int readDword()
		{
			unsigned int i = 0;
			mInput->read( &i, sizeof(i), 0 );
			return i;
		}

		unsigned short int readWord()
		{
			unsigned short int i = 0;
			mInput->read( &i, sizeof(i), 0 );
			return i;
		}

		unsigned char readByte()
		{
			unsigned char c = 0;
			mInput->read( &c, sizeof(c), 0 );
			return c;
		}

		bool readBool()
		{	return (readByte() != 0x00); }

		template<typename T>
		void readType( T& d )
		{
			mInput->read( &d, sizeof(d), 0 );
		}

/*_		template<>
		void readType<std::string>( std::string& d )*/
		void readType( std::string& d )
		{
			readString( d );
		}

		std::auto_ptr<binary_input> release_input() {
			return std::auto_ptr<binary_input>( mInput.release() );
		}

	private:
		std::auto_ptr<binary_input>		mInput;
	};

	//
	struct mutant_reader
		:	public binary_input_utils
	{
	public:

		mutant_reader( std::auto_ptr<binary_input> input );
		virtual ~mutant_reader();

		virtual void read( anim_character_set& char_set );
		virtual void read( simple_skinned& data ) {}

/*		virtual void read( simple_scene& scene );
		virtual void read( data::base_mesh& mesh );
		virtual void read( data::dx9_mesh& mesh );
		virtual void read( data::skin_info& skin );
*/

		std::string readCharacter( anim_character& anim_char );
		std::string readClip( anim_clip& clip );
		std::string readBundle( anim_bundle& bundle, anim_clip& clip );

		void readAnimation( anim_bundle& bundle, anim_clip& clip );
		void readHierarchy( anim_hierarchy& hier );

		template<typename _T>
		void readVector( _T& v, int count )
		{
			typename _T::value_type data;
			v.reserve( v.size() + count );
			while( count-- )
			{
				readType( data );
				v.push_back( data );
			}
		}
		template<typename _T>
		void readArray( _T* v, int count )
		{
			for( int q = 0; q < count; ++q )
			{
				 readType( v[q] );
			}
		}

		void enableLog( bool enabled = true ) { mutlog.setEnabled( enabled ); }

	private:
		mutlog_t	mutlog;
	};
}

#endif
