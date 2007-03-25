#include "dxstdafx.h"
#include "TimeControl.h"

TimeControl::TimeControl()
:	currentTime(0)
{
	resetKeys();
}

void TimeControl::update(double dt)
{
	currentTime += dt;
	if(currentTime - dt < timeKey[1] && currentTime >= timeKey[1])
		currentTime = timeKey[0];
}

void TimeControl::restart(int complete)
{
	if(complete)
		currentTime = 0.0;
	else
		currentTime = timeKey[0];
}

void TimeControl::scroll(double dt)
{
	currentTime += dt;
	if(dt < 0.0f)
		currentTime = max(currentTime, 0);
}

void TimeControl::resetKeys()
{ 
	from(0.0); to(-1.0f);
}

void TimeControl::from(double key)
{
	timeKey[0] = key;
}

void TimeControl::to(double key)
{
	timeKey[1] = key;
}
