#include "psp.h"

namespace mutalisk { namespace data
{

// psp_texture
//
psp_texture::psp_texture()
: data(0)
, vramAddr(0)
, memContainer(0)
{
}

psp_texture::~psp_texture()
{ 
	clear(*this);
}

void clear(psp_texture& data)
{
	if (data.memContainer)
	{
//		printf("い free = %x\n", (unsigned int)data.memContainer);
		free(data.memContainer);
	}
	else if (data.data)
	{
//		printf("い free = %x\n", (unsigned int)data.data);
		free(data.data);
	}
	memset(&data, 0x00, sizeof(data));
}

void psp_texture::patchupTextureFromMemory(MtxHeader* header)
{
	assert(MtxHeader::Signature_CurrentVersion == header->signature);

	memContainer = header;			// take over ownership of memory

	format = header->pixelFormat;
	width  = header->textureWidth;
	height = header->textureHeight;
	stride = header->textureStride;
	mipmap = 0;

	clutFormat = header->clutFormat;
	clutEntries = header->clutEntries;

	data = header+1;
	clut = (void*)(header->paletteOffset + (int)data);

	swizzled = header->swizzle;
//	printf("い alloc = %x\n", (unsigned int)data);
/*
	printf("い width = %i\n", width);
	printf("い height = %i\n", height);
	printf("い format = %i\n", format);
	printf("い stride = %i\n", stride);
	printf("い alloc = %x\n", (unsigned int)data);
	printf("い size = %i\n", header->vramAllocationSize);
	printf("い clut? = %i\n", clutFormat);
	printf("い clut# = %i\n", clutEntries);
	printf("い clut@ = %x\n", (unsigned int)clut);
	printf("い swizzled = %s\n", swizzled ? "yes" : "no");
*/
}

} // namespace data 
} // namespace mutalisk
