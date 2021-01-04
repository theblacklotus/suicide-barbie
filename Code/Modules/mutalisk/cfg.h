#ifndef MUTALISK_CFG_H_
#define MUTALISK_CFG_H_

// c4996: 'std::_Copy_opt' was declared deprecated
#if defined WIN32
	#pragma warning(disable:4996)
#endif

// c1055: compiler limit : out of keys VC workaround
//#define __ERROR_C1055_WORKAROUND
#if defined __ERROR_C1055_WORKAROUND
	#include <cassert>
	#define ASSERT(exp) (void)( (exp) || (_assert(#exp, __FILE__, -1), 0) )
#else
	#include <cassert>
	#include <cstdio>
#ifdef PSP_FINAL
#define ASSERT(exp)	{}
#elif defined(__psp__)
	#define ASSERT(exp) {if (!(exp)) { printf("ASSERT : %s\n", #exp); int* p = (int*)1; int i = *p; }}
#else
	#define ASSERT(exp) assert(exp)
#endif
#endif

#endif // MUTALISK_CFG_H_
