#ifndef TIME_BLOCK_H_
#define TIME_BLOCK_H_

#include "cfg.h"
#include <pspkernel.h>

namespace mutalisk
{
	void initTickFrequency();
	float tickFrequency();
	float tickResolution();

	struct TimeBlock
	{
		TimeBlock();
		void peek();
		float ms();

		u64 currTick;
		u64 prevTick;
	};
}

#endif // TIME_BLOCK_H_
