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
		#define printf(...) do {} while(false)
		#define pspDebugScreenPrintf(...) do {} while(false)
		#define pspDebugScreenInit() do {} while(false)
		#define pspDebugScreenSetOffset(i) do {} while(false)
		#define pspDebugScreenSetXY(x, y) do {} while(false)
	#endif

#endif
