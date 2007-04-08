#include "DemoPlayer.h"

#include "Timeline.h"
#include "ScenePlayer.h"
#if defined(MUTALISK_DX9)
#	include "dx9/dx9ScenePlayer.h"
#elif defined(MUTALISK_PSP)
#	include "psp/pspScenePlayer.h"
#endif

using namespace mutalisk;
namespace
{
	void splitFilename(std::string const& fullPath, std::string& path, std::string& fileName)
	{
		using namespace std;
		size_t offset0 = fullPath.find_last_of('/');
		size_t offset1 = fullPath.find_last_of('\\');

		size_t offset = max(offset0, offset1);
		if(offset == std::string::npos)
			offset = min(offset0, offset1);

		path = "";
		fileName = fullPath;
		if(offset == std::string::npos)
			return;

		++offset;
		path = fullPath.substr(0, offset);
		fileName = fullPath.substr(offset);
	}
}

#if defined(MUTALISK_DX9)
void BaseDemoPlayer::platformSetup(IDirect3DDevice9& device, ID3DXEffect& defaultEffect)
{
	renderContext.device = &device;	
	renderContext.defaultEffect = &defaultEffect;
	D3DXMatrixIdentity(&renderContext.viewProjMatrix);
	D3DXMatrixIdentity(&renderContext.projMatrix);
}
#elif defined(MUTALISK_PSP)
void BaseDemoPlayer::platformSetup()
{
	ScePspFMatrix4 identityMatrix;
	gumLoadIdentity(&identityMatrix);

	renderContext.viewProjMatrix = identityMatrix;
	renderContext.projMatrix = identityMatrix;
}
#endif

BaseDemoPlayer::Scene const& BaseDemoPlayer::load(Scene& scene, std::string const& sceneName)
{
	std::string path, fileName;
	splitFilename(sceneName, path, fileName);
	scene.pathPrefix = path;
	setResourcePath(mPathPrefix + path);

	scene.blueprint = loadResource<mutalisk::data::scene>(fileName);
	scene.renderable = prepare(renderContext, *scene.blueprint).release();
	scene.startTime = -1.0f;
	scene.znear = 1.0f;
	scene.zfar = 50.0f;
	return scene;
}

void BaseDemoPlayer::restart(Scene const& scene)
{
	scene.startTime = -1.0f;
}
/*
void BaseDemoPlayer::processJobQueue()
{
	for(size_t q = 0; q < mJobQueue.size(); ++q)
	{
		ASSERT(mJobQueue[q].scene->renderable);
	#if defined(MUTALISK_DX9)
		mutalisk::update(*mJobQueue[q].scene->renderable, mJobQueue[q].time);
		mutalisk::process(*mJobQueue[q].scene->renderable);
	#elif defined(MUTALISK_PSP)
		mJobQueue[q].scene->renderable->update(mJobQueue[q].time);
		mJobQueue[q].scene->renderable->process();
	#endif
		mJobQueue[q].onDraw(*mJobQueue[q].scene->renderable);
	}
	mJobQueue.resize(0);
}*/

void BaseDemoPlayer::processJobQueue()
{
	for(size_t q = 0; q < mJobQueue.size(); ++q)
	{
		ASSERT(mJobQueue[q]);
		mJobQueue[q]->process();
		delete mJobQueue[q];
	}
	mJobQueue.resize(0);
}

#if defined(MUTALISK_PSP)
void BaseDemoPlayer::loadTextures(Scene& scene, bool async)
{
	setResourcePath(mPathPrefix + scene.pathPrefix);
	array<mutalisk::data::scene::Ref>& textureIds = scene.blueprint->textureIds;
	array<RenderableScene::SharedResources::Texture>& textures = scene.renderable->mResources.textures;
	textures.resize(textureIds.size());
	for(size_t q = 0; q < textureIds.size(); ++q)
	{
		if (async)
		{
			RenderableScene::SharedResources::Texture* tex = &textures[q];
			QueueItem item(mPathPrefix + scene.pathPrefix + textureIds[q], tex);
			m_texQueue.push_back(item);
		}
		else
		{
			textures[q].blueprint = loadResource<mutalisk::data::texture>(textureIds[q]);
			textures[q].renderable = prepare(renderContext, *textures[q].blueprint);
		}
	}
}


void BaseDemoPlayer::unloadTextures(Scene& scene)
{
	array<RenderableScene::SharedResources::Texture>& textures = scene.renderable->mResources.textures;
	textures.resize(0);
}

