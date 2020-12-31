#include "../library/Shiny.h"

#include "pspPlatform.h"
#include "pspCommonEffectImpl.h"

using namespace mutalisk;
using namespace mutalisk::effects;

struct Shiny::Impl : public CommonEffectImpl
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

	void setupEnvironmentMap(BaseEffect::Input const& input)
	{
		ASSERT(input.surface);
		BaseEffect::Input::Surface const& surface = *input.surface;

		mutalisk::data::psp_texture const* envmapTexture = surface.envmapTexture;
		if(envmapTexture)
		{
			mutalisk::data::psp_texture const& texture = *envmapTexture;
			if(texture.clutEntries)
			{
				sceGuClutMode(texture.clutFormat,0,0xff,0);
				sceGuClutLoad(texture.clutEntries,texture.clut);
			}
			sceGuTexMode(texture.format,0,0,texture.swizzled);
			sceGuTexImage(texture.mipmap,texture.width,texture.height,texture.stride,texture.data);
			sceGuTexWrap(GU_CLAMP, GU_CLAMP);
			sceGuTexFilter(GU_LINEAR_MIPMAP_NEAREST, GU_LINEAR_MIPMAP_NEAREST);
			sceGuTexFunc(GU_TFX_MODULATE,GU_TCC_RGBA);
			sceGuEnable(GU_TEXTURE_2D);
		}
		else
			sceGuDisable(GU_TEXTURE_2D);

		sceGuBlendFunc(GU_ADD, GU_FIX, GU_FIX, 0xffffffff, 0xffffffff);
		sceGuEnable(GU_BLEND);
		
		ScePspFMatrix4 viewMatrix = input.matrices[BaseEffect::ViewMatrix];
		ScePspFMatrix4 m;
		gumFullInverse(&m, &viewMatrix);

		// setup envmap matrix (2x3)
		ScePspFVector3 envmapMatrixColumns[2] = {
			{ m.x.x, -m.x.y, m.x.z },
			{ m.y.x, -m.y.y, m.y.z }
		};
		sceGuLight( 2, GU_DIRECTIONAL, GU_DIFFUSE, &envmapMatrixColumns[0] );
		sceGuLight( 3, GU_DIRECTIONAL, GU_DIFFUSE, &envmapMatrixColumns[1] );

		// setup envmap texture coord generation
		sceGuTexMapMode(
			GU_ENVIRONMENT_MAP,	// envmap mode on
			2,	// use 2nd light position as an envmap matrix 1st column
			3	// use 3rd light position as an envmap matrix 2nd column
			);

		sceGuAmbientColor(surface.ambient);
		sceGuDisable(GU_LIGHTING);
	}
};

Shiny::Shiny()
:	mImpl(new Impl())
{
}

Shiny::~Shiny()
{
}

void Shiny::begin()
{
	mImpl->begin();
}

unsigned Shiny::passCount(Input const& i)
{
	unsigned lightPasses = static_cast<unsigned>(mImpl->processLights(i.lights).size());
	return std::max(1U, lightPasses) + 1;
}

BaseEffect::PassInfo const& Shiny::passInfo(Input const& i, unsigned passIndex)
{
	return mImpl->passInfo;
}

void Shiny::pass(Input const& i, unsigned passIndex)
{
	unsigned fxPass = std::min(1U, passIndex); 
	if(mImpl->passIndex != fxPass)
		mImpl->pass(fxPass);

	Impl::LightsInPassesT const& lightsInPasses = mImpl->processLights(i.lights);
	if(passIndex < lightsInPasses.size())
	{
		mImpl->setupLights(mImpl->processLights(i.lights)[passIndex], i);
		mImpl->setupSurface(i);
		sceGuTexMapMode(GU_TEXTURE_COORDS, 0, 0);
	}
	else
		mImpl->setupEnvironmentMap(i);
	mImpl->setupGeometry(i);
	mImpl->setupBuffers(i);
	
	mImpl->commit();
}

void Shiny::captureState()
{
	mImpl->captureState();
}

void Shiny::end()
{
	mImpl->end();
	mImpl->restoreState();
	sceGuTexMapMode(GU_TEXTURE_COORDS, 0, 0);
}
