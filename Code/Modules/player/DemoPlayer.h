#ifndef MUTALISK__DEMOPLAYER_H_
#define MUTALISK__DEMOPLAYER_H_

#include "cfg.h"
#include "platform.h"
#include <mutalisk/mutalisk.h>
#if defined(MUTALISK_PSP)
#include <pspkernel.h>
#include <list>
#include "ScenePlayer.h"
#include "psp/pspScenePlayer.h"
#endif

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
			std::string								pathPrefix;
			float									znear;
			float									zfar;
		};
		typedef void(*OnDrawT)(RenderableSceneT const&);

		struct PostProcessSettings {
			PostProcessSettings() : strength(0), threshold(0), srcModifier(0), dstModifier(255), quality(3) {}
			float strength;
			unsigned threshold;
			unsigned srcModifier;
			unsigned dstModifier;
			unsigned quality;
		};

	public:
		virtual ~BaseDemoPlayer() {}

		// script interface
		void clear();
		void clearZ();
		void clearColor();

		Scene const& load(Scene& scene, std::string const& sceneName);
		void draw(Scene const& scene, OnDrawT onDraw, float timeScale = 1.0f);
		void draw(Scene const& scene, float timeScale = 1.0f);
		void pause(Scene const& scene) {}
		void restart(Scene const& scene);
		float sceneTime(Scene const& scene);

		void ppBloom(float strength, unsigned threshold = 0, unsigned srcModifier = 0, unsigned dstModifier = 255, unsigned quality = 3);
		void blink() {}

		// getters
		unsigned frame() const { return mCurrFrame; }
		float time() const { return mCurrTime; }
		PostProcessSettings const& ppSettings() const { return mPPSettings; }

	public:
		// system interface
		enum nPhase { UpdatePhase, RenderPhase };
		void setPhase(nPhase phase);
		void setTime(float t);
		void setPath(std::string const& pathPrefix);
#if defined(MUTALISK_DX9)
		void platformSetup(IDirect3DDevice9& device, ID3DXEffect& defaultEffect);
#elif defined(MUTALISK_PSP)
		void platformSetup();
#endif
		void start() { onStart(); }
		void processJobQueue();

	protected:
		virtual void onStart() = 0;

	protected:
		RenderContextT	renderContext;
		float			mCurrTime;
		unsigned		mCurrFrame;
		std::string		mPathPrefix;
		PostProcessSettings
						mPPSettings;
		nPhase			mPhase;

	public:
		struct IJob
		{
			virtual ~IJob() {}
			virtual void process() = 0;
		};
		std::vector<IJob*>
						mJobQueue;
/*	private:
		struct Job
		{
			Scene const* scene;
			float time;
			OnDrawT onDraw;
		};
		std::vector<Job>
						mJobQueue;*/

#if defined(MUTALISK_PSP)					//	texture streaming
	public:
		BaseDemoPlayer()
		:	mPhase(UpdatePhase), m_currentLoad(0)
		{
		}
		void loadTextures(Scene& scene, bool async = true);
		void unloadTextures(Scene& scene);
		int updateTextures();
	private:
		typedef std::pair<std::string, RenderableScene::SharedResources::Texture*> QueueItem;
		std::list<QueueItem> m_texQueue;
		SceUID			m_currentLoad;
		data::MtxHeader*m_currentTexture;
		RenderableScene::SharedResources::Texture*		m_currentResource;
#endif

	};

} // namespace mutalisk

#endif // MUTALISK__DEMOPLAYER_H_
