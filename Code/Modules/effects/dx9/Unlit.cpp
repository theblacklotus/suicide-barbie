#include "../library/Unlit.h"

#include "dx9Platform.h"
#include "dx9CommonEffectImpl.h"

using namespace mutalisk;
using namespace mutalisk::effects;

struct Unlit::Impl : public CommonEffectImpl
{
	PassInfo	passInfo;

	void setupAmbientOnly()
	{
		D3DXVECTOR4 ambient = D3DXVECTOR4(1,1,1,1);
		fx().SetValue("vLightAmbient", &ambient, sizeof(D3DXVECTOR4));
		fx().SetValue("vMaterialAmbient", &ambient, sizeof(D3DXVECTOR4));
		fx().SetInt("iNumLights", 0);
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
	mImpl->passIndex = ~0;
	mImpl->begin("Main");
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
	unsigned fxPass = min(1, passIndex); 
	if(mImpl->passIndex != fxPass)
		mImpl->pass(fxPass);

	mImpl->setupSurface(i);
	mImpl->setupGeometry(i);
	mImpl->setupBuffers(i);
	mImpl->setupAmbientOnly();
	
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
