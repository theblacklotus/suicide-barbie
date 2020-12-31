#include "../library/Mirror.h"

#include "pspPlatform.h"
#include "pspCommonEffectImpl.h"

using namespace mutalisk;
using namespace mutalisk::effects;

MatrixT gTexProjMatrix;
void mutalisk::effects::setTexProjMatrix(MatrixT const& m)
{
	gTexProjMatrix = m;
}

struct Mirror::Impl : public CommonEffectImpl
{
	PassInfo	passInfo;

	void setupAmbientOnly()
	{
		sceGuAmbient(~0U);
		sceGuAmbientColor(~0U);
		sceGuDisable(GU_LIGHTING);
	}
};

Mirror::Mirror()
:	mImpl(new Impl())
{
}

Mirror::~Mirror()
{
}

void Mirror::begin()
{
	mImpl->begin();
}

unsigned Mirror::passCount(Input const& i)
{
	return 1;
}

BaseEffect::PassInfo const& Mirror::passInfo(Input const& i, unsigned passIndex)
{
	return mImpl->passInfo;
}

void Mirror::pass(Input const& i, unsigned passIndex)
{
	unsigned fxPass = std::min(1U, passIndex); 
	if(mImpl->passIndex != fxPass)
		mImpl->pass(fxPass);

	mImpl->setupSurface(i);
	mImpl->setupGeometry(i);
	mImpl->setupBuffers(i);
	mImpl->setupAmbientOnly();

/*

//	sceGumMatrixMode(GU_MODEL);
//	sceGumLoadIdentity();	// 0

	ScePspFMatrix4 objTexProjMatrix;
	ScePspFMatrix4 w = i.matrices[BaseEffect::WorldMatrix];
	ScePspFMatrix4 iw;
	gumFastInverse(&iw, &w);
	gumMultMatrix(&objTexProjMatrix, &gTexProjMatrix, &w);
//	objTexProjMatrix = gTexProjMatrix;

	sceGuTexMapMode(GU_TEXTURE_MATRIX, 0, 0);
	sceGuTexProjMapMode(GU_POSITION);

	sceGuSetMatrix(GU_MODEL, &w);
	sceGuSetMatrix(GU_TEXTURE, &objTexProjMatrix);

	sceGuTexWrap(GU_REPEAT, GU_REPEAT);
*/

//	sceGuAmbient(0x00ffffff);
//	sceGuColor(i.surface->emissive);
//	sceGuSendCommandi(88, (unsigned int)((1.0f-i.surface->transparency) * 255.0f));

	sceGuEnable(GU_STENCIL_TEST);
	sceGuStencilFunc(GU_ALWAYS, 1, 1);
	sceGuStencilOp(GU_KEEP, GU_KEEP, GU_REPLACE);
	sceGuColorFunc(GU_NEVER, 0, 0xffffff);
	sceGuBlendFunc(GU_ADD, GU_FIX, GU_FIX, 0, ~0U);
	sceGuEnable(GU_BLEND);

	mImpl->commit();
}

void Mirror::captureState()
{
	mImpl->captureState();
}

void Mirror::end()
{
	mImpl->end();
	mImpl->restoreState();

//	sceGuTexMapMode( GU_TEXTURE_COORDS, 0, 0 );
	sceGuDisable(GU_STENCIL_TEST);
	sceGuColorFunc(GU_ALWAYS, 0, 0);
	sceGuDisable(GU_BLEND);
}
