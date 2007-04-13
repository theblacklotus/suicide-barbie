#ifndef MUTALISK__TIMELINE_H_
#define MUTALISK__TIMELINE_H_

#include "cfg.h"
#include <vector>
#include <utility>
#include <math.h>

namespace mutalisk
{
	enum { FramesPerSecond = 30 };
	static unsigned timeToFrame(float t) { return static_cast<unsigned>(floor(t*FramesPerSecond)); }

	template <typename Context>
	class Timeline
	{
	public:
		typedef void (Context::*TimelineFuncT)();
		struct Item
		{
			enum nFlags { EachFrame = 0x00, Once = 0x01, AutoClear = 0x02,
				Default = EachFrame|AutoClear };

			unsigned startFrame;
			TimelineFuncT func;
			nFlags flags;

			Item(float sec = -1.f, int frame = 0, TimelineFuncT func_ = 0, nFlags flags_ = Default)
			: func(func_), flags(flags_) { startFrame = (sec >= 0)? (unsigned int)(sec*FramesPerSecond) + frame: ~0U; }
		};

	private:
		typedef std::vector<Item>							ScriptT;
		typedef std::vector<std::pair<ScriptT, unsigned> >	RunningScripts;

		RunningScripts	mScripts;

	public:
		void addScript(Item items[])
		{
			unsigned itemCount = 0;
			for(int q = 0; items[q].startFrame != ~0U; ++q)
				++itemCount;

			ASSERT(itemCount > 0);

			ScriptT newScript(itemCount);
			std::copy(items, items + itemCount, newScript.begin());
			mScripts.push_back(std::make_pair(newScript, -1));
		}

		void jump(Context& ctx, unsigned frame)
		{
			for(typename RunningScripts::iterator it = mScripts.begin(); it != mScripts.end(); ++it)
			{
				for(unsigned currScriptIt = 0; currScriptIt < it->first.size(); ++currScriptIt)
				{
					if(frame >= it->first[currScriptIt].startFrame)
						it->second = currScriptIt;
				}
			}
		}

		void update(Context& ctx, unsigned frame)
		{
			for(typename RunningScripts::iterator it = mScripts.begin(); it != mScripts.end(); ++it)
			{
				unsigned& currScriptIt = it->second;
				if(currScriptIt == it->first.size())
					continue;

				unsigned nextScriptIt = currScriptIt;
				if(currScriptIt == (unsigned)-1 || frame >= it->first[currScriptIt].startFrame)
				{
					++nextScriptIt;
					if(nextScriptIt < it->first.size() && frame >= it->first[nextScriptIt].startFrame)
					{
						currScriptIt = nextScriptIt;
						if (it->first[currScriptIt].flags & Item::Once)
						{
							printf("call once (next) with startFrame = %i\n", it->first[currScriptIt].startFrame);
							TimelineFuncT func = it->first[currScriptIt].func;
							if (func == 0)
								printf("func is 0!!\n");
							(ctx.*func)();
						}
					}
				}
				else
				{
					if(nextScriptIt > 0)
						--nextScriptIt;
					if(nextScriptIt != currScriptIt && frame < it->first[currScriptIt].startFrame)
					{
						currScriptIt = nextScriptIt;
						if (it->first[currScriptIt].flags & Item::Once)
						{
							printf("call once (previous) with startFrame = %i\n", it->first[currScriptIt].startFrame);
							TimelineFuncT func = it->first[currScriptIt].func;
							if (func == 0)
								printf("func is 0!!\n");
//							(ctx.*func)();
						}
					}
				}

				if (currScriptIt == (unsigned)-1 || (it->first[currScriptIt].flags & Item::Once))
					continue;

				TimelineFuncT func = it->first[currScriptIt].func;
//				ASSERT(func);
				if (func == 0)
					printf("func is 0!!\n");
				else
					(ctx.*func)();
			}
		}
	};

} // namespace mutalisk

#endif // MUTALISK__TIMELINE_H_
