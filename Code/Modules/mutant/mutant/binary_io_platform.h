#ifndef MUTANT_BINARY_IO_PLATFORM_H_
#define MUTANT_BINARY_IO_PLATFORM_H_

#include "cfg.h"

#if defined __psp__
	#include "binary_io_psp.h"
#else
	#include "binary_io_win32.h"
#endif

#endif // MUTANT_BINARY_IO_PSP_H_
