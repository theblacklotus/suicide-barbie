
#ifndef NEWAGE_BASE_COMMON_TYPES_H
#define NEWAGE_BASE_COMMON_TYPES_H

#if defined(__psp__)
#	include <psptypes.h>
#	define __use_system_types
#endif

typedef unsigned char bool8;
typedef unsigned short bool16;
typedef unsigned int bool32;

typedef unsigned int uint;

#if !defined(__use_system_types)
typedef char s8;
typedef short s16;
typedef int s32;
typedef long long s64;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
#endif

#ifndef NULL
#define NULL ((void*) 0)
#endif


#ifndef bool
#ifndef __cplusplus
	typedef s32 bool;
#else
	#define bool s32
#endif
#endif


#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif


#endif

