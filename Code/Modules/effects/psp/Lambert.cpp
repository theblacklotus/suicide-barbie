#include "../library/Lambert.h"

#include "pspPlatform.h"
#include "pspCommonEffectImpl.h"

using namespace mutalisk;
using namespace mutalisk::effects;
/*
void Lambert::fillRequest(Request& request)
{
	request.required.matrices.push_back(BaseEffect::WorldMatrix);
	request.required.matrices.push_back(BaseEffect::ViewMatrix);
	request.required.matrices.push_back(BaseEffect::ProjMatrix);
	request.optional.textures.push_back(BaseEffect::DiffuseTexture);
	request.required.vecs.push_back(BaseEffect::AmbientColor);
	request.required.vecs.push_back(BaseEffect::DiffuseColor);
	request.required.vecs.push_back(BaseEffect::SpecularColor);
	request.lightCountRange.first = 0;
	request.lightCountRange.second = MAX_LIGHTS;
}*/

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
/*
{
	void toNative(mutalisk::data::Color const& color, ScePspFVector4& vec)
	{
		vec.x = color.r;
		vec.y = color.g;
		vec.z = color.b;
		vec.w = color.a;
	}

	unsigned int toNative(mutalisk::data::Color const& srcColor)
	{
		ScePspFVector4 vec;
		toNative(srcColor, vec);
		return toNative(vec);
	}

	unsigned int toNative(ScePspFVector4 const& srcColor)
	{
		unsigned int 
		dstColor = (unsigned int)(srcColor.x * 255.0f);
		dstColor |= (unsigned int)(srcColor.y * 255.0f) << 8;
		dstColor |= (unsigned int)(srcColor.z * 255.0f) << 16;
		dstColor |= (unsigned int)(srcColor.w * 255.0f) << 24;
		return dstColor;
	}

	void setupLights(Input const& input)
	{
		ScePspFVector4 totalAmbient = {0,0,0,0};
		for(unsigned q = 0; q < input.lights.size(); ++q)
		{
			ASSERT(q < MAX_LIGHTS);
			if(!input.lights[q].first)
				continue;

			data::scene::Light const& light = *input.lights[q].first;
			ScePspFMatrix4 const& worldMatrix = input.lights[q].second;
			ScePspFVector3 lightPos = { worldMatrix.w.x, worldMatrix.w.y, worldMatrix.w.z };
			ScePspFVector3 lightDir = { worldMatrix.z.x, worldMatrix.z.y, worldMatrix.z.z };

			// $TBD: support specular
			// $TBD: support point/spot lights

			sceGuLightColor(q, GU_DIFFUSE, toNative(light.diffuse));
			sceGuLightAtt(q, light.attenuation[0], light.attenuation[1], light.attenuation[2]);

			totalAmbient.x += light.ambient.r;
			totalAmbient.y += light.ambient.g;
			totalAmbient.z += light.ambient.b;
			totalAmbient.w += light.ambient.a;

			switch(light.type)
			{
			case mutalisk::data::scene::Light::Directional:
				sceGuLight(q, GU_DIRECTIONAL, GU_DIFFUSE, &lightDir);
				break;
			case mutalisk::data::scene::Light::Spot:
			case mutalisk::data::scene::Light::Point:
				break;
			}
			lightPos = lightPos;

			sceGuEnable(GU_LIGHT0 + q);
		}
		sceGuAmbient(toNative(totalAmbient));

		if(!input.lights.empty())
			sceGuEnable(GU_LIGHTING);
	}

	void setupSurface(Input const& input)
	{
//		toNative(input.vecs[AmbientColor]);
//		toNative(input.vecs[DiffuseColor]);
//		toNative(input.vecs[SpecularColor]);
	}

	void setupGeometry(Input const& input)
	{
		sceGumMatrixMode(GU_PROJECTION);
		sceGumLoadMatrix(input.matrices[BaseEffect::ProjMatrix]);
		
		sceGumMatrixMode(GU_VIEW);
		sceGumLoadMatrix(input.matrices[BaseEffect::ViewMatrix]);

		sceGumMatrixMode(GU_MODEL);
		sceGumLoadMatrix(input.matrices[BaseEffect::WorldMatrix]);
	}

	void pass(unsigned passIndex)
	{
	}

	void restoreState()
	{
	}


	enum { MAX_PASSES = 1 };
	PassInfo						passInfo[MAX_PASSES];
	unsigned						passIndex;
	bool							hasActivePass;
};
*/
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
