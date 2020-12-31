#if defined(__psp__)		// PSP specific global define overides...
	#define PSP_FINAL		// define to remove printf flooding etc
	#if defined(PSP_FINAL)

		#include <stdio.h>
		#ifdef __cplusplus
			#include <stdexcept>
		#endif
		#include <pspdebug.h>
		#include <pspkdebug.h>
		#define __STDIO_H__ 
		#define printf(...) do {} while(0)
		#define pspDebugScreenPrintf(...) do {} while(0)
		#define pspDebugScreenInit() do {} while(0)
		#define pspDebugScreenSetOffset(i) do {} while(0)
		#define pspDebugScreenSetXY(x, y) do {} while(0)
	#endif

#endif
