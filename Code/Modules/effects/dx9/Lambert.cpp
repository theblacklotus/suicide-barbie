#include "../library/Lambert.h"

#include "dx9Platform.h"
#include "dx9CommonEffectImpl.h"

using namespace mutalisk;
using namespace mutalisk::effects;

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
