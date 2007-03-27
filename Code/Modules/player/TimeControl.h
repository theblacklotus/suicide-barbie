#ifndef MUTALISK__TIME_CONTROL_H_
#define MUTALISK__TIME_CONTROL_H_

#include "cfg.h"

namespace mutalisk
{
	class TimeControl
	{
	public:
		TimeControl();

		float update(float dt);
		void restart(int complete);
		void scroll(float dt);
		
		void resetKeys();
		void from(float key);
		void to(float key);

		float time() const { return static_cast<float>(currentTime); }

	private:
		float currentTime;
		float timeKey[2];
	};
} // namespace mutalisk

#endif // MUTALISK__TIME_CONTROL_H_
