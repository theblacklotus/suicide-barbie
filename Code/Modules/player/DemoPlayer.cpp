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
	setResourcePath(mPathPrefix + path);

	scene.blueprint = loadResource<mutalisk::data::scene>(fileName);
	scene.renderable = prepare(renderContext, *scene.blueprint).release();
	scene.startTime = -1.0f;
	return scene;
}

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

void BaseDemoPlayer::draw(Scene const& scene)
{
	if(scene.startTime <= 0.0f)
		scene.startTime = time();

	ASSERT(scene.renderable);
#if defined(MUTALISK_DX9)
	mutalisk::update(*scene.renderable, time() - scene.startTime);
	mutalisk::process(*scene.renderable);
#elif defined(MUTALISK_PSP)
	scene.renderable->update(time() - scene.startTime);
	scene.renderable->process();
#endif
	mutalisk::render(renderContext, *scene.renderable);
}

void BaseDemoPlayer::clear()
{
}

void BaseDemoPlayer::clearZ()
{
#if defined(MUTALISK_DX9)
	DX_MSG("Depth clear") = 
		renderContext.device->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DXCOLOR(0.0f,0.0f,0.0f,0.0f), 1.0f, 0);
#elif defined(MUTALISK_PSP)
	sceGuClearDepth(0xffff);
	sceGuClear(GU_DEPTH_BUFFER_BIT);
#endif
}

void BaseDemoPlayer::clearColor()
{
}
