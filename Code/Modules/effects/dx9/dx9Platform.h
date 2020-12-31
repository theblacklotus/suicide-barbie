#ifndef MUTALISK_EFFECTS__DX9_PLATFORM_H_
#define MUTALISK_EFFECTS__DX9_PLATFORM_H_

#include "../cfg.h"
#include <d3d9.h>
#include <d3dx9.h>
#include <mutalisk/dx9/dx9Platform.h>
#include <mutalisk/scene.h>

namespace mutalisk { namespace effects {

	struct Context {
		IDirect3DDevice9*	device;
		ID3DXEffect*		uberShader;
	};
	extern Context gContext;

	typedef mutalisk::data::scene::Light	LightT;
	typedef IDirect3DBaseTexture9			TextureT;
	typedef D3DXMATRIX						MatrixT;
	typedef D3DXVECTOR4						VecT;
	typedef D3DXCOLOR						ColorT;

} // namespace effects 
} // namespace mutalisk

#endif // MUTALISK_EFFECTS__DX9_PLATFORM_H_