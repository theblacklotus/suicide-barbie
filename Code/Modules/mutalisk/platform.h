#ifndef MUTALISK_DATA_PLATFORM_H_
#define MUTALISK_DATA_PLATFORM_H_

#include "cfg.h"

#if defined WIN32
#	include "dx9/dx9Platform.h"
#elif defined __psp__
#	include "psp/pspPlatform.h"
#else
	MUTALISK_NOT_IMPLEMENTED("Platform not supported");
#endif

#endif // MUTALISK_DATA_PLATFORM_H_
