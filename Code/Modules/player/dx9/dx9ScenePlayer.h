#ifndef NEWAGE_DX9_SCENEPLAYER_H_
#define NEWAGE_DX9_SCENEPLAYER_H_

#include "cfg.h"
#include <memory>
#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr.h>

#include "dx9Platform.h"
#include <mutalisk/mutalisk.h>

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
	D3DXMATRIX					viewMatrix;
	D3DXMATRIX					projMatrix;
	D3DXMATRIX					viewProjMatrix;
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

struct Dx9RenderableScene;
AP<Dx9RenderableScene> prepare(RenderContext& rc, mutalisk::data::scene const& data);
AP<RenderableMesh> prepare(RenderContext& rc, mutalisk::data::mesh const& data);
//AP<RenderableTexture> prepare(RenderContext& rc, mutalisk::data::texture const& data);
void update(Dx9RenderableScene& scene, float deltaTime);
void process(Dx9RenderableScene& scene);
void render(RenderContext& rc, Dx9RenderableScene const& scene, bool animatedActors = true, bool animatedCamera = true, int maxActors = -1);


#ifdef AP_DEFINED_LOCALY
#undef AP
#endif

#endif // NEWAGE_DX9_SCENEPLAYER_H_
