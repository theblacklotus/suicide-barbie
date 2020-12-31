#ifndef MUTALISK_METAINFO_H_
#define MUTALISK_METAINFO_H_

#include "cfg.h"

namespace mutalisk { namespace data
{

	// metainfo
	template <typename fwd_parent>
	struct parent : public fwd_parent
	{
		typedef fwd_parent	base_type;
		base_type& base() { return *this; }
		base_type const& base() const { return *this; }
	};

} // namespace data 
} // namespace mutalisk

#endif // MUTALISK_METAINFO_H_
