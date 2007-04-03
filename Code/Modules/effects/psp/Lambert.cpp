#include "../library/Lambert.h"

#include "pspPlatform.h"
#include "pspCommonEffectImpl.h"

using namespace mutalisk;
using namespace mutalisk::effects;

struct Lambert::Impl : public CommonEffectImpl
{
	PassInfo							passInfo;
	LightsInPassesT						lightsInPasses;
	BaseEffect::Input::Lights const*	prevLights;

	Impl() : prevLights(0)
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
	mImpl->begin();	// @NOTE: only difference in dx9/psp impl of Lambert
}

unsigned Lambert::passCount(Input const& i)
{
	unsigned lightPasses = static_cast<unsigned>(mImpl->processLights(i.lights).size());
	return std::max(1U, lightPasses);
}

BaseEffect::PassInfo const& Lambert::passInfo(Input const& i, unsigned passIndex)
{
	return mImpl->passInfo;
}

void Lambert::pass(Input const& i, unsigned passIndex)
{
	unsigned fxPass = std::min(1U, passIndex); 
	if(mImpl->passIndex != fxPass)
		mImpl->pass(fxPass);

	mImpl->setupLights(mImpl->processLights(i.lights)[passIndex], i);
	mImpl->setupSurface(i);
	mImpl->setupGeometry(i);
	mImpl->setupBuffers(i);
	
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
