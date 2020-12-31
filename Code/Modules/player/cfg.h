#ifndef MUTALISK_PLAYER__CFG_H_
#define MUTALISK_PLAYER__CFG_H_

#include <mutalisk/common.h>

#if defined(WIN32)
	// disable:  unreferenced local function has been removed
	#pragma warning(disable : 4505)

	// disable:  assignment operator could not be generated
	#pragma warning(disable : 4512)

	// disable: forcing value to bool 'true' or 'false' (performance warning)
	#pragma warning(disable:4800)

	// disable:  name was marked as #pragma deprecated
	#pragma warning(disable : 4995)
#endif

#endif // MUTALISK_EFFECTS__CFG_H_
