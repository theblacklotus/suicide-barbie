#include "../library/Lambert.h"

#include "dx9Platform.h"
#include "dx9CommonEffectImpl.h"

using namespace mutalisk;
using namespace mutalisk::effects;

//void Lambert::fillRequest(Request& request)
//{
//	request.required.matrices.push_back(BaseEffect::WorldMatrix);
//	request.required.matrices.push_back(BaseEffect::ViewMatrix);
//	request.required.matrices.push_back(BaseEffect::WorldViewProjMatrix);
//	request.optional.textures.push_back(BaseEffect::DiffuseTexture);
//	request.required.vecs.push_back(BaseEffect::AmbientColor);
//	request.required.vecs.push_back(BaseEffect::DiffuseColor);
//	request.required.vecs.push_back(BaseEffect::SpecularColor);
//	request.lightCountRange.first = 0;
//	request.lightCountRange.second = CommonEffectImpl::MAX_LIGHTS;
//}

struct Lambert::Impl : public CommonEffectImpl
{
	PassInfo							passInfo;
	LightsInPassesT						lightsInPasses;
	BaseEffect::Input::Lights const*	prevLights;

	Impl()
	{
		lightsInPasses.resize(4);
	}

	LightsInPassesT& processLights(BaseEffect::Input::Lights const& lights)
	{
		if(this->prevLights == &lights)
			return this->lightsInPasses;

		this->lightsInPasses.resize(0);
		organizeLightsInPasses(lights, this->lightsInPasses);

		this->prevLights = &lights;
		return this->lightsInPasses;
	}
};

Lambert::Lambert()
:	mImpl(new Impl())
{
}

Lambert::~Lambert()
{
}

void Lambert::begin()
{
	mImpl->passIndex = ~0;
	mImpl->begin("Main");
}

unsigned Lambert::passCount(Input const& i)
{
	unsigned lightPasses = static_cast<unsigned>(mImpl->processLights(i.lights).size());
	return max(1, lightPasses);
}

BaseEffect::PassInfo const& Lambert::passInfo(Input const& i, unsigned passIndex)
{
	return mImpl->passInfo;
}

void Lambert::pass(Input const& i, unsigned passIndex)
{
	unsigned fxPass = min(1, passIndex); 
	if(mImpl->passIndex != fxPass)
		mImpl->pass(fxPass);

	mImpl->setupLights(mImpl->processLights(i.lights)[passIndex]);
	mImpl->setupSurface(i);
	mImpl->setupGeometry(i);


	ASSERT(i.surface);
	mImpl->fx().SetFloat("fOpacity", 1.0f - i.surface->transparency);
	mImpl->fx().SetValue("vUvOffset", &D3DXVECTOR2(i.surface->uOffset, i.surface->vOffset), sizeof(D3DXVECTOR2));
	
	mImpl->commit();
}

void Lambert::captureState()
{
	mImpl->captureState();
}

void Lambert::end()
{
	mImpl->end();
	mImpl->restoreState();
}
