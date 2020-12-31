#ifndef MUTANT_BINARY_COMRESSED_INPUT_H_
#define MUTANT_BINARY_COMRESSED_INPUT_H_

#include "cfg.h"

#include <memory>
#include <zlib/zlib.h>
#include "binary_io.h"

namespace mutant
{
	class mutant_compressed_input : public binary_input
	{
	public:
		enum { BUF_SIZE = 4096 };

		mutant_compressed_input( std::auto_ptr<binary_input>& input );
		~mutant_compressed_input();

		virtual void read( void* dest, int len, int* wasRead );

	protected:
		void initInflate();

		void flush();

	private:
		std::auto_ptr<binary_input>	mInput;

		z_stream		zstream;
		unsigned char*	mBuffer;
	};
}

#endif // MUTANT_BINARY_COMRESSED_INPUT_H_
