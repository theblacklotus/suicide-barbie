#ifndef MUTALISK_PSP_TEXTURE_H_
#define MUTALISK_PSP_TEXTURE_H_

#include "mtxHeader.h"

namespace mutalisk { namespace data
{
	struct psp_texture
	{
		int format;
		int mipmap;
		int width, height, stride;
		void* data;
		void* vramAddr;

		int clutFormat;
		int clutEntries;
		void* clut;

		// memory management
		psp_texture(); ~psp_texture();
	};

	// memory management
	void clear(psp_texture& data);

// psp_texture
//
inline template <typename In> In& operator>> (In& i, psp_texture& texture)
{
//	clear(texture);
	memset(&texture, 0, sizeof(texture));

	try
	{
		MtxHeader header;

		i.readArray(&header, 1);
		assert(MtxHeader::Signature_CurrentVersion == header.signature);

		texture.format = header.pixelFormat;
		texture.width  = header.textureWidth;
		texture.height = header.textureHeight;
		texture.stride = header.textureStride;

		texture.clutFormat = header.clutFormat;
		texture.clutEntries = header.clutEntries;

		texture.data = malloc(header.vramAllocationSize);
		texture.clut = (void*)(header.paletteOffset + (int)texture.data);
		
		printf("い width = %i\n", texture.width);
		printf("い height = %i\n", texture.height);
		printf("い format = %i\n", texture.format);
		printf("い stride = %i\n", texture.stride);
		printf("い alloc = %x\n", texture.data);
		printf("い size = %i\n", header.vramAllocationSize);
		printf("い clut? = %i\n", texture.clutFormat);
		printf("い clut# = %i\n", texture.clutEntries);
		printf("い clut@ = %x\n", texture.clut);

//		i.readArray((u32*)texture.data, header.vramAllocationSize/4);
		i.readData(texture.data, header.vramAllocationSize);
/*
		// base_mesh
		i >> mesh.base();

		// psp_mesh
		mesh.vertexDecl = i.readDword();
		mesh.primitiveType = i.readDword();
		if( i.readBool() )
		{
			mesh.skinInfo = new skin_info;
			i >> *mesh.skinInfo;

			mesh.boneIndexStride = i.readDword();
			mesh.boneIndexDataSize = i.readDword();
			mesh.boneIndexData = new unsigned char[mesh.boneIndexDataSize];
			i.readArray(mesh.boneIndexData, mesh.boneIndexDataSize);
		}
		else
		{	
			mesh.skinInfo = 0;

			mesh.boneIndexStride = 0;
			mesh.boneIndexDataSize = 0;
			mesh.boneIndexData = 0;
		}
*/	} catch( EIoEof& ) {
		mutant_throw( "Unexpected end-of-file (file may be corrupted)" );
	} catch( EIoError& ) {
		mutant_throw( "Read/write error" );
	}
	return i;
}


} // namespace data
} // namespace mutalisk

#endif // MUTALISK_PSP_TEXTURE_H_
