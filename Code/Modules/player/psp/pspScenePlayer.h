#ifndef NEWAGE_PSP_SCENEPLAYER_H_
#define NEWAGE_PSP_SCENEPLAYER_H_

#include "../cfg.h"
#include <memory>

#include <pspgu.h>
#include <pspgum.h>

#include <mutalisk/psp/pspPlatform.h>
#include <mutalisk/mutalisk.h>

#include "../Animators.h"
#include "../AnimatorAlgos.h"

namespace mutalisk
{
	extern bool gDelayedTextureLoading;

#ifndef AP
#define AP_DEFINED_LOCALY
#define AP std::auto_ptr
#endif

//struct Mesh
//{
//};

struct RenderContext
{
	RenderContext();
	ScePspFMatrix4	viewMatrix;
	ScePspFMatrix4	projMatrix;
	ScePspFMatrix4	viewProjMatrix;

	float znear;
	float zfar;
};

struct RenderableMesh
{
	RenderableMesh(mutalisk::data::mesh const& blueprint)
		: mBlueprint(blueprint), mAmplifiedVertexDecl(0), mAmplifiedBufferIndex(0), mUserData(0) {
		mAmplifiedVertexData[0] = 0;
		mAmplifiedVertexData[1] = 0; }
	~RenderableMesh() {
		delete[] mAmplifiedVertexData[0];
		delete[] mAmplifiedVertexData[1];
		delete[] mUserData; }
	mutalisk::data::mesh const&			mBlueprint;
	unsigned char*						mAmplifiedVertexData[2];
	int									mAmplifiedVertexDecl;
	unsigned							mAmplifiedVertexStride;
	unsigned							mAmplifiedBufferIndex;
	unsigned char*						mUserData;

private:
	RenderableMesh(RenderableMesh const& c);
	RenderableMesh& operator= (RenderableMesh const& c);
};

struct RenderableTexture
{
	RenderableTexture(mutalisk::data::texture const& blueprint)
	: mBlueprint(blueprint)
	{
	}

	mutalisk::data::texture const& mBlueprint;

private:
	RenderableTexture(RenderableTexture const& c);
	RenderableTexture& operator= (RenderableTexture const& c);
};

struct RenderableScene;
AP<RenderableScene> prepare(RenderContext& rc, mutalisk::data::scene const& data, std::string const& pathPrefix = "");
AP<RenderableMesh> prepare(RenderContext& rc, mutalisk::data::mesh const& data);
AP<RenderableTexture> prepare(RenderContext& rc, mutalisk::data::texture const& data);

void render(RenderContext& rc, RenderableScene const& scene, int maxActors = -1);
//	bool animatedActors = true, bool animatedLights = true, int maxActors = -1, int maxLights = -1);

#ifdef AP_DEFINED_LOCALY
#undef AP
#endif
} // namespace mutalisk

#endif // NEWAGE_PSP_SCENEPLAYER_H_
