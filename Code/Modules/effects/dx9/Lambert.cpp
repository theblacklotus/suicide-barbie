#include "../library/Lambert.h"

#include "dx9Platform.h"

using namespace mutalisk;
using namespace mutalisk::effects;

void Lambert::fillRequest(Request& request)
{
	request.required.matrices.push_back(BaseEffect::WorldMatrix);
	request.required.matrices.push_back(BaseEffect::ViewMatrix);
	request.required.matrices.push_back(BaseEffect::WorldViewProjMatrix);
	request.optional.textures.push_back(BaseEffect::DiffuseTexture);
	request.required.vecs.push_back(BaseEffect::AmbientColor);
	request.required.vecs.push_back(BaseEffect::DiffuseColor);
	request.required.vecs.push_back(BaseEffect::SpecularColor);
	request.lightCountRange.first = 0;
	request.lightCountRange.second = MAX_LIGHTS;
}

struct Lambert::Impl
{
	void setupLights(Input const& input)
	{
		int vLightType[MAX_LIGHTS];							// Light's position in world space
		D3DXVECTOR3 vLightPos[MAX_LIGHTS];					// Light's position in world space
		D3DXVECTOR3 vLightDir[MAX_LIGHTS];					// Light's direction in world space
		D3DXVECTOR3 vLightAttenuation[MAX_LIGHTS];			// Light's attenuations {attn0, attn1, attn2}
		float  fLightSpotCutoff[MAX_LIGHTS];				// Light's spot cutoff
		float  fLightSpotExp[MAX_LIGHTS];					// Light's spot exponent
		D3DXVECTOR4 vLightDiffuse[MAX_LIGHTS];				// Light's diffuse color
		D3DXVECTOR4 vLightSpecular[MAX_LIGHTS];				// Light's specular color
		D3DXVECTOR4 vLightAmbient = D3DXVECTOR4(0,0,0,0);	// Light's ambient color

		for(unsigned q = 0; q < input.lights.size(); ++q)
		{
			ASSERT(q < MAX_LIGHTS);
			if(!input.lights[q].first)
				continue;

			data::scene::Light const& light = *input.lights[q].first;
			D3DXMATRIX const& worldMatrix = input.lights[q].second;
			D3DXVECTOR3 lightPos = D3DXVECTOR3(
				worldMatrix._41,
				worldMatrix._42,
				worldMatrix._43);
			D3DXVECTOR3 lightDir = D3DXVECTOR3(
				worldMatrix._31,
				worldMatrix._32,
				worldMatrix._33);
			D3DXVec3Normalize(&lightDir, &lightDir);

			vLightDiffuse[q] = D3DXVECTOR4(light.diffuse.r, light.diffuse.g, light.diffuse.b, light.diffuse.a);
			vLightSpecular[q] = D3DXVECTOR4(light.specular.r, light.specular.g, light.specular.b, light.specular.a);
			vLightAmbient += D3DXVECTOR4(light.ambient.r, light.ambient.g, light.ambient.b, light.ambient.a);

			vLightAttenuation[q] = D3DXVECTOR3(light.attenuation[0], light.attenuation[1], light.attenuation[2]);

			switch(light.type)
			{
			case data::scene::Light::Directional:
				vLightType[q] = 0; // @TBD: lightDIRECTIONAL
				vLightDir[q] = lightDir;
				break;
			case data::scene::Light::Spot:
				vLightType[q] = 2; // @TBD: lightSPOT
				vLightPos[q] = lightPos;
				vLightDir[q] = lightDir;
				fLightSpotCutoff[q] = cosf(light.phi);
				fLightSpotExp[q] = 2.0f; //@TBD: light.theat 
				break;
			case data::scene::Light::Point:
				vLightType[q] = 1; // @TBD: lightPOINT
				vLightPos[q] = lightPos;
				break;
			}
		}

		fx().SetValue("nLightType", vLightType, sizeof(int)*MAX_LIGHTS);
		fx().SetValue("vLightPos", vLightPos, sizeof(D3DXVECTOR3)*MAX_LIGHTS);
		fx().SetValue("vLightDir", vLightDir, sizeof(D3DXVECTOR3)*MAX_LIGHTS);
		fx().SetValue("vLightAttenuation", vLightAttenuation, sizeof(D3DXVECTOR3)*MAX_LIGHTS);
		fx().SetValue("fLightSpotCutoff", fLightSpotCutoff, sizeof(float)*MAX_LIGHTS);
		fx().SetValue("fLightSpotExp", fLightSpotExp, sizeof(float)*MAX_LIGHTS);
		fx().SetValue("vLightDiffuse", vLightDiffuse, sizeof(D3DXVECTOR4)*MAX_LIGHTS);
		fx().SetValue("vLightSpecular", vLightSpecular, sizeof(D3DXVECTOR4)*MAX_LIGHTS);
		fx().SetValue("vLightAmbient", vLightAmbient, sizeof(D3DXVECTOR4)*MAX_LIGHTS);
		fx().SetInt("iNumLights", static_cast<int>(input.lights.size()));
	}

