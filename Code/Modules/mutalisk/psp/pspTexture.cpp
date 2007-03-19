#include "psp.h"

namespace mutalisk { namespace data
{

// psp_texture
//
psp_texture::psp_texture()
: data(0)
, vramAddr(0)
{
}

psp_texture::~psp_texture()
{ 
	clear(*this);
}

void clear(psp_texture& data)
{
}

} // namespace data 
} // namespace mutalisk
