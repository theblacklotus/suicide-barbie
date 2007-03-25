#ifndef MUTALISK__DEMOPLAYER_H_
#define MUTALISK__DEMOPLAYER_H_

#include "cfg.h"
#include "platform.h"
#include <mutalisk/mutalisk.h>

namespace mutalisk
{
	class BaseDemoPlayer
	{
	public:
		struct Scene
		{
			std::auto_ptr<mutalisk::data::scene>	blueprint;
			mutable RenderableSceneT*				renderable;
			mutable float							startTime;
		};

	public:
		virtual ~BaseDemoPlayer() {}

		// script interface
		void clear();
		void clearZ();
		void clearColor();

		Scene const& load(Scene& scene, std::string const& sceneName);
		void draw(Scene const& scene);
		void pause(Scene const& scene) {}
		void restart(Scene const& scene) {}
		float sceneTime(Scene const& scene);

		void blink() {}

		// getters
		unsigned frame() const { return mCurrFrame; }
		float time() const { return mCurrTime; }

	public:
		// system interface
		void setTime(float t);
		void setPath(std::string const& pathPrefix);
#if defined(MUTALISK_DX9)
		void platformSetup(IDirect3DDevice9& device, ID3DXEffect& defaultEffect);
#elif defined(MUTALISK_PSP)
		void platformSetup();
#endif
		void start() { onStart(); }

	protected:
		virtual void onStart() = 0;

	protected:
		RenderContextT	renderContext;
	private:
		float			mCurrTime;
		unsigned		mCurrFrame;
		std::string		mPathPrefix;
	};

} // namespace mutalisk

#endif // MUTALISK__DEMOPLAYER_H_
