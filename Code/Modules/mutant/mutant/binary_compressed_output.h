#ifndef MUTANT_BINARY_COMPRESSED_OUTPUT_H_
#define MUTANT_BINARY_COMPRESSED_OUTPUT_H_

#include "cfg.h"

#include <memory>

#include <zlib/zlib.h>
#include "binary_io.h"

namespace mutant
{
	class mutant_compressed_output : public binary_output
	{
	public:
		enum { BUF_LEN = 1024 };

		mutant_compressed_output( std::auto_ptr<binary_output>& output );
		~mutant_compressed_output();

		virtual void write( void const* src, int len, int* wasWritten );

	protected:
		void initDeflate();
		void flush();

	private:
		std::auto_ptr<binary_output>	mOutput;
		z_stream						zstream;
		unsigned char*					mBuffer;
	};
}

#endif // MUTANT_BINARY_COMPRESSED_OUTPUT_H_
