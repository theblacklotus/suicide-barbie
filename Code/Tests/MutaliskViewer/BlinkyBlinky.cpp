#include <stdio.h>
#include <pspgu.h>

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

#define BLINKYFRAME(frame, col) \
	{															\
		int diffFrame = frame - (int)(time * 60.f);				\
		if (diffFrame < 0)										\
			diffFrame = -diffFrame;								\
		if (diffFrame < 3)										\
		{														\
			color = col;										\
			return 1.f;											\
		}														\
	}


float getBlinkyValue(float time, unsigned& color)
{
	color = 0xffffffff;
	BLINKYFRAME(140, GU_ARGB(0xff, 0xd2, 0x28, 0x78));
	BLINKYFRAME(396, GU_ARGB(0xff, 0xde, 0x8c, 0x00));
	BLINKYFRAME(682, GU_ARGB(0xff, 0xda, 0x5a, 0x25));
	BLINKYUNIT(23.045f);
	BLINKYUNIT(61.466f);
	BLINKYUNIT(92.191f);
	BLINKYUNIT(154.13f);
	BLINKYUNIT(184.34f);

	BLINKY(115.22f, 115.22f, 115.22f+ 0.928f, 1.0f);
	BLINKY(122.90f, 122.90f, 122.90f+ 0.928f, 1.0f);
	BLINKY(245.94f, 245.94f, 245.94f+ 0.928f, 1.0f);
	BLINKY(276.64f, 276.64f, 276.64f+ 0.928f, 1.0f);

	BLINKY(554.00f, 557.00f, 10000, -1.0f);
	return 0.f;
}
