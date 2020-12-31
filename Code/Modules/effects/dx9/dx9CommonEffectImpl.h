#ifndef MUTALISK_EFFECTS__DX9_COMMON_EFFECT_IMPL_H_
#define MUTALISK_EFFECTS__DX9_COMMON_EFFECT_IMPL_H_

#include "../cfg.h"
#include "../BaseEffect.h"
#include "dx9Platform.h"

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
//	void selectLights(BaseEffect::Input::Lights const& lights, unsigned passIndex, LightsPerPass& lightsPerPass);
	size_t organizeLightsInPasses(BaseEffect::Input::Lights const& lights,
		LightsInPassesT& lightsInPasses);

//	void setupLights(BaseEffect::Input const& input);
	void setupLights(LightsPerPass const& input);
	void setupSurface(BaseEffect::Input const& input);
	void setupGeometry(BaseEffect::Input const& input);
	void setupBuffers(BaseEffect::Input const& input);

	void begin(const char* techniqueName);
	void end();

	void pass(unsigned passIndex);
	void commit();
	void endPass();

	void captureState();
	void restoreState();

	IDirect3DDevice9& device() const;
	ID3DXEffect& fx() const;

	com_ptr<IDirect3DStateBlock9>	stateBlock;
	bool							hasActivePass;

	unsigned						passIndex;
};

} // namespace effects 
} // namespace mutalisk

#endif // MUTALISK_EFFECTS__DX9_COMMON_EFFECT_IMPL_H_