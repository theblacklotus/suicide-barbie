#ifndef __TBL_INTRO_H
#define __TBL_INTRO_H

#include <pspsdk.h>

struct IntroRenderTarget
{
	int format;
	int mipmap;
	int width, height, stride;
	void* data;
	void* vramAddr;
};

bool loadIntro(const std::string& prefix);
SceUID startIntro(const IntroRenderTarget renderTarget[2]);
void unloadIntro();

#endif // __TBL_INTRO_H
