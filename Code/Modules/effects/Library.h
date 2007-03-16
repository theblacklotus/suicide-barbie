#ifndef MUTALISK_EFFECTS__LIBRARY_H_
#define MUTALISK_EFFECTS__LIBRARY_H_

#include "cfg.h"
#include "platform.h"

namespace mutalisk { namespace effects {
	struct BaseEffect;

	typedef unsigned IndexT;
	enum { Default = 0, NotFound = ~0 };
	IndexT getIndexByName(std::string const& name);
	BaseEffect* getByIndex(IndexT index);

	unsigned version();

} // namespace effects 
} // namespace mutalisk

#endif // MUTALISK_EFFECTS__BASE_EFFECT_H_
