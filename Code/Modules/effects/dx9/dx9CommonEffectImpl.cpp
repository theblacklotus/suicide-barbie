#include "dx9CommonEffectImpl.h"

using namespace mutalisk;
using namespace mutalisk::effects;

BaseEffect::Input::BufferControl gBufferControl;
bool gBufferControlInitialized = false;

size_t CommonEffectImpl::organizeLightsInPasses(BaseEffect::Input::Lights const& lights,
	std::vector<LightsPerPass>& lightsInPasses)
{
	size_t estimatedPassCount = (max(1, lights.count) - 1) / 4 + 1;
	lightsInPasses.reserve(estimatedPassCount);

	unsigned light = 0;
	for(unsigned q = 0; q < estimatedPassCount; ++q)
	{
		LightsPerPass pass;
		pass.count = 0;
		for(unsigned w = 0; w < 4 && light < lights.count; ++w, ++light)
		{
			pass.lights[w] = &lights.data[light];
			pass.matrices[w] = &lights.matrices[light];
			++pass.count;
		}
		lightsInPasses.push_back(pass);
	}
	return estimatedPassCount;
}

void CommonEffectImpl::setupLights(LightsPerPass const& input)
{
	int vLightType[MAX_LIGHTS];							// Light's position in world space
	D3DXVECTOR3 vLightPos[MAX_LIGHTS];					// Light's position in world space
	D3DXVECTOR3 vLightDir[MAX_LIGHTS];					// Light's direction in world space
	D3DXVECTOR3 vLightAttenuation[MAX_LIGHTS];			// Light's attenuations {attn0, attn1, attn2}
	float  fLightSpotCutoff[MAX_LIGHTS];				// Light's spot cutoff
	float  fLightSpotExp[MAX_LIGHTS];					// Light's spot exponent
	D3DXVECTOR4 vLightDiffuse[MAX_LIGHTS];				// Light's diffuse color
	D3DXVECTOR4 vLightDiffuseAux0[MAX_LIGHTS];			// Light's diffuse auxilary color 0
	D3DXVECTOR4 vLightDiffuseAux1[MAX_LIGHTS];			// Light's diffuse auxilary color 1
	D3DXVECTOR4 vLightSpecular[MAX_LIGHTS];				// Light's specular color
	D3DXVECTOR4 vLightAmbient = D3DXVECTOR4(0,0,0,0);	// Light's ambient color

	for(unsigned q = 0; q < input.count; ++q)
	{
		ASSERT(q < MAX_LIGHTS);
		ASSERT(input.lights[q]);
		ASSERT(input.matrices[q]);

		data::scene::Light const& light = *input.lights[q];
		D3DXMATRIX const& worldMatrix = *input.matrices[q];
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
			fLightSpotExp[q] = 2.0f; //@TBD: light.theta
			break;
		case data::scene::Light::Point:
			vLightType[q] = 1; // @TBD: lightPOINT
			vLightPos[q] = lightPos;
			break;
		case data::scene::Light::DirectionalExt:
			vLightType[q] = 3; // @TBD: lightDIRECTIONAL_EXT
			vLightDir[q] = lightDir;
			break;
		}

		switch(light.type)
		{
		case data::scene::Light::Directional:
		case data::scene::Light::Spot:
		case data::scene::Light::Point:
			vLightAmbient += D3DXVECTOR4(light.ambient.r, light.ambient.g, light.ambient.b, light.ambient.a);
			vLightDiffuseAux0[q] = D3DXVECTOR4(0,0,0,0);	
			vLightDiffuseAux1[q] = D3DXVECTOR4(0,0,0,0);	
			break;

		case data::scene::Light::DirectionalExt:
			vLightDiffuseAux0[q] = 
				D3DXVECTOR4(light.diffuseAux0.r, light.diffuseAux0.g, light.diffuseAux0.b, light.diffuseAux0.a);
			vLightDiffuseAux1[q] =
				D3DXVECTOR4(light.ambient.r, light.ambient.g, light.ambient.b, light.ambient.a);
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
	fx().SetValue("vLightDiffuseAux0", vLightDiffuseAux0, sizeof(D3DXVECTOR4)*MAX_LIGHTS);
	fx().SetValue("vLightDiffuseAux1", vLightDiffuseAux1, sizeof(D3DXVECTOR4)*MAX_LIGHTS);
	fx().SetValue("vLightSpecular", vLightSpecular, sizeof(D3DXVECTOR4)*MAX_LIGHTS);
	fx().SetValue("vLightAmbient", vLightAmbient, sizeof(D3DXVECTOR4));
	fx().SetInt("iNumLights", static_cast<int>(input.count));
}

void CommonEffectImpl::setupSurface(BaseEffect::Input const& input)
{
	ASSERT(input.surface);
	BaseEffect::Input::Surface const& surface = *input.surface;

	fx().SetValue("vMaterialAmbient", &surface.ambient, sizeof(surface.ambient));
	fx().SetValue("vMaterialDiffuse", &surface.diffuse, sizeof(surface.diffuse));
	fx().SetValue("vMaterialSpecular", &surface.specular, sizeof(surface.specular));

	IDirect3DBaseTexture9 const* diffuseTexture = surface.diffuseTexture;
	fx().SetBool("bDiffuseTextureEnabled", (diffuseTexture != 0));
	if(diffuseTexture)
	{
		fx().SetTexture("tDiffuse", const_cast<IDirect3DBaseTexture9*>(diffuseTexture));
		fx().SetInt("nAddressU", D3DTADDRESS_WRAP);//addressU);
		fx().SetInt("nAddressV", D3DTADDRESS_WRAP);//addressV);
	}

	fx().SetFloat("fOpacity", 1.0f - surface.transparency);
	fx().SetValue("vUvOffset", &D3DXVECTOR2(surface.uOffset, surface.vOffset), sizeof(D3DXVECTOR2));

	fx().SetInt("nSrcBlend", surface.srcBlend);
	fx().SetInt("nDestBlend", surface.dstBlend);
	bool alphaBlendEnable = !((input.surface->srcBlend == D3DBLEND_ONE) && (input.surface->dstBlend == D3DBLEND_ZERO));
	fx().SetBool("bAlphaBlendEnable", alphaBlendEnable);
}

void CommonEffectImpl::setupGeometry(BaseEffect::Input const& input)
{
	ASSERT(input.matrices);

	DX_MSG("set World matrix") =
		fx().SetMatrix("mWorld", &input.matrices[BaseEffect::WorldMatrix]);	// 0

	DX_MSG("set View matrix") =
		fx().SetMatrix("mView", &input.matrices[BaseEffect::ViewMatrix]);	// 1

	DX_MSG("set WorldViewProj matrix") =
		fx().SetMatrix("mWorldViewProjection", &input.matrices[BaseEffect::WorldViewProjMatrix]);	// 4
}

void CommonEffectImpl::setupBuffers(BaseEffect::Input const& input)
{
	ASSERT(input.bufferControl);
	BaseEffect::Input::BufferControl const& bufferControl = *input.bufferControl;
	
	if(gBufferControl.colorWriteEnable != bufferControl.colorWriteEnable || !gBufferControlInitialized)
		device().SetRenderState(D3DRS_COLORWRITEENABLE, (gBufferControl.colorWriteEnable)? 
			D3DCOLORWRITEENABLE_ALPHA|D3DCOLORWRITEENABLE_BLUE|D3DCOLORWRITEENABLE_GREEN|D3DCOLORWRITEENABLE_RED: 0);

	if(gBufferControl.zWriteEnable != bufferControl.zWriteEnable || !gBufferControlInitialized)
		device().SetRenderState(D3DRS_ZWRITEENABLE, bufferControl.zWriteEnable);

	if(gBufferControl.zReadEnable != bufferControl.zReadEnable || !gBufferControlInitialized)
		device().SetRenderState(D3DRS_ZENABLE, bufferControl.zReadEnable);

	if(gBufferControl.zEqual != bufferControl.zEqual || !gBufferControlInitialized)
		device().SetRenderState(D3DRS_ZFUNC, (bufferControl.zEqual)? D3DCMP_EQUAL: D3DCMP_LESSEQUAL);

	gBufferControlInitialized = true;
	gBufferControl = *input.bufferControl;
}

IDirect3DDevice9& CommonEffectImpl::device() const
{
	ASSERT(gContext.device);
	return *gContext.device;
}

ID3DXEffect& CommonEffectImpl::fx() const
{
	ASSERT(gContext.uberShader);
	return *gContext.uberShader;
}

void CommonEffectImpl::begin(const char* techniqueName)
{
	DX_MSG("set technqiue") =
		fx().SetTechnique(techniqueName);

	unsigned passes = 0;
	DX_MSG("begin effect") =
		fx().Begin(&passes, D3DXFX_DONOTSAVESTATE);
	hasActivePass = false;
}

void CommonEffectImpl::end()
{
	endPass();
	DX_MSG("begin effect") =
		fx().End();
}

void CommonEffectImpl::pass(unsigned i)
{
	endPass();
	DX_MSG("begin pass") =
		fx().BeginPass(i);
	hasActivePass = true;
	passIndex = i;
}

void CommonEffectImpl::endPass()
{
	if(!hasActivePass) return;
	DX_MSG("begin pass") =
		fx().EndPass();
	hasActivePass = false;
}

void CommonEffectImpl::commit()
{
	DX_MSG("commit changes") =
		fx().CommitChanges();
}

void CommonEffectImpl::captureState()
{
	stateBlock = 0;
	DX_MSG("capture device state") =
		device().CreateStateBlock(D3DSBT_ALL, &stateBlock);
}

void CommonEffectImpl::restoreState()
{
	if(stateBlock == 0) return;

	DX_MSG("restore device state") =
	stateBlock->Apply();
	stateBlock = 0;
}