#include "../library/Shiny.h"

#include "dx9Platform.h"
#include "dx9CommonEffectImpl.h"

using namespace mutalisk;
using namespace mutalisk::effects;

void Shiny::fillRequest(Request& request)
{
	request.required.matrices.push_back(BaseEffect::WorldMatrix);
	request.required.matrices.push_back(BaseEffect::ViewMatrix);
	request.required.matrices.push_back(BaseEffect::WorldViewProjMatrix);
	request./*required*/optional.textures.push_back(BaseEffect::EnvmapTexture);
	request.optional.textures.push_back(BaseEffect::DiffuseTexture);
//	request.required.vecs.push_back(BaseEffect::AmbientColor);
//	request.required.vecs.push_back(BaseEffect::DiffuseColor);
//	request.required.vecs.push_back(BaseEffect::SpecularColor);
	request.lightCountRange.first = 0;
	request.lightCountRange.second = 4;
}

struct Shiny::Impl : public CommonEffectImpl
{
	void setupSurfacePass()
	{
		fx().SetValue("vLightAmbient", D3DXVECTOR4(1,1,1,1), sizeof(D3DXVECTOR4));
		fx().SetInt("iNumLights", 0);
	}

	void setupEnvmapPass(BaseEffect::Input const& input)
	{
		IDirect3DBaseTexture9 const* envmapTexture = input.textures[BaseEffect::EnvmapTexture];
//		IDirect3DBaseTexture9 const* envmapTexture = input.textures[BaseEffect::DiffuseTexture];
//		ASSERT(envmapTexture);
		fx().SetBool("bDiffuseTextureEnabled", true);
		fx().SetTexture("tDiffuse", const_cast<IDirect3DBaseTexture9*>(envmapTexture));
		fx().SetInt("nAddressU", D3DTADDRESS_CLAMP);
		fx().SetInt("nAddressV", D3DTADDRESS_CLAMP);
//		fx().SetInt("nAddressU", D3DTADDRESS_WRAP);
//		fx().SetInt("nAddressV", D3DTADDRESS_WRAP);



//		float angle = -2.0f * valEnvMap * (GU_PI/180.0f);
//		float cs = cosf(angle);
//		float sn = sinf(angle);
		static float sx = 0.5f;
		static float sy = 0.5f;
		static float sz = 1;
		static float oz = 1;
		static D3DXMATRIX viewMatrix = D3DXMATRIX(
/*			0.5,   0,   0, 0,
			0,   0.5,  0, 0,
			0,    0,   1, 1,
			.5,  .5,  1, 1 );
*/			
/*			0.5,   0,   0, 0,
			0,   0.5,  0, 0,
			0,    0,   0, 0,
			.5,  .5,  0, 1 );*/
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
//		projMatrix._43 += z;

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

	enum { MAX_PASSES = 1 };
	PassInfo	passInfo[MAX_PASSES];
};

Shiny::Shiny()
:	mImpl(new Impl())
{
	fillRequest(mRequest);
	allocInput(mInput, mRequest);
}

Shiny::~Shiny()
{
}

unsigned Shiny::begin()
{
	mImpl->passIndex = ~0;
	mImpl->begin("Main");
	return Impl::MAX_PASSES;
}

BaseEffect::PassInfo const& Shiny::passInfo(unsigned passIndex)
{
	return mImpl->passInfo[passIndex];
}

void Shiny::pass(unsigned passIndex)
{
	ASSERT(validateInput(mInput, mRequest));
	bool resetPass = (mImpl->passIndex != passIndex);
	switch(passIndex)
	{
	case 0:
		if(resetPass)
			mImpl->pass(0);
		mImpl->setupSurfacePass();
		mImpl->setupEnvmapPass(mInput);
		mImpl->setupSurface(mInput);
		mImpl->setupGeometry(mInput);
		break;
/*	case 1:
		if(resetPass)
			mImpl->pass(1);
		mImpl->setupEnvmapPass(mInput);
//		mImpl->setupSurfacePass();
		mImpl->setupSurface(mInput);
		mImpl->setupGeometry(mInput);
		break;*/
	default:
		ASSERT("Invalid pass index");
	}
	mImpl->commit();
	mImpl->passIndex = passIndex;
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
