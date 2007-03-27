#ifndef MUTALISK_PSP_MTXHEADER_H_
#define MUTALISK_PSP_MTXHEADER_H_

#ifndef NEWAGE_BASE_COMMON_TYPES_H		// ugly as hell :)
	typedef char s8;
	typedef short s16;
	typedef int s32;
	typedef long long s64;
	typedef unsigned char u8;
	typedef unsigned short u16;
	typedef unsigned int u32;
	typedef unsigned long long u64;
#endif

namespace mutalisk { namespace data
{
struct MtxHeader
{
	enum
	{
		Signature_CurrentVersion = '1XTM', // "MTX1" in little endian format

		MaxMipLevels = 7
	};

	struct
	{
		u32 signature;

		u16 pixelFormat;
		u16 textureWidth;
		u16 textureHeight;
		u16 textureStride;

		u32 vramAllocationSize;			// vram allocation size, in bytes

		u16	clutFormat;
		u16 clutEntries;
		u32 paletteOffset;

		union
		{
			u32 bitfield;
			struct
			{
				u32 swizzle:1;
			};
		};

	// mips are not supported for now..
//		u16 mipOffsets[MaxMipLevels];	// Offset from end of MtxHeader, divided by 16
//		u16 paletteOffset;				// Offset from end of MtxHeader, divided by 16


/*
		u32 alphaBlendEnable;			// 1 if this texture should be rendered with alphablend enabled; 0 otherwise

		int format;
		int mipmap;
		int width, height, stride;
		void* data;
		void* vramAddr;

		alpha blend func
		alpha test ref
		wrap
		clamp
*/
	};
};

} // namespace data 
} // namespace mutalisk

#endif // MUTALISK_PSP_MTXHEADER_H_
