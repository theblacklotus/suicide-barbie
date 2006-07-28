#ifndef MUTANT_BINARY_INPUT_H_
#define MUTANT_BINARY_INPUT_H_

#include <memory>
#include "binary_io.h"

namespace mutant
{
	class mutant_plain_input : public binary_input
	{
	public:
		mutant_plain_input( std::auto_ptr<binary_input>& input )
		:	mInput(input) {
		}

		~mutant_plain_input() {
		}

		virtual void read( void* dest, int n, int* wasRead ) {
			mInput->read( dest, n, wasRead );
		}
		
	private:
		std::auto_ptr<binary_input>	mInput;
	};
}

#endif
