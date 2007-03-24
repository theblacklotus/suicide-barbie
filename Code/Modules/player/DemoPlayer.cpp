#include "DemoPlayer.h"

#include "Timeline.h"

using namespace mutalisk;
namespace
{
	void splitFilename(std::string const& fullPath, std::string& path, std::string& fileName)
	{
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

void BaseDemoPlayer::platformSetup(IDirect3DDevice9& device, ID3DXEffect& defaultEffect)
{
	renderContext.device = &device;	
	renderContext.defaultEffect = &defaultEffect;
	D3DXMatrixIdentity(&renderContext.viewProjMatrix);
	D3DXMatrixIdentity(&renderContext.projMatrix);
}

BaseDemoPlayer::Scene BaseDemoPlayer::load(std::string const& sceneName)
{
	std::string path, fileName;
	splitFilename(sceneName, path, fileName);
	setResourcePath(path);

	Scene scene;
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
	::update(*scene.renderable, time() - scene.startTime);
	::process(*scene.renderable);
	::render(renderContext, *scene.renderable);
}

void BaseDemoPlayer::clear()
{
}

void BaseDemoPlayer::clearZ()
{
	DX_MSG("Depth clear") = 
		renderContext.device->Clear(0, NULL, D3DCLEAR_ZBUFFER, D3DXCOLOR(0.0f,0.0f,0.0f,0.0f), 1.0f, 0);
}

void BaseDemoPlayer::clearColor()
{
}