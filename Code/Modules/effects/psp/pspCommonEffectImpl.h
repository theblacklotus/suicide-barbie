#ifndef MUTALISK_EFFECTS__PSP_COMMON_EFFECT_IMPL_H_
#define MUTALISK_EFFECTS__PSP_COMMON_EFFECT_IMPL_H_

#include "../cfg.h"
#include "../BaseEffect.h"
#include "pspPlatform.h"

namespace mutalisk { namespace effects {

struct CommonEffectImpl
{
	enum { MAX_LIGHTS = 4 };
	struct LightsPerPass {
		LightT const*	lights[MAX_LIGHTS];
		MatrixT	const*	matrices[MAX_LIGHTS];
		size_t			count;
	};
	typedef std::vector<LightsPerPass>	LightsInPassesT;
	size_t organizeLightsInPasses(BaseEffect::Input::Lights const& lights,
		LightsInPassesT& lightsInPasses);

	void setupLights(LightsPerPass const& input, BaseEffect::Input const& baseInput);
	void setupSurface(BaseEffect::Input const& input);
	void setupGeometry(BaseEffect::Input const& input);
	void setupBuffers(BaseEffect::Input const& input);

	void begin();
	void end();

	void pass(unsigned passIndex);
	void commit();
	void endPass();

	void captureState();
	void restoreState();

	unsigned	passIndex;

private:
	int			stateBlock;
};

ColorT replaceAlpha(ColorT src, unsigned int alpha);
ColorT mulAlpha(ColorT src, unsigned int alpha);
ColorT replaceAlpha(mutalisk::data::Color src, float alpha);
ColorT mulAlpha(mutalisk::data::Color src, float alpha);

} // namespace effects 
} // namespace mutalisk

#endif // MUTALISK_EFFECTS__PSP_COMMON_EFFECT_IMPL_H_
