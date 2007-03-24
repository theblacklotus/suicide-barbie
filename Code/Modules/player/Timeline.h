#ifndef MUTALISK__TIMELINE_H_
#define MUTALISK__TIMELINE_H_

#include "cfg.h"

namespace mutalisk
{
	enum { FramesPerSecond = 30 };
	static unsigned timeToFrame(float t) { return static_cast<unsigned>(floor(t*FramesPerSecond)); }

	template <typename Context>
	class Timeline
	{
	public:
		typename typedef void (Context::*TimelineFuncT)();
		struct Item
		{
			enum nFlags { EachFrame = 0x00, Once = 0x01, AutoClear = 0x02,
				Default = EachFrame|AutoClear };

			unsigned startFrame;
			TimelineFuncT func;
			nFlags flags;

			Item(unsigned sec = 0, int frame = -1, TimelineFuncT func_ = 0, nFlags flags_ = Default)
			: func(func_), flags(flags_) { startFrame = (frame >= 0)? sec*FramesPerSecond + frame: ~0U; }
		};

		void addScript(Item items[])
		{
			unsigned itemCount = 0;
			for(int q = 0; items[q].startFrame != ~0U; ++q)
				++itemCount;

			ASSERT(itemCount > 0);

			ScriptT newScript(itemCount);
			std::copy(items, items + itemCount, newScript.begin());
			mScripts.push_back(std::make_pair(newScript, 0));
		}

		void update(Context& ctx, unsigned frame)
		{
			for(RunningScripts::iterator it = mScripts.begin(); it != mScripts.end(); ++it)
			{
				unsigned& currScriptIt = it->second;
				if(currScriptIt == it->first.size())
					continue;

				unsigned nextScriptIt = currScriptIt;
				if(frame >= it->first[currScriptIt].startFrame)
				{
					++nextScriptIt;
					if(nextScriptIt < it->first.size() && frame >= it->first[nextScriptIt].startFrame)
						currScriptIt = nextScriptIt;
				}
				else
				{
					if(nextScriptIt > 0)
						--nextScriptIt;
					if(frame < it->first[currScriptIt].startFrame)
						currScriptIt = nextScriptIt;
				}

				TimelineFuncT func = it->first[currScriptIt].func;
				ASSERT(func);
				(ctx.*func)();
			}
		}

	private:
		typedef std::vector<Item>							ScriptT;
		typedef std::vector<std::pair<ScriptT, unsigned> >	RunningScripts;

		RunningScripts	mScripts;
	};

} // namespace mutalisk

#endif // MUTALISK__TIMELINE_H_
