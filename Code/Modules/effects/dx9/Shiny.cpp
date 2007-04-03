#include "../library/Shiny.h"

#include "dx9Platform.h"
#include "dx9CommonEffectImpl.h"

using namespace mutalisk;
using namespace mutalisk::effects;

struct Shiny::Impl : public CommonEffectImpl
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

	void setupEnvironmentMap(BaseEffect::Input const& input)
	{
		ASSERT(input.surface);
		BaseEffect::Input::Surface const& surface = *input.surface;

		
		fx().SetValue("vLightAmbient", &D3DXCOLOR(1,1,1,1), sizeof(D3DXCOLOR));
		fx().SetValue("vMaterialAmbient", &surface.ambient, sizeof(surface.ambient));
		fx().SetValue("vMaterialDiffuse", &D3DXCOLOR(0,0,0,0), sizeof(D3DXCOLOR));
		fx().SetValue("vMaterialSpecular", &D3DXCOLOR(0,0,0,0), sizeof(D3DXCOLOR));
		fx().SetValue("vMaterialEmissive", &D3DXCOLOR(0,0,0,0), sizeof(D3DXCOLOR));

		IDirect3DBaseTexture9 const* envmapTexture = surface.envmapTexture;
		fx().SetBool("bDiffuseTextureEnabled", (envmapTexture != 0));
		if(envmapTexture)
		{
			fx().SetTexture("tDiffuse", const_cast<IDirect3DBaseTexture9*>(envmapTexture));
			fx().SetTexture("tEnvironment", const_cast<IDirect3DBaseTexture9*>(envmapTexture));
			fx().SetInt("nAddressU", D3DTADDRESS_CLAMP);//addressU);
			fx().SetInt("nAddressV", D3DTADDRESS_CLAMP);//addressV);
		}

		fx().SetFloat("fOpacity", 1.0f);
		fx().SetValue("vUvOffset", &D3DXVECTOR2(0.0f, 0.0f), sizeof(D3DXVECTOR2));

		fx().SetInt("nSrcBlend", D3DBLEND_ONE);
		fx().SetInt("nDestBlend", D3DBLEND_ONE);
		fx().SetBool("bAlphaBlendEnable", true);

		{
			static float sx = 0.5f;
			static float sy = 0.5f;
			static float sz = 1;
			static float oz = 1;
			static D3DXMATRIX viewMatrix = D3DXMATRIX(
				1,    0,   0,   0,
				0,    1,   0,   0,
				0,    0,   1,   0,
				0,    0,   1,   1 );

			D3DXMATRIX projMatrix;
			static float x = 2.75f;
			static float n = -0.5f;
			static float z = 0.5f;
			D3DXMatrixPerspectiveLH(&projMatrix, x, x, n, -100.0f);
			projMatrix._41 = 0.0f;
			projMatrix._42 = 0.0f;

			static D3DXMATRIX postMatrix = D3DXMATRIX(
				1,    0,   0,   0,
				0,    1,   0,   0,
				0,    0,   1,   0,
			  0.5,  0.5,   0,   1 );
			D3DXMATRIX textureProjMatrix;
			D3DXMatrixMultiply(&textureProjMatrix, &viewMatrix, &projMatrix);
			D3DXMatrixMultiply(&textureProjMatrix, &textureProjMatrix, &postMatrix);
			fx().SetMatrix("mTextureProjection", &textureProjMatrix);
			fx().SetInt("nTextureProjType", 2); // @TBD: texture_projNORMAL
		}
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
	mImpl->passIndex = ~0;
	mImpl->begin("Main");
}

unsigned Shiny::passCount(Input const& i)
{
	unsigned lightPasses = static_cast<unsigned>(mImpl->processLights(i.lights).size());
	return max(1, lightPasses) + 1;
}

BaseEffect::PassInfo const& Shiny::passInfo(Input const& i, unsigned passIndex)
{
	return mImpl->passInfo;
}

void Shiny::pass(Input const& i, unsigned passIndex)
{
	unsigned fxPass = min(1, passIndex); 
	if(mImpl->passIndex != fxPass)
		mImpl->pass(fxPass);

	Shiny::Impl::LightsInPassesT const& lightsInPasses = mImpl->processLights(i.lights);
	if(passIndex < lightsInPasses.size())
	{
		mImpl->setupLights(mImpl->processLights(i.lights)[passIndex]);
		mImpl->setupSurface(i);
	}
	else
	{
		mImpl->setupEnvironmentMap(i);
	}

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
}
