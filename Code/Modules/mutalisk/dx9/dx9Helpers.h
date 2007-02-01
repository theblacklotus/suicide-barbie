#ifndef __DX_HELPERS_H
#define __DX_HELPERS_H

#include "cfg.h"
#include <d3dx9.h>

namespace dx {

template< typename T >
IDirect3DDevice9& getDevice( T& o )
{
	IDirect3DDevice9* device = NULL;
	o.GetDevice( &device );
	ASSERT( device );

	return *device;
}

template< typename T >
void release( T& o )
{
	if( o )
		o->Release();
	o = NULL;
}

template< typename T >
void assertRelease( T& o )
{
	ASSERT( o );
	o->Release();
	o = NULL;
}

unsigned calcTextureSize(IDirect3DTexture9 const& texture);
unsigned calcTextureSize(IDirect3DCubeTexture9 const& texture);
unsigned calcTextureSize(IDirect3DVolumeTexture9 const& texture);

class ResultCheck
{
public:
	ResultCheck( char const* errorMessage_ = "execute DirectX function",
		char const* messagePrefix_ = "Failed to ",
		char const* messagePostfix_ = "." );

	ResultCheck& operator= (HRESULT result);

private:
	char const*	errorMessage;
	char const*	messagePrefix;
	char const*	messagePostfix;
};

}; // namespace dx

#ifndef DX_MSG
#define DX_MSG(msg) dx::ResultCheck(msg)
#endif

#ifndef DX_CHK
#define DX_CHK dx::ResultCheck()
#endif

#endif