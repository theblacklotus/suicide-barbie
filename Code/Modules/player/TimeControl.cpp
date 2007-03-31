#include "TimeControl.h"

using namespace mutalisk;

TimeControl::TimeControl()
:	currentTime(0)
,	isPaused(false)
,	discontinuity(0.f)
{
	resetKeys();
}

float TimeControl::update(float dt)
{
	if (!isPaused)
	{
		currentTime += dt;
		if(currentTime - dt < timeKey[1] && currentTime >= timeKey[1])
		{
			currentTime = timeKey[0];
			discontinuity -= timeKey[1] - timeKey[0];
		}
	}

	return time();
}

float TimeControl::getDiscontinuity()
{
	float v = discontinuity;
	discontinuity = 0.f;
	return v;
}

void TimeControl::restart(int complete)
{
	float v = currentTime;
	if(complete)
		currentTime = 0.0;
	else
		currentTime = timeKey[0];
	discontinuity += currentTime - v;

}

void TimeControl::scroll(float dt)
{
	discontinuity += dt;
	currentTime += dt;
	if(dt < 0.0f)
		currentTime = std::max<float>(currentTime, 0.0f);
}

void TimeControl::pause(int pause)
{
	isPaused = pause;
}

void TimeControl::resetKeys()
{ 
	from(0.0); to(-1.0f);
}

void TimeControl::from(float key)
{
	timeKey[0] = key;
}

void TimeControl::to(float key)
{
	timeKey[1] = key;
}
