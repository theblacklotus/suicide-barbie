#include "TimeControl.h"

using namespace mutalisk;

TimeControl::TimeControl()
:	currentTime(0)
{
	resetKeys();
}

float TimeControl::update(float dt)
{
	currentTime += dt;
	if(currentTime - dt < timeKey[1] && currentTime >= timeKey[1])
		currentTime = timeKey[0];

	return time();
}

void TimeControl::restart(int complete)
{
	if(complete)
		currentTime = 0.0;
	else
		currentTime = timeKey[0];
}

void TimeControl::scroll(float dt)
{
	currentTime += dt;
	if(dt < 0.0f)
		currentTime = std::max<float>(currentTime, 0.0f);
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
