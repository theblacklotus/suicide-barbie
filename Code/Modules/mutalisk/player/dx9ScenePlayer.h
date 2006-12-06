#ifndef NEWAGE_DX9_SCENEPLAYER_H_
#define NEWAGE_DX9_SCENEPLAYER_H_

#include "cfg.h"
#include <memory>
#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr.h>

#include <mutalisk/dx9/dx9Platform.h>
#include <mutalisk/mutalisk.h>

#include "comptr.h"
#include "dxHelpers.h"
#include "animator/Animators.h"
#include "animator/AnimatorAlgos.h"

#ifndef AP
#define AP_DEFINED_LOCALY
#define AP std::auto_ptr
#endif


struct RenderContext
{
	com_ptr<IDirect3DDevice9>	device;
	com_ptr<ID3DXEffect>		defaultEffect;
	D3DXMATRIX					viewProjMatrix;
	D3DXMATRIX					projMatrix;
};

struct RenderableMesh
{
	RenderableMesh(mutalisk::data::mesh const& blueprint) : mBlueprint(blueprint) {}
	mutalisk::data::mesh const&			mBlueprint;
	com_ptr<ID3DXMesh>					mNative;

private:
	RenderableMesh(RenderableMesh const& c);
	RenderableMesh& operator= (RenderableMesh const& c);
};

struct RenderableScene;
AP<RenderableScene> prepare(RenderContext& rc, mutalisk::data::scene const& data);
AP<RenderableMesh> prepare(RenderContext& rc, mutalisk::data::mesh const& data);
void render(RenderContext& rc, RenderableScene const& scene, bool animatedActors = true, bool animatedCamera = true, int maxActors = -1);


#ifdef AP_DEFINED_LOCALY
#undef AP
#endif

#endif // NEWAGE_DX9_SCENEPLAYER_H_