	void setupSurface(Input const& input)
	{
		fx().SetVector("vMaterialAmbient", &input.vecs[AmbientColor]);
		fx().SetVector("vMaterialDiffuse", &input.vecs[DiffuseColor]);
		fx().SetVector("vMaterialSpecular", &input.vecs[SpecularColor]);

		IDirect3DBaseTexture9 const* diffuseTexture = input.textures[BaseEffect::DiffuseTexture];
		fx().SetBool("bDiffuseTextureEnabled", (diffuseTexture != 0));
		if(diffuseTexture)
		{
			fx().SetTexture("tDiffuse", const_cast<IDirect3DBaseTexture9*>(diffuseTexture));
			fx().SetInt("nAddressU", D3DTADDRESS_WRAP);//addressU);
			fx().SetInt("nAddressV", D3DTADDRESS_WRAP);//addressV);
		}
	}

	void setupGeometry(Input const& input)
	{
		D3DXMATRIX m;
		D3DXMatrixIdentity(&m);

		m = *input.matrices[BaseEffect::WorldMatrix];
		DX_MSG("set World matrix") =
			fx().SetMatrix("mWorld", &m);

		m = *input.matrices[BaseEffect::ViewMatrix];
		DX_MSG("set View matrix") =
			fx().SetMatrix("mView", &m);

		m = *input.matrices[BaseEffect::WorldViewProjMatrix];
		DX_MSG("set WorldViewProj matrix") =
			fx().SetMatrix("mWorldViewProjection", &m);
	}

	// common
	IDirect3DDevice9& device() const
	{
		ASSERT(gContext.device);
		return *gContext.device;
	}

	ID3DXEffect& fx() const
	{
		ASSERT(gContext.uberShader);
		return *gContext.uberShader;
	}

	void begin(const char* techniqueName)
	{
		DX_MSG("set technqiue") =
			fx().SetTechnique(techniqueName);

		unsigned passes = 0;
		DX_MSG("begin effect") =
			fx().Begin(&passes, D3DXFX_DONOTSAVESTATE);
		hasActivePass = false;
	}

	void end()
	{
		endPass();
		DX_MSG("begin effect") =
			fx().End();
	}

	void pass(unsigned passIndex)
	{
		endPass();
		DX_MSG("begin pass") =
			fx().BeginPass(passIndex);
		hasActivePass = true;
	}

	void endPass()
	{
		if(!hasActivePass) return;
		DX_MSG("begin pass") =
			fx().EndPass();
		hasActivePass = false;
	}

	void commit()
	{
		DX_MSG("commit changes") =
			fx().CommitChanges();
	}

	void captureState()
	{
		stateBlock = 0;
		DX_MSG("capture device state") =
			device().CreateStateBlock(D3DSBT_ALL, &stateBlock);
	}

	void restoreState()
	{
		if(stateBlock == 0) return;

		DX_MSG("restore device state") =
		stateBlock->Apply();
		stateBlock = 0;
	}

	enum { MAX_PASSES = 1 };
	PassInfo						passInfo[MAX_PASSES];
	com_ptr<IDirect3DStateBlock9>	stateBlock;
	unsigned						passIndex;
	bool							hasActivePass;
};

Lambert::Lambert()
:	mImpl(new Impl())
{
	fillRequest(mRequest);
	allocInput(mInput, mRequest);
}

Lambert::~Lambert()
{
}

unsigned Lambert::begin()
{
	mImpl->passIndex = ~0;
	mImpl->begin("Main");
	return Impl::MAX_PASSES;
}

BaseEffect::PassInfo const& Lambert::passInfo(unsigned passIndex)
{
	return mImpl->passInfo[passIndex];
}

void Lambert::pass(unsigned passIndex)
{
	ASSERT(validateInput(mInput, mRequest));
	bool resetPass = (mImpl->passIndex != passIndex);
	switch(passIndex)
	{
	case 0:
		if(resetPass)
			mImpl->pass(0);
		mImpl->setupLights(mInput);
		mImpl->setupSurface(mInput);
		mImpl->setupGeometry(mInput);
		break;
	default:
		ASSERT("Invalid pass index");
	}
	mImpl->commit();
	mImpl->passIndex = passIndex;

/*	unsigned lightCount = mInput.lights.size();
	if (passIndex < lightCount)
	{
		diffusePass(mInput, passIndex);
	}
	else if (passIndex < lightCount * 2)
	{
		specularPass(mInput, passIndex - lightCount);
	}
	else 
	{
		ASSERT("Incorrect pass index");
	}*/
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
