#include "../library/Unlit.h"

#include "pspPlatform.h"
#include "pspCommonEffectImpl.h"

using namespace mutalisk;
using namespace mutalisk::effects;

struct Unlit::Impl : public CommonEffectImpl
{
	PassInfo	passInfo;

	void setupAmbientOnly()
	{
//		D3DXVECTOR4 ambient = D3DXVECTOR4(1,1,1,1);
//		fx().SetValue("vLightAmbient", &ambient, sizeof(D3DXVECTOR4));
//		fx().SetValue("vMaterialAmbient", &ambient, sizeof(D3DXVECTOR4));
//		fx().SetInt("iNumLights", 0);

		sceGuAmbient(~0U);
		sceGuAmbientColor(~0U);
		sceGuDisable(GU_LIGHTING);
	}
};

Unlit::Unlit()
:	mImpl(new Impl())
{
}

Unlit::~Unlit()
{
}

void Unlit::begin()
{
	mImpl->begin();
}

unsigned Unlit::passCount(Input const& i)
{
	return 1;
}

BaseEffect::PassInfo const& Unlit::passInfo(Input const& i, unsigned passIndex)
{
	return mImpl->passInfo;
}

void Unlit::pass(Input const& i, unsigned passIndex)
{
	unsigned fxPass = std::min(1U, passIndex); 
	if(mImpl->passIndex != fxPass)
		mImpl->pass(fxPass);

	mImpl->setupSurface(i);
	mImpl->setupGeometry(i);
	mImpl->setupBuffers(i);
	mImpl->setupAmbientOnly();

//	sceGuAmbient(0x00ffffff);
//	sceGuColor(i.surface->emissive);
	sceGuSendCommandi(88, (unsigned int)((1.0f-i.surface->transparency) * 255.0f));

	mImpl->commit();
}

void Unlit::captureState()
{
	mImpl->captureState();
}

void Unlit::end()
{
	mImpl->end();
	mImpl->restoreState();
}
