#include "pspCommonEffectImpl.h"

#include <mutalisk/utility.h>

using namespace mutalisk;
using namespace mutalisk::effects;

BaseEffect::Input::BufferControl gBufferControl;
bool gBufferControlInitialized = false;

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
	return colorRGBtoDWORD(src) | COLOR_CHANNEL(alpha, 24);
}
ColorT mutalisk::effects::mulAlpha(mutalisk::data::Color src, float alpha)
{
	return colorRGBtoDWORD(src) | COLOR_CHANNEL(src.a * alpha, 24);
}

size_t CommonEffectImpl::organizeLightsInPasses(BaseEffect::Input::Lights const& lights,
	std::vector<LightsPerPass>& lightsInPasses)
{
	size_t estimatedPassCount = ((std::max(1U, lights.count) - 1) >> 2) + 1;
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

	
	if(lightsInPasses.empty())
	{
		LightsPerPass emptyPass;
		emptyPass.count = 0;
		lightsInPasses.push_back(emptyPass);
	}

	return lightsInPasses.size();
}

namespace
{
	float saturate(float v)
	{
		return std::min(std::max(v, 0.0f), 1.0f);
	}
}

void CommonEffectImpl::setupLights(LightsPerPass const& input, BaseEffect::Input const& baseInput)
{
	mutalisk::data::Color totalAmbient;
	totalAmbient.r = 0; totalAmbient.g = 0; totalAmbient.b = 0; totalAmbient.a = 0;

	unsigned lightIndex = 0;
	for(unsigned q = 0; q < input.count; ++q, ++lightIndex)
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

		sceGuLightColor(lightIndex, GU_DIFFUSE, colorRGBAtoDWORD(light.diffuse));
		sceGuLightAtt(lightIndex, light.attenuation[0], light.attenuation[1], light.attenuation[2]);

		totalAmbient.r += light.ambient.r;
		totalAmbient.g += light.ambient.g;
		totalAmbient.b += light.ambient.b;
		totalAmbient.a += light.ambient.a;

		switch(light.type)
		{
		case mutalisk::data::scene::Light::Directional:
			sceGuLight(lightIndex, GU_DIRECTIONAL, GU_DIFFUSE, &lightDir);
			break;
		case mutalisk::data::scene::Light::DirectionalExt:
			{
			mutalisk::data::Color diffuse0, diffuse1;
			diffuse0.r = saturate(light.diffuse.r - light.ambient.r);
			diffuse0.g = saturate(light.diffuse.g - light.ambient.g);
			diffuse0.b = saturate(light.diffuse.b - light.ambient.b);
			diffuse0.a = saturate(light.diffuse.a - light.ambient.a);
			diffuse1.r = saturate(light.diffuseAux0.r - light.ambient.r);
			diffuse1.g = saturate(light.diffuseAux0.g - light.ambient.g);
			diffuse1.b = saturate(light.diffuseAux0.b - light.ambient.b);
			diffuse1.a = saturate(light.diffuseAux0.a - light.ambient.a);
			ScePspFVector3 negLightDir = { -lightDir.x, -lightDir.y, -lightDir.z };

			sceGuLightColor(lightIndex, GU_DIFFUSE, colorRGBAtoDWORD(diffuse0));
			sceGuLight(lightIndex, GU_DIRECTIONAL, GU_DIFFUSE, &lightDir);
			sceGuEnable(GU_LIGHT0 + lightIndex);
			++lightIndex;

			sceGuLightColor(lightIndex, GU_DIFFUSE, colorRGBAtoDWORD(diffuse1));
			sceGuLight(lightIndex, GU_DIRECTIONAL, GU_DIFFUSE, &negLightDir);
			}
			break;

		case mutalisk::data::scene::Light::Spot:
		case mutalisk::data::scene::Light::Point:
			ASSERT("Not supported");
			break;
		}
		lightPos = lightPos;

		sceGuEnable(GU_LIGHT0 + lightIndex);
	}
	sceGuAmbient(colorRGBAtoDWORD(totalAmbient));

	//if(input.count > 0)
	sceGuEnable(GU_LIGHTING);
}

void CommonEffectImpl::setupSurface(BaseEffect::Input const& input)
{
	ASSERT(input.surface);

	BaseEffect::Input::Surface const& surface = *input.surface;
	sceGuModelColor(surface.emissive, surface.diffuse, surface.diffuse, 0);
//	sceGuSendCommandi(88, 0xff);

	sceGuBlendFunc(GU_ADD, surface.srcBlend, surface.dstBlend, 
		surface.srcFix, surface.dstFix);

	bool alphaBlendEnable = !(
		surface.srcBlend == GU_FIX && surface.srcFix == ~0U &&
		surface.dstBlend == GU_FIX && surface.dstFix == 0U);
	if(alphaBlendEnable)
	{
		sceGuEnable(GU_BLEND);
	}
	else
	{
		sceGuDisable(GU_BLEND);
	}
	if (input.surface)
	{
		sceGuTexOffset(input.surface->uOffset, input.surface->vOffset);
		mutalisk::data::psp_texture const* diffuse = input.surface->diffuseTexture;
//		printf("い psp_texture = %x\n", diffuse);
		if (diffuse)
		{
			mutalisk::data::psp_texture const& texture = *diffuse;
//		printf("い width = %i\n", texture.width);
//		printf("い height = %i\n", texture.height);
//		printf("い format = %i\n", texture.format);
//		printf("い stride = %i\n", texture.stride);

//		printf("い alloc = %x\n", texture.data);

//		printf("い clut? = %i\n", texture.clutFormat);
//		printf("い clut# = %i\n", texture.clutEntries);
//		printf("い clut@ = %x\n", texture.clut);
			if(texture.clutEntries)
			{
				sceGuClutMode(texture.clutFormat,0,0xff,0);
				sceGuClutLoad(texture.clutEntries,texture.clut);
			}
			sceGuTexMode(texture.format,0,0,texture.swizzled);
			sceGuTexImage(texture.mipmap,texture.width,texture.height,texture.stride,texture.data);
			sceGuTexWrap(input.surface->xTexWrap, input.surface->yTexWrap);
			sceGuTexFilter(GU_LINEAR_MIPMAP_NEAREST, GU_LINEAR_MIPMAP_NEAREST);
			sceGuTexFunc(GU_TFX_MODULATE, GU_TCC_RGBA);
			sceGuEnable(GU_TEXTURE_2D);
		}
		else
		{
			sceGuDisable(GU_TEXTURE_2D);
		}
	}
	else
	{
		printf("い no surface\n");
	}
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
