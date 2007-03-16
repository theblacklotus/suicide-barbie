#ifndef MUTALISK_EFFECT__SHINY_H_
#define MUTALISK_EFFECT__SHINY_H_

#include "../cfg.h"

#include <memory>
#include "../BaseEffect.h"

namespace mutalisk { namespace effects {

class Shiny : public BaseEffect
{
protected:
	static void fillRequest(Request& request);

public:
	Shiny();
	virtual ~Shiny();

	virtual unsigned begin();
	virtual PassInfo const& passInfo(unsigned passIndex);
	virtual void pass(unsigned passIndex);
	virtual void end();

	virtual void captureState();

/*	virtual void begin();
	virtual unsigned passCount(Input const& i);
	virtual PassInfo const& passInfo(Input const& i, unsigned passIndex);
	virtual void pass(Input const& i, unsigned passIndex);
	virtual void end();
*/

private:
	struct Impl;
	std::auto_ptr<Impl>	mImpl;
};

} // namespace effects 
} // namespace mutalisk

#endif // MUTALISK_EFFECT__SHINY_H_
