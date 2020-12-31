#ifndef MUTALISK_PLAYER__PLATFORM_H_
#define MUTALISK_PLAYER__PLATFORM_H_

#include "cfg.h"
#include <mutalisk/platform.h>

#if defined WIN32
#	include "dx9/dx9Platform.h"
#elif defined __psp__
#	include "psp/pspPlatform.h"
#else
	MUTALISK_NOT_IMPLEMENTED("Platform not supported");
#endif

#endif // MUTALISK_PLAYER__PLATFORM_H_
