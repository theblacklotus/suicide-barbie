#include "TimeBlock.h"

#include <psprtc.h>
#include <mutalisk/mutalisk.h>

namespace mutalisk
{

float gTickFrequency = -1.f;
float gTickResolution = 0.f;

void initTickFrequency()
{
	gTickResolution = sceRtcGetTickResolution();
	gTickFrequency = gTickResolution / (1000 * 1000);
	gTickFrequency = 1.0f / gTickFrequency;
}

float tickFrequency() { return gTickFrequency; }
float tickResolution() { return gTickResolution; }

TimeBlock::TimeBlock()
: currTick(0)
, prevTick(0)
{
}

void TimeBlock::peek()
{
	ASSERT(gTickFrequency >= 0.f);
	prevTick = currTick;
	sceRtcGetCurrentTick(&currTick);
}

float TimeBlock::ms()
{
	return static_cast<float>((currTick - prevTick) * gTickFrequency) / 1000.0f;
}

} //namespace mutalisk
