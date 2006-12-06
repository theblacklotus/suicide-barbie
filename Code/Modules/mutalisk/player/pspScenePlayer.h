#ifndef NEWAGE_PSP_SCENEPLAYER_H_
#define NEWAGE_PSP_SCENEPLAYER_H_

#include "cfg.h"
#include <memory>

#include <pspgu.h>
#include <pspgum.h>

#include <mutalisk/psp/pspPlatform.h>
#include <mutalisk/mutalisk.h>

#include "animator/Animators.h"
#include "animator/AnimatorAlgos.h"

#ifndef AP
#define AP_DEFINED_LOCALY
#define AP std::auto_ptr
#endif

//struct Mesh
//{
//};

struct Effect
{
};

struct RenderContext
{
	Effect*			defaultEffect;
	ScePspFMatrix4	viewProjMatrix;
	ScePspFMatrix4	projMatrix;
};

struct RenderableMesh
{
	RenderableMesh(mutalisk::data::mesh const& blueprint) : mBlueprint(blueprint) { delete[] mAmplifiedVertexData; }
	mutalisk::data::mesh const&			mBlueprint;
	unsigned char*						mAmplifiedVertexData;

private:
	RenderableMesh(RenderableMesh const& c);
	RenderableMesh& operator= (RenderableMesh const& c);
};

struct RenderableScene;
AP<RenderableScene> prepare(RenderContext& rc, mutalisk::data::scene const& data, std::string const& pathPrefix = "");
AP<RenderableMesh> prepare(RenderContext& rc, mutalisk::data::mesh const& data);
void render(RenderContext& rc, RenderableScene const& scene,
	bool animatedActors = true, bool animatedLights = true, int maxActors = -1, int maxLights = -1);


#ifdef AP_DEFINED_LOCALY
#undef AP
#endif

#endif // NEWAGE_PSP_SCENEPLAYER_H_
