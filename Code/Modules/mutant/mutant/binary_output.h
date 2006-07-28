#ifndef MUTANT_BINARY_OUTPUT_H_
#define MUTANT_BINARY_OUTPUT_H_

namespace mutant
{
	class mutant_plain_output : public binary_output
	{
	public:
		mutant_plain_output( std::auto_ptr<binary_output>& output )
		:	mOutput( output ) {
		}

		~mutant_plain_output() {
		}

		virtual void write( void const* src, int n, int* wasWritten ) {
			mOutput->write( src, n, wasWritten );
		}
		
	private:
		std::auto_ptr<binary_output> mOutput;
	};
}

#endif // MUTANT_BINARY_OUTPUT_H_
