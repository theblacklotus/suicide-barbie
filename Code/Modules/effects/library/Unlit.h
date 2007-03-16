#ifndef MUTALISK_EFFECT__UNLIT_H_
#define MUTALISK_EFFECT__UNLIT_H_

#include "../cfg.h"

#include <memory>
#include "../BaseEffect.h"

namespace mutalisk { namespace effects {

class Unlit : public BaseEffect
{
public:
	Unlit();
	virtual ~Unlit();

	virtual void begin();
	virtual unsigned passCount(Input const& i);
	virtual PassInfo const& passInfo(Input const& i, unsigned passIndex);
	virtual void pass(Input const& i, unsigned passIndex);
	virtual void end();
	
	virtual void captureState();

private:
	struct Impl;
	std::auto_ptr<Impl>	mImpl;
};

} // namespace effects 
} // namespace mutalisk

#endif // MUTALISK_EFFECT__UNLIT_H_
