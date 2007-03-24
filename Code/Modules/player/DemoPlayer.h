#ifndef MUTALISK__DEMOPLAYER_H_
#define MUTALISK__DEMOPLAYER_H_

#include "cfg.h"
#include "platform.h"
#include "ScenePlayer.h"
#include "dx9/dx9ScenePlayer.h"
#include <mutalisk/mutalisk.h>

namespace mutalisk
{
	class BaseDemoPlayer
	{
	public:
		struct Scene
		{
			std::auto_ptr<mutalisk::data::scene>	blueprint;
			mutable Dx9RenderableScene*				renderable;
			mutable float							startTime;
		};

	public:
		// script interface
		void clear();
		void clearZ();
		void clearColor();

		Scene load(std::string const& sceneName);
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
		void platformSetup(IDirect3DDevice9& device, ID3DXEffect& defaultEffect);
		void start() { onStart(); }

	protected:
		virtual void onStart() = 0;

	protected:
		RenderContext	renderContext;
	private:
		float			mCurrTime;
		unsigned		mCurrFrame;
	};

} // namespace mutalisk

#endif // MUTALISK__DEMOPLAYER_H_
