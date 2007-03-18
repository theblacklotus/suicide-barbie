#include "pspCommonEffectImpl.h"

using namespace mutalisk;
using namespace mutalisk::effects;

BaseEffect::Input::BufferControl gBufferControl;
bool gBufferControlInitialized = false;

namespace 
{
#define CHANNEL(v, shift) ((unsigned int)((v) * 255.0f) << (shift))
unsigned color4(mutalisk::data::Color const& srcColor)
{
	return CHANNEL(srcColor.r, 0) | CHANNEL(srcColor.g, 8) | CHANNEL(srcColor.b, 16) | CHANNEL(srcColor.a, 24);
}
unsigned color3(mutalisk::data::Color const& srcColor)
{
	return CHANNEL(srcColor.r, 0) | CHANNEL(srcColor.g, 8) | CHANNEL(srcColor.b, 16);
}
}

ColorT mutalisk::effects::replaceAlpha(ColorT src, unsigned int alpha)
{
	src &= 0x00ffffff;
	src |= (alpha << 24);
	return src;
}
ColorT mutalisk::effects::mulAlpha(ColorT src, unsigned int alpha)
{
	unsigned srcAlpha = (src & 0xff000000)>>24;
	return replaceAlpha(src, (srcAlpha * alpha) >> 8);
}
ColorT mutalisk::effects::replaceAlpha(mutalisk::data::Color src, float alpha)
{
	return color3(src) | CHANNEL(alpha, 24);
}
ColorT mutalisk::effects::mulAlpha(mutalisk::data::Color src, float alpha)
{
	return color3(src) | CHANNEL(src.a * alpha, 24);
}

size_t CommonEffectImpl::organizeLightsInPasses(BaseEffect::Input::Lights const& lights,
	std::vector<LightsPerPass>& lightsInPasses)
{
	size_t estimatedPassCount = (std::max(1U, lights.count) - 1) >> 2 + 1;
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
	mutalisk::data::Color totalAmbient;
	totalAmbient.r = 0; totalAmbient.g = 0; totalAmbient.b = 0; totalAmbient.a = 0;

	for(unsigned q = 0; q < input.count; ++q)
	{
		ASSERT(q < MAX_LIGHTS);
		ASSERT(input.lights[q]);
		ASSERT(input.matrices[q]);

		data::scene::Light const& light = *input.lights[q];
		ScePspFMatrix4 const& worldMatrix = *input.matrices[q];
		ScePspFVector3 lightPos = { worldMatrix.w.x, worldMatrix.w.y, worldMatrix.w.z };
		ScePspFVector3 lightDir = { worldMatrix.z.x, worldMatrix.z.y, worldMatrix.z.z };

		// $TBD: support specular
		// $TBD: support point/spot lights

		sceGuLightColor(q, GU_DIFFUSE, color4(light.diffuse));
		sceGuLightAtt(q, light.attenuation[0], light.attenuation[1], light.attenuation[2]);

		totalAmbient.r += light.ambient.r;
		totalAmbient.g += light.ambient.g;
		totalAmbient.b += light.ambient.b;
		totalAmbient.a += light.ambient.a;

		switch(light.type)
		{
		case mutalisk::data::scene::Light::Directional:
			sceGuLight(q, GU_DIRECTIONAL, GU_DIFFUSE, &lightDir);
			break;
		case mutalisk::data::scene::Light::DirectionalExt:
		case mutalisk::data::scene::Light::Spot:
		case mutalisk::data::scene::Light::Point:
			ASSERT("Not supported");
			break;
		}
		lightPos = lightPos;

		sceGuEnable(GU_LIGHT0 + q);
	}
	sceGuAmbient(color4(totalAmbient));

	if(input.count > 0)
		sceGuEnable(GU_LIGHTING);

/*	int vLightType[MAX_LIGHTS];							// Light's position in world space
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
	*/
}

