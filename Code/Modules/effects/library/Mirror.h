#ifndef MUTALISK_EFFECT__MIRROR_H_
#define MUTALISK_EFFECT__MIRROR_H_

#include "../cfg.h"

#include <memory>
#include "../BaseEffect.h"

namespace mutalisk { namespace effects {

class Mirror : public BaseEffect
{
public:
	Mirror();
	virtual ~Mirror();

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

void setTexProjMatrix(MatrixT const& m);

} // namespace effects 
} // namespace mutalisk

#endif // MUTALISK_EFFECT__MIRROR_H_