int BaseDemoPlayer::updateTextures()
{
	// if texture load in flight; check to see if done
	if (m_currentLoad)
	{
		SceIores res;
		int ret = sceIoPollAsync(m_currentLoad, &res);
		if (ret == 1)
		{
//			printf("sceIoPollAsync returned %x ; still loading\n", ret);
			return m_texQueue.size();
		}
		else if(ret == 0)
		{
//			printf("sceIoPollAsync returned %x ; loading done with res = %i\n", ret, res);
		}
		else
		{
			printf("sceIoPollAsync returned %x with res = %x\n", ret, (unsigned)res);
			m_currentLoad = 0;
			return m_texQueue.size();
		}
		sceIoClose(m_currentLoad);
		m_currentLoad = 0;
		std::auto_ptr<data::texture> resource(new mutalisk::data::texture);
		resource->patchupTextureFromMemory(m_currentTexture);

		m_currentResource->blueprint = resource;
		m_currentResource->renderable = prepare(renderContext, *m_currentResource->blueprint);
		return m_texQueue.size();
	}

	// if no texture load in flight; check to see if have any queued up
	if (!m_currentLoad && !m_texQueue.empty())
	{
		QueueItem& item = m_texQueue.front();
		std::string& name = item.first;
		m_currentResource = item.second;	

//		printf("trying to open file %s\n", name.c_str());
		m_currentLoad = sceIoOpen(name.c_str(), PSP_O_RDONLY, 0777);
		if (m_currentLoad>=0)
		{
//			printf("successfully opened file\n");
			int size = sceIoLseek(m_currentLoad, 0, PSP_SEEK_END);
			sceIoLseek(m_currentLoad, 0, PSP_SEEK_SET);
//			printf("file size = %i\n", size);
			m_currentTexture = (data::MtxHeader*)malloc(size + sizeof(data::MtxHeader));
//			printf("mem ptr = %x\n", (unsigned)m_currentTexture);
			if (sceIoChangeAsyncPriority(m_currentLoad, 0x8))
			{
				printf("sceIoChangeAsyncPriority failed\n");
			}
			int ret = sceIoReadAsync(m_currentLoad, m_currentTexture, size);
//			printf("sceIoReadAsync returned = %x\n", ret);
		}
		else
		{
			printf("unable to find file %s\n", name.c_str());
		}

		m_texQueue.pop_front();
	}
	return m_texQueue.size();	
}
#endif

void BaseDemoPlayer::setTime(float t)
{
	mCurrTime = t;
	mCurrFrame = timeToFrame(t);
}

void BaseDemoPlayer::setPath(std::string const& pathPrefix)
{
	mPathPrefix = pathPrefix;
}

 float BaseDemoPlayer::sceneTime(Scene const& scene)
{
	ASSERT(scene.startTime >= 0.0f);
	return time() - scene.startTime;
}

namespace {
void onDrawDefault(RenderableSceneT const& scene) {}
}
void BaseDemoPlayer::draw(Scene const& scene, float timeScale)
{
	draw(scene, &::onDrawDefault, timeScale);
}

struct RenderJob : public BaseDemoPlayer::IJob
{
	BaseDemoPlayer::Scene const*	scene;
	RenderContextT*					renderContext;
	void process()
	{
		renderContext->znear = scene->znear;
		renderContext->zfar = scene->zfar;
		mutalisk::render(*renderContext, *scene->renderable);
	}
};

void BaseDemoPlayer::draw(Scene const& scene, OnDrawT onDraw, float timeScale)
{
	if(scene.startTime <= 0.0f)
		scene.startTime = time();

	ASSERT(scene.renderable);
#if defined(MUTALISK_DX9)
	mutalisk::update(*scene.renderable, (time() - scene.startTime) * timeScale);
	mutalisk::process(*scene.renderable);
#elif defined(MUTALISK_PSP)
	scene.renderable->update((time() - scene.startTime) * timeScale);
	scene.renderable->process();
#endif
	//renderContext.znear = scene.znear;
	//renderContext.zfar = scene.zfar;

	onDraw(*scene.renderable);
//	mutalisk::render(renderContext, *scene.renderable);
	RenderJob* job = new RenderJob;
	job->scene = &scene;
	job->renderContext = &renderContext;
	mJobQueue.push_back(job);
}
/*
void BaseDemoPlayer::draw(Scene const& scene, OnDrawT onDraw)
{
	if(scene.startTime <= 0.0f)
	{
		scene.startTime = time();

		scene.renderable->update(0.0f);
		scene.renderable->process();
		onDraw(*scene.renderable);
	}
//	else
	{
		renderContext.znear = scene.znear;
		renderContext.zfar = scene.zfar;
		mutalisk::render(renderContext, *scene.renderable);
	}

	Job job;
	job.scene = &scene;
	job.time = time() - scene.startTime;
	job.onDraw = onDraw;
	mJobQueue.push_back(job);
}*/

void BaseDemoPlayer::clear()
{
}

struct ClearZJob : public BaseDemoPlayer::IJob
{
	RenderContextT*					renderContext;
	void process()
	{
	#if defined(MUTALISK_DX9)
		ASSERT(renderContext);
		DX_MSG("Depth clear") = 
			renderContext->device->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DXCOLOR(0.0f,0.0f,0.0f,0.0f), 1.0f, 0);
	#elif defined(MUTALISK_PSP)
		sceGuClearDepth(0xffff);
		sceGuClear(GU_DEPTH_BUFFER_BIT);
	#endif
	}
};

void BaseDemoPlayer::clearZ()
{
	ClearZJob* job = new ClearZJob;
	job->renderContext = &renderContext;
	mJobQueue.push_back(job);
}

void BaseDemoPlayer::clearColor()
{
}

struct BloomJob : public BaseDemoPlayer::IJob
{
	BaseDemoPlayer::PostProcessSettings* dstSettings;
	BaseDemoPlayer::PostProcessSettings srcSettings;
	void process()
	{
		ASSERT(dstSettings);
		*dstSettings = srcSettings;
	}
};
void BaseDemoPlayer::ppBloom(float strength, unsigned threshold, unsigned srcModifier, unsigned dstModifier)
{
	BloomJob* job = new BloomJob;
	job->dstSettings = &mPPSettings;
	job->srcSettings.strength = strength;
	job->srcSettings.threshold = threshold;
	job->srcSettings.srcModifier = srcModifier;
	job->srcSettings.dstModifier = dstModifier;
	mJobQueue.push_back(job);
}
