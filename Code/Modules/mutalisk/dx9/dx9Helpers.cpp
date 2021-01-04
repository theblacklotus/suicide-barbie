#include "dx9Helpers.h"

#include <string>
#include <cstdlib>
#include "../errors.h"

using namespace dx;

unsigned calcTextureSize(IDirect3DTexture9 const& texture)
{
	IDirect3DTexture9& tex = const_cast<IDirect3DTexture9&>(texture);
	const int mipMapLevels = tex.GetLevelCount();	
	
	//
	// Size calculated by this equation:
	// 1. in case of uncompressed textures - width*height*bytesPerPixel.
	// 2. in case of compressed textures - max(1, width/4)*max(1, height/4)*(DXTver == 1 ? 8 : 16).
	// To treat both compressed and uncompressed textures uniformly lets use -
	// bytes = max(1,width/divisor)*max(1,height/divisor)*multiplier, and in case of
	// uncompressed textures use 1 for divisor and bytesPerPixel in place of multiplier.
	//
	int multiplier = 0;
	int divisor = 1;
	
	// Get first mip map level description:
	D3DSURFACE_DESC sd;
	tex.GetLevelDesc( 0, &sd );
	
	// Initialize multiplier and divisor to match actual format:
	switch (sd.Format) {
		
		// 64 bpp???
		case D3DFMT_A16B16G16R16:
			multiplier = 8;
			break;

		// 32 bpp:
		case D3DFMT_A2W10V10U10:
		case D3DFMT_V16U16:
		case D3DFMT_X8L8V8U8:
		case D3DFMT_Q8W8V8U8:
		case D3DFMT_A8R8G8B8:
		case D3DFMT_X8R8G8B8:
		case D3DFMT_A2B10G10R10:
		case D3DFMT_A8B8G8R8:
		case D3DFMT_X8B8G8R8:
		case D3DFMT_G16R16:
		case D3DFMT_A2R10G10B10:
			multiplier = 4;
			break;

		// 24 bpp:
		case D3DFMT_R8G8B8:
			multiplier = 3;
			break;
		 
		// 16 bpp:
		case D3DFMT_V8U8:
		case D3DFMT_L6V5U5:
		case D3DFMT_A8P8:
		case D3DFMT_A8L8:
		case D3DFMT_R5G6B5:
		case D3DFMT_X1R5G5B5:
		case D3DFMT_A1R5G5B5:
		case D3DFMT_A4R4G4B4:
		case D3DFMT_A8R3G3B2:
		case D3DFMT_X4R4G4B4:
			multiplier = 2;
			break;

		// 8 bpp:
		case D3DFMT_A4L4:
		case D3DFMT_P8:
		case D3DFMT_L8:
		case D3DFMT_R3G3B2:
		case D3DFMT_A8:
			multiplier = 1;
			break;

    // Compressed:
		case D3DFMT_DXT1:
			divisor = 4;
			multiplier = 8;
			break;

		case D3DFMT_DXT2:
		case D3DFMT_DXT3:
		case D3DFMT_DXT4:
		case D3DFMT_DXT5:
			divisor = 4;
			multiplier = 16;
			break;

		// UNKNOWN FORMAT!
		default: 
			return 0;
	};


	unsigned bytes = 0;

	// Calculate size:
	for ( int i = 0; i < mipMapLevels; ++i ) {
		tex.GetLevelDesc( i, &sd );
		bytes +=
			max( 1, sd.Width / divisor ) *
			max( 1, sd.Height / divisor ) * multiplier;
	}
	return bytes;
}


ResultCheck::ResultCheck( char const* errorMessage_, char const* messagePrefix_, char const* messagePostfix_ )
:	errorMessage( errorMessage_ ),
	messagePrefix( messagePrefix_ ),
	messagePostfix( messagePostfix_ )
{
}

ResultCheck& ResultCheck ::operator= ( HRESULT result )
{
	if( FAILED(result) )
	{
		THROW_DXERROR( result,
			std::string( messagePrefix ) +
			std::string( errorMessage ) +
			std::string( messagePostfix ) );
	}

	return *this;
}