#ifndef MUTALISK_CFG_H_
#define MUTALISK_CFG_H_

// c1055: compiler limit : out of keys VC workaround
//#define __ERROR_C1055_WORKAROUND
#if defined __ERROR_C1055_WORKAROUND
	#include <cassert>
	#define ASSERT(exp) (void)( (exp) || (_assert(#exp, __FILE__, -1), 0) )
#else
	#include <cassert>
	#define ASSERT(exp) assert(exp)
#endif

#endif // MUTALISK_CFG_H_
