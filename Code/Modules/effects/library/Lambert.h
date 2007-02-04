#ifndef MUTALISK_EFFECT__LAMBERT_H_
#define MUTALISK_EFFECT__LAMBERT_H_

#include "../cfg.h"

#include <memory>
#include "../BaseEffect.h"

namespace mutalisk { namespace effects {

class Lambert : public BaseEffect
{
protected:
	enum { MAX_LIGHTS = 4 };
	static void fillRequest(Request& request)
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

public:
	Lambert();
	virtual ~Lambert();

	virtual unsigned begin();
	virtual PassInfo const& passInfo(unsigned passIndex);
	virtual void pass(unsigned passIndex);
	virtual void end();

	virtual void captureState();

private:
	struct Impl;
	std::auto_ptr<Impl>	mImpl;
};

} // namespace effects 
} // namespace mutalisk

#endif // MUTALISK_EFFECT__LAMBERT_H_
