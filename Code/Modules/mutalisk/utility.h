#ifndef MUTALISK_DATA_UTILITY_H_
#define MUTALISK_DATA_UTILITY_H_

#include "cfg.h"
#include "types.h"

namespace mutalisk { namespace data
{
	#define COLOR_CHANNEL(v, shift) ((unsigned int)((v) * 255.0f) << (shift))

	unsigned colorRGBtoDWORD(float r, float g, float b)
	{
		return COLOR_CHANNEL(r, 0) | COLOR_CHANNEL(g, 8) | COLOR_CHANNEL(b, 16);
	}
	unsigned colorRGBAtoDWORD(float r, float g, float b, float a)
	{
		return COLOR_CHANNEL(r, 0) | COLOR_CHANNEL(g, 8) | COLOR_CHANNEL(b, 16) | COLOR_CHANNEL(a, 24);
	}

	unsigned colorRGBtoDWORD(mutalisk::data::Color const& srcColor)
	{
		return colorRGBtoDWORD(srcColor.r, srcColor.g, srcColor.b);
	}
	unsigned colorRGBAtoDWORD(mutalisk::data::Color const& srcColor)
	{
		return colorRGBAtoDWORD(srcColor.r, srcColor.g, srcColor.b, srcColor.a);
	}

} // namespace data
} // namespace mutalisk

#endif // MUTALISK_DATA_UTILITY_H_
