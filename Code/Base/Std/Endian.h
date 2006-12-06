
#ifndef NEWAGE_BASE_STD_ENDIAN_H
#define NEWAGE_BASE_STD_ENDIAN_H

#if defined AMIGA
#define NEWAGE_BIG_ENDIAN
#elif defined WIN32 || defined __psp__
#else
#error Platform not defined
#endif

static inline u16 swap16(u16 i)
{
	return (u16)(((((unsigned int) i) & 0xff00) >> 8)
		| ((((unsigned int) i) & 0xff) << 8));
}

static inline u32 swap32(u32 i)
{
	return (u32)(((((unsigned int) i) & 0xff000000) >> 24)
		| ((((unsigned int) i) & 0xff0000) >> 8)
		| ((((unsigned int) i) & 0xff00) << 8)
		| ((((unsigned int) i) & 0xff) << 24));
}

static inline u64 swap64(u64 i)
{
	return (((u64) ( swap32((u32) i))) << 32) | ((u64) (swap32((u32) (i >> 32))));
}

static inline unsigned int endianReadU8Little(u8* mem)
{
	return *mem;
}

static inline unsigned int endianReadU8Big(u8* mem)
{
	return *mem;
}

static inline int endianReadS8Little(s8* mem)
{
	return *mem;
}

static inline int endianReadS8Big(s8* mem)
{
	return *mem;
}

static inline unsigned int endianReadU16Little(u16* mem)
{
#ifdef NEWAGE_BIG_ENDIAN
	return swap16(*mem);
#else
	return *mem;
#endif
}

static inline unsigned int endianReadU16Big(u16* mem)
{
#ifdef NEWAGE_BIG_ENDIAN
	return *mem;
#else
	return swap16(*mem);
#endif
}

static inline int endianReadS16Little(s16* mem)
{
#ifdef NEWAGE_BIG_ENDIAN
	return swap16(*mem);
#else
	return *mem;
#endif
}

static inline int endianReadS16Big(s16* mem)
{
#ifdef NEWAGE_BIG_ENDIAN
	return *mem;
#else
	return swap16(*mem);
#endif
}

static inline unsigned int endianReadU32Little(u32* mem)
{
#ifdef NEWAGE_BIG_ENDIAN
	return swap32(*mem);
#else
	return *mem;
#endif
}

static inline unsigned int endianReadU32Big(u32* mem)
{
#ifdef NEWAGE_BIG_ENDIAN
	return *mem;
#else
	return swap32(*mem);
#endif
}

static inline int endianReadS32Little(s32* mem)
{
#ifdef NEWAGE_BIG_ENDIAN
	return swap32(*mem);
#else
	return *mem;
#endif
}

static inline int endianReadS32Big(s32* mem)
{
#ifdef NEWAGE_BIG_ENDIAN
	return *mem;
#else
	return swap32(*mem);
#endif
}

static inline float endianReadFloat32Little(float* mem)
{
#ifdef NEWAGE_BIG_ENDIAN
	union
	{
		u32 i;
		float f;
	} x;
	x.i = swap32(*((u32*) mem));
	return x.f;
#else
	return *mem;
#endif
}

static inline float endianReadFloat32Big(float* mem)
{
#ifdef NEWAGE_BIG_ENDIAN
	return *mem;
#else
	union
	{
		u32 i;
		float f;
	} x;
	x.i = swap32(*((u32*) mem));
	return x.f;
#endif
}

static inline double endianReadFloat64Little(double* mem)
{
#ifdef NEWAGE_BIG_ENDIAN
	union
	{
		u64 i;
		double d;
	} x;
	x.i = swap64(*((u64*) mem));
	return x.d;
#else
	return *mem;
#endif
}

static inline double endianReadFloat64Big(double* mem)
{
#ifdef NEWAGE_BIG_ENDIAN
	return *mem;
#else
	union
	{
		u64 i;
		double d;
	} x;
	x.i = swap64(*((u64*) mem));
	return x.d;
#endif
}

