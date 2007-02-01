#include "dx9NullDevice.h"

IDirect3DDevice9* createDx9NullDevice()
{
    HRESULT hr;
    IDirect3D9* pD3D = Direct3DCreate9( D3D_SDK_VERSION );
    if( NULL == pD3D )
        return NULL;

    D3DDISPLAYMODE Mode;
    pD3D->GetAdapterDisplayMode(0, &Mode);

    D3DPRESENT_PARAMETERS pp;
    ZeroMemory( &pp, sizeof(D3DPRESENT_PARAMETERS) ); 
    pp.BackBufferWidth  = 1;
    pp.BackBufferHeight = 1;
    pp.BackBufferFormat = Mode.Format;
    pp.BackBufferCount  = 1;
    pp.SwapEffect       = D3DSWAPEFFECT_COPY;
    pp.Windowed         = TRUE;

    IDirect3DDevice9* pd3dDevice;
    hr = pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_NULLREF, GetDesktopWindow(), 
                             D3DCREATE_HARDWARE_VERTEXPROCESSING, &pp, &pd3dDevice );
    if( pD3D )
		pD3D->Release();
    if( FAILED(hr) || pd3dDevice == NULL )
        return NULL;

    return pd3dDevice;
}
