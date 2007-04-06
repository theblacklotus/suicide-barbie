#include <stdio.h>

namespace
{
	bool calcRange(float& value, float time, float attack, float decay, float release, float amplitude)
	{
		if (time >= attack && time < release)
		{
			float t;
			if (time < decay)
			{
				t = time - attack;
				t /= (decay - attack);
			}
			else
			{
				t = release - time;
				t /= (release - decay);
			}
			t *= amplitude;
			value = t;
			return true;
		}
		return false;
	}

}

#define BLINKY(attack, decay, release, amplitude) \
	{												\
		float value;								\
		if (calcRange(value, time, attack, decay, release, amplitude))		\
			return value;							\
	}

#define BLINKYUNIT(beat) \
	BLINKY(beat - 0.928f, beat, beat + 0.928, 1.f)


float getBlinkyValue(float time)
{
	BLINKYUNIT(23.045f);
	return 0.f;
}