void CommonEffectImpl::setupSurface(BaseEffect::Input const& input)
{
	ASSERT(input.surface);

	sceGuAmbientColor(~0U);
	BaseEffect::Input::Surface const& surface = *input.surface;

/*	fx().SetValue("vMaterialAmbient", &surface.ambient, sizeof(surface.ambient));
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
	fx().SetInt("nDestBlend", surface.dstBlend);*/

	sceGuBlendFunc(GU_ADD, surface.srcBlend, surface.dstBlend, 
		surface.srcFix, surface.dstFix);

	bool alphaBlendEnable = !(
		surface.srcBlend == GU_FIX && surface.srcFix == ~0U &&
		surface.dstBlend == GU_FIX && surface.dstFix == 0U);
//	if(alphaBlendEnable)
//		sceGuEnable(GU_BLEND);
//	else
//		sceGuDisable(GU_BLEND);
	sceGuDisable(GU_BLEND);
}

void CommonEffectImpl::setupGeometry(BaseEffect::Input const& input)
{
	ASSERT(input.matrices);
	sceGumMatrixMode(GU_PROJECTION);
	sceGumLoadMatrix(&input.matrices[BaseEffect::ProjMatrix]);	// 2
	
	sceGumMatrixMode(GU_VIEW);
	sceGumLoadMatrix(&input.matrices[BaseEffect::ViewMatrix]);	// 1

	sceGumMatrixMode(GU_MODEL);
	sceGumLoadMatrix(&input.matrices[BaseEffect::WorldMatrix]);	// 0

/*	DX_MSG("set World matrix") =
		fx().SetMatrix("mWorld", &input.matrices[BaseEffect::WorldMatrix]);	// 0

	DX_MSG("set View matrix") =
		fx().SetMatrix("mView", &input.matrices[BaseEffect::ViewMatrix]);	// 1

	DX_MSG("set WorldViewProj matrix") =
		fx().SetMatrix("mWorldViewProjection", &input.matrices[BaseEffect::WorldViewProjMatrix]);	// 4
	*/
}

void CommonEffectImpl::setupBuffers(BaseEffect::Input const& input)
{
	ASSERT(input.bufferControl);
	BaseEffect::Input::BufferControl const& bufferControl = *input.bufferControl;
	
	if(gBufferControl.colorWriteEnable != bufferControl.colorWriteEnable || !gBufferControlInitialized)
	{
		//	device().SetRenderState(D3DRS_COLORWRITEENABLE, (gBufferControl.colorWriteEnable)? 
		//		D3DCOLORWRITEENABLE_ALPHA|D3DCOLORWRITEENABLE_BLUE|D3DCOLORWRITEENABLE_GREEN|D3DCOLORWRITEENABLE_RED: 0);
//		sceGuPixelMask((bufferControl.colorWriteEnable)? ~0U: 0U);
	}

	if(gBufferControl.zWriteEnable != bufferControl.zWriteEnable || !gBufferControlInitialized)
	//	device().SetRenderState(D3DRS_ZWRITEENABLE, bufferControl.zWriteEnable);
	{
		sceGuDepthMask((bufferControl.zWriteEnable)? GU_FALSE: GU_TRUE);//GU_TRUE);
	}

	if(gBufferControl.zReadEnable != bufferControl.zReadEnable || !gBufferControlInitialized)
	{
		if(bufferControl.zReadEnable)
			sceGuEnable(GU_DEPTH_TEST);
		else
			sceGuDisable(GU_DEPTH_TEST);
	}

	if(gBufferControl.zEqual != bufferControl.zEqual || !gBufferControlInitialized)
		sceGuDepthFunc((bufferControl.zEqual)? GU_EQUAL: GU_LEQUAL);

	gBufferControlInitialized = true;
	gBufferControl = *input.bufferControl;
}

void CommonEffectImpl::begin()
{
	passIndex = ~0U;
}

void CommonEffectImpl::end()
{
}

void CommonEffectImpl::pass(unsigned i)
{
	passIndex = i;
}

void CommonEffectImpl::endPass()
{
}

void CommonEffectImpl::commit()
{
}

void CommonEffectImpl::captureState()
{
//	stateBlock = sceGuGetAllStatus();
}

void CommonEffectImpl::restoreState()
{
//	sceGuSetAllStatus(stateBlock);
}
