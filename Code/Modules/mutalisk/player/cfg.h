#ifndef __PLAYER_CFG_H
#define __PLAYER_CFG_H

#include "../cfg.h"

// disable:  unreferenced local function has been removed
#pragma warning(disable : 4505)

// disable:  assignment operator could not be generated
#pragma warning(disable : 4512)

// disable: forcing value to bool 'true' or 'false' (performance warning)
#pragma warning( disable:4800 )

// disable:  name was marked as #pragma deprecated
#pragma warning(disable : 4995)


// c1055: compiler limit : out of keys VC workaround
//#define __ERROR_C1055_WORKAROUND
#if defined __ERROR_C1055_WORKAROUND
	#include <cassert>
	#define ASSERT(exp) (void)( (exp) || (_assert(#exp, __FILE__, -1), 0) )
#else
	#include <cassert>
	#define ASSERT(exp) assert(exp)
#endif

#endif
