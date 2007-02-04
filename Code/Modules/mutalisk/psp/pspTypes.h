#ifndef MUTALISK_PSP_TYPES_H_
#define MUTALISK_PSP_TYPES_H_

#include "../cfg.h"

namespace mutalisk { namespace data
{
	typedef unsigned int size_t;

	struct Texture
	{
		int format;
		int mipmap;
		int width, height, stride;
		void* data;
		void* vramAddr;
	};

} // namespace data
} // namespace mutalisk


#endif // MUTALISK_PSP_TYPES_H_