static inline float endianReadFloatLittle(float* mem)
{
	return endianReadFloat32Little(mem);
}

static inline float endianReadFloatBig(float* mem)
{
	return endianReadFloat32Big(mem);
}

static inline double endianReadDoubleLittle(double* mem)
{
	return endianReadFloat64Little(mem);
}

static inline double endianReadDoubleBig(double* mem)
{
	return endianReadFloat64Big(mem);
}


static inline void endianWriteU8Little(u8* mem, u8 data)
{
	*mem = data;
}

static inline void endianWriteU8Big(u8* mem, u8 data)
{
	*mem = data;
}

static inline void endianWriteS8Little(s8* mem, s8 data)
{
	*mem = data;
}

static inline void endianWriteS8Big(s8* mem, s8 data)
{
	*mem = data;
}

static inline void endianWriteU16Little(u16* mem, u16 data)
{
#ifdef NEWAGE_BIG_ENDIAN
	*mem = swap16(data);
#else
	*mem = data;
#endif
}

static inline void endianWriteU16Big(u16* mem, u16 data)
{
#ifdef NEWAGE_BIG_ENDIAN
	*mem = data;
#else
	*mem = swap16(data);
#endif
}

static inline void endianWriteS16Little(s16* mem, s16 data)
{
	endianWriteU16Little((u16*) mem, (u16) data);
}

static inline void endianWriteS16Big(s16* mem, s16 data)
{
	endianWriteU16Big((u16*) mem, (u16) data);
}

static inline void endianWriteU32Little(u32* mem, u32 data)
{
#ifdef NEWAGE_BIG_ENDIAN
	*((u32*) mem) = swap32(data);
#else
	*((u32*) mem) = data;
#endif
}

static inline void endianWriteU32Big(u32* mem, u32 data)
{
#ifdef NEWAGE_BIG_ENDIAN
	*mem = data;
#else
	*mem = swap32(data);
#endif
}

static inline void endianWriteS32Little(s32* mem, s32 data)
{
	endianWriteU32Little((u32*) mem, (u32) data);
}

static inline void endianWriteS32Big(s32* mem, s32 data)
{
	endianWriteU32Big((u32*) mem, (u32) data);
}

static inline void endianWriteFloat32Little(float* mem, float data)
{
#ifdef NEWAGE_BIG_ENDIAN
	union
	{
		u32 i;
		float f;
	} x;
	x.f = data;
	x.i = swap32(x.i);
	*mem= x.f;
#else
	*mem = data;
#endif
}

static inline void endianWriteFloat32Big(float* mem, float data)
{
#ifdef NEWAGE_BIG_ENDIAN
	*mem = data;
#else
	union
	{
		u32 i;
		float f;
	} x;
	x.f = data;
	x.i = swap32(x.i);
	*mem = x.f;
#endif
}

static inline void endianWriteFloat64Little(double* mem, double data)
{
#ifdef NEWAGE_BIG_ENDIAN
	union
	{
		u64 i;
		double d;
	} x;
	x.d = data;
	x.i = swap64(x.i);
	*mem = x.d;
#else
	*mem = data;
#endif
}

static inline void endianWriteFloat64Big(double* mem, double data)
{
#ifdef NEWAGE_BIG_ENDIAN
	*mem = data;
#else
	union
	{
		u64 i;
		double d;
	} x;
	x.d = data;
	x.i = swap64(x.i);
	*mem = x.d;
#endif
}

static inline void endianWriteFloatLittle(float* mem, float data)
{
	endianWriteFloat32Little(mem, data);
}

static inline void endianWriteFloatBig(float* mem, float data)
{
	endianWriteFloat32Big(mem, data);
}

static inline void endianWriteDoubleLittle(double* mem, double data)
{
	endianWriteFloat64Little(mem, data);
}

static inline void endianWriteDoubleBig(double* mem, double data)
{
	endianWriteFloat64Big(mem, data);
}


#endif
