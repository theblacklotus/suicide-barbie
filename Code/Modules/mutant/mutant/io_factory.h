#ifndef MUTANT_READER_FACTORY_H_
#define MUTANT_READER_FACTORY_H_

#include "cfg.h"

#include <memory>
#include "binary_compressed_input.h"
#include "binary_input.h"
#include "binary_compressed_output.h"
#include "binary_output.h"
#include "binary_io_platform.h"
#include "data.h"
#include "errors.h"

namespace mutant
{
	struct writer_factory
	{
		enum eWriteType
		{
			COMPRESSED,
			PLAIN,

			UNKNOWN = 0x31337
		};

		static std::auto_ptr<binary_output> createOutput( std::string const& name, eWriteType type = COMPRESSED )
		{
			typedef std::auto_ptr<binary_output> ret_ptr;
			try
			{
				ret_ptr out( new file_output(name) );

				switch( type )
				{
				case COMPRESSED: {
					unsigned int magic = ANIM_COMPR_MAGIC;
					out->write( &magic, sizeof(magic), 0 );
					return ret_ptr( new mutant_compressed_output(out) );
				}
				case PLAIN: {
					unsigned int magic = ANIM_MAGIC;
					out->write( &magic, sizeof(magic), 0 );
					return ret_ptr( new mutant_plain_output(out) );
				}
				default:
					THROW_MutantError( "Invalid write type id passed" );
				}
			} catch( EIoEof& ) {
				THROW_MutantError( "unexpected end of file while writing `" + name + "'" );
			} catch( EIoError& ) {
				THROW_MutantError( "Error while writing `" + name + "'" );
			}
			return ret_ptr(NULL);
		}
	};


	struct reader_factory
	{
		static writer_factory::eWriteType getFileType( std::string const& name )
		{
			file_input in(name);

			unsigned int magic = 0;
			in.read( &magic, sizeof(magic), 0 );

			switch( magic )
			{
			case ANIM_COMPR_MAGIC:
				return writer_factory::COMPRESSED;
			case ANIM_MAGIC:
				return writer_factory::PLAIN;
			default:
				return writer_factory::UNKNOWN;
			}
		}

		static std::auto_ptr<binary_input> createInput( std::string const& name ) {
			typedef std::auto_ptr<binary_input> ret_ptr;

			try
			{
				ret_ptr in( new file_input(name) );

				unsigned int magic = 0;
				in->read( &magic, sizeof(magic), 0 );

				switch( magic )
				{
				case ANIM_COMPR_MAGIC:
					return ret_ptr( new mutant_compressed_input(in) );
				case ANIM_MAGIC:
					return ret_ptr( new mutant_plain_input(in) );
				default:
					THROW_IoError( "Invalid magic token. File corrupted or unsupported version" );
				}
			} catch( EIoEof& ) {
				THROW_MutantError( "unexpected end of file while reading `" + name + "'" );
			} catch( EIoError& e ) {
				THROW_MutantError( std::string("Error while reading file. ") + e.what() );
			}
			return ret_ptr(NULL);
		}
	};
}

#endif // MUTANT_READER_FACTORY_H_
