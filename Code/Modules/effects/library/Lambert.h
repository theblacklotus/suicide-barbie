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
	static void fillRequest(Request& request);

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
