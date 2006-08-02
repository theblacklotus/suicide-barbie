/*
 * Test of the Screen class
 */
#include <Modules/mutant/mutant/reader.h>
#include <Modules/mutant/mutant/io_factory.h>

extern "C" {
	#include <Base/Math/Math.h>
	#include <Base/Std/Std.h>
	#include <Base/Math/Lin.h>
	#include <Base/Math/Quat.h>
}


#include <pspkernel.h>
#include <pspdisplay.h>
#include <psprtc.h>
#include <pspdebug.h>

#include <pspctrl.h>
#include <pspgu.h>
#include <pspgum.h>

#include "callbacks.h"
#include "vram.h"

#include "animator/Animators.h"
#include "animator/AnimatorAlgos.h"

PSP_MODULE_INFO("HardCorePlayHCScene", 0x1000, 1, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER);

static unsigned int __attribute__((aligned(16))) list[262144];
extern unsigned char logo_start[];


struct Vertex
{
	float u, v;
	unsigned int color;
	float x,y,z;
};

struct Vertex __attribute__((aligned(16))) vertices[12*3] =
{
	{0, 0, 0xff7f0000,-1,-1, 1}, // 0
	{1, 0, 0xff7f0000,-1, 1, 1}, // 4
	{1, 1, 0xff7f0000, 1, 1, 1}, // 5

	{0, 0, 0xff7f0000,-1,-1, 1}, // 0
	{1, 1, 0xff7f0000, 1, 1, 1}, // 5
	{0, 1, 0xff7f0000, 1,-1, 1}, // 1

	{0, 0, 0xff7f0000,-1,-1,-1}, // 3
	{1, 0, 0xff7f0000, 1,-1,-1}, // 2
	{1, 1, 0xff7f0000, 1, 1,-1}, // 6

	{0, 0, 0xff7f0000,-1,-1,-1}, // 3
	{1, 1, 0xff7f0000, 1, 1,-1}, // 6
	{0, 1, 0xff7f0000,-1, 1,-1}, // 7

	{0, 0, 0xff007f00, 1,-1,-1}, // 0
	{1, 0, 0xff007f00, 1,-1, 1}, // 3
	{1, 1, 0xff007f00, 1, 1, 1}, // 7

	{0, 0, 0xff007f00, 1,-1,-1}, // 0
	{1, 1, 0xff007f00, 1, 1, 1}, // 7
	{0, 1, 0xff007f00, 1, 1,-1}, // 4

	{0, 0, 0xff007f00,-1,-1,-1}, // 0
	{1, 0, 0xff007f00,-1, 1,-1}, // 3
	{1, 1, 0xff007f00,-1, 1, 1}, // 7

	{0, 0, 0xff007f00,-1,-1,-1}, // 0
	{1, 1, 0xff007f00,-1, 1, 1}, // 7
	{0, 1, 0xff007f00,-1,-1, 1}, // 4

	{0, 0, 0xff00007f,-1, 1,-1}, // 0
	{1, 0, 0xff00007f, 1, 1,-1}, // 1
	{1, 1, 0xff00007f, 1, 1, 1}, // 2

	{0, 0, 0xff00007f,-1, 1,-1}, // 0
	{1, 1, 0xff00007f, 1, 1, 1}, // 2
	{0, 1, 0xff00007f,-1, 1, 1}, // 3

	{0, 0, 0xff00007f,-1,-1,-1}, // 4
	{1, 0, 0xff00007f,-1,-1, 1}, // 7
	{1, 1, 0xff00007f, 1,-1, 1}, // 6

	{0, 0, 0xff00007f,-1,-1,-1}, // 4
	{1, 1, 0xff00007f, 1,-1, 1}, // 6
	{0, 1, 0xff00007f, 1,-1,-1}, // 5
};

#define BUF_WIDTH (512)
#define SCR_WIDTH (480)
#define SCR_HEIGHT (272)

std::auto_ptr<mutant::anim_character_set> mutantTest()
{
	std::string fileName = "ms0:PSP/MUSIC/mutant1.man";

	std::auto_ptr<mutant::binary_input> input = mutant::reader_factory::createInput( fileName );
	mutant::mutant_reader mutReader( input );
	mutReader.enableLog( false );

	std::auto_ptr<mutant::anim_character_set> mutCharSet( new mutant::anim_character_set );
	mutReader.read( *mutCharSet );

	return mutCharSet;
}

std::auto_ptr<mutant::simple_skinned> skinTest()
{
	std::string fileName = "ms0:PSP/MUSIC/mutant1.msh";

	std::auto_ptr<mutant::binary_input> input = mutant::reader_factory::createInput( fileName );
	mutant::mutant_reader mutReader( input );
	mutReader.enableLog( false );

	std::auto_ptr<mutant::simple_skinned> mutSkinned( new mutant::simple_skinned );
	mutReader.read( *mutSkinned );

	return mutSkinned;
}


//Vertex __attribute__((aligned(16))) cylinder_vertices[CYLINDER_SLICES*CYLINDER_ROWS];
//unsigned short __attribute__((aligned(16))) cylinder_indices[CYLINDER_SLICES*(CYLINDER_ROWS-1)*6];
void skinToBuffers( mutant::simple_skinned& skin, void** vbuffer, void** ibuffer )
{
	*vbuffer = malloc( skin.vertexCount * sizeof(mutant::simple_skinned::Vec3) );
	memcpy( *vbuffer, skin.positions, skin.vertexCount * sizeof(mutant::simple_skinned::Vec3) );
//	*vbuffer = (void*)skin.positions; // copy
	*ibuffer = (void*)skin.indices;
}

void skinToBuffers2( mutant::simple_skinned& skin, void** vbuffer, void** ibuffer )
{
	*vbuffer = malloc( skin.vertexCount * sizeof(mutant::simple_skinned::Vec3)*2 );
//	memchr( *vbuffer, 0, skin.vertexCount * sizeof(mutant::simple_skinned::Vec3)*2 );
	*ibuffer = (void*)skin.indices;
}

typedef std::vector<std::pair<int, int> > BoneMapT;
BoneMapT& mapSkinnedBonesToHierarchy(
	mutant::simple_skinned::Bone* bones, size_t boneCount,
	mutant::anim_hierarchy const& hierarchy,
	BoneMapT& remapedBones )
{
	remapedBones.reserve( remapedBones.size() + boneCount );

	unsigned boneId = 0;
	for( size_t q = 0; q < boneCount; ++q )
	{
		std::string const& boneName = bones[q].name;

		while( hierarchy[ boneId ].name != boneName && boneId < hierarchy.size() )
			++boneId;

		if( hierarchy[ boneId ].name != boneName )
			printf( "Couldn't find '%s' in hierarchy", boneName.c_str() );

		remapedBones.push_back( std::make_pair( q, boneId ) );
	}

	return remapedBones;
}

void updateSkinMesh( mutant::simple_skinned& skin, BoneMapT& boneMap, CTransform::t_matrix const* data, float* vbuffer, size_t stride )
{
	static std::vector<CTransform::t_matrix> worldMatrices;
	worldMatrices.resize( boneMap.size() );

	unsigned i = 0;
	BoneMapT::iterator bIdIt = boneMap.begin();
	for( ; (i < boneMap.size()) && (bIdIt != boneMap.end()); ++bIdIt, ++i )
	{
		float* mat16 = skin.bones[ bIdIt->first ].matrix.data;
		CTransform::t_matrix tm( // new-age wants transposed matrix
			mat16[0], mat16[4], mat16[8],
			mat16[1], mat16[5], mat16[9],
			mat16[2], mat16[6], mat16[10],
			mat16[12], mat16[13], mat16[14]
		);
		Mat34_mul( &worldMatrices[i], (Mat34*)&data[ bIdIt->second ], &tm );
		worldMatrices[i] = worldMatrices[i];
	}

	while( i < worldMatrices.size() )
		Mat34_setIdentity( &worldMatrices[i++] );

	for( size_t q = 0; q < skin.vertexCount; ++q )
	{
		CTransform::t_vector pos3( skin.positions[q].data[0], skin.positions[q].data[1], skin.positions[q].data[2] );
		CTransform::t_vector accum3( 0.0f, 0.0f, 0.0f );
		float accumWeight = 0.0f;
		const size_t weightsPerVertex = skin.weightsPerVertex;
		for( size_t w = 0; w < weightsPerVertex; ++w )
		{
			unsigned short boneId = skin.boneIndices[q * skin.weightsPerVertex + w];
			float boneWeight = skin.weights[q * skin.weightsPerVertex + w];
			if( w != weightsPerVertex - 1 )
				accumWeight += boneWeight;
			else
				boneWeight = 1.0f - accumWeight;

			Vec3 v3;
			Vec3_setMat34MulVec3( &v3, &worldMatrices[boneId], &pos3 );
			Vec3_scale( &v3, &v3, boneWeight );
			Vec3_add( &accum3, &accum3, &v3 );
		}

		vbuffer[0] = accum3.x;
		vbuffer[1] = accum3.y;
		vbuffer[2] = accum3.z;
		vbuffer = (float*)((u8*)vbuffer + stride);
	}
}

void updateSkinMesh2( mutant::simple_skinned& skin, BoneMapT& boneMap, CTransform::t_matrix const* data, float* vbuffer, size_t stride, Vec3 spriteR )
{
	static std::vector<CTransform::t_matrix> worldMatrices;
	worldMatrices.resize( boneMap.size() );

	unsigned i = 0;
	BoneMapT::iterator bIdIt = boneMap.begin();
	for( ; (i < boneMap.size()) && (bIdIt != boneMap.end()); ++bIdIt, ++i )
	{
		float* mat16 = skin.bones[ bIdIt->first ].matrix.data;
		CTransform::t_matrix tm( // new-age wants transposed matrix
			mat16[0], mat16[4], mat16[8],
			mat16[1], mat16[5], mat16[9],
			mat16[2], mat16[6], mat16[10],
			mat16[12], mat16[13], mat16[14]
		);
		Mat34_mul( &worldMatrices[i], (Mat34*)&data[ bIdIt->second ], &tm );
		worldMatrices[i] = worldMatrices[i];
	}

	while( i < worldMatrices.size() )
		Mat34_setIdentity( &worldMatrices[i++] );

	for( size_t q = 0; q < skin.vertexCount; ++q )
	{
		CTransform::t_vector pos3( skin.positions[q].data[0], skin.positions[q].data[1], skin.positions[q].data[2] );
		CTransform::t_vector accum3( 0.0f, 0.0f, 0.0f );
		float accumWeight = 0.0f;
		const size_t weightsPerVertex = skin.weightsPerVertex;
		for( size_t w = 0; w < weightsPerVertex; ++w )
		{
			unsigned short boneId = skin.boneIndices[q * skin.weightsPerVertex + w];
			float boneWeight = skin.weights[q * skin.weightsPerVertex + w];
			if( w != weightsPerVertex - 1 )
				accumWeight += boneWeight;
			else
				boneWeight = 1.0f - accumWeight;

			Vec3 v3;
			Vec3_setMat34MulVec3( &v3, &worldMatrices[boneId], &pos3 );
			Vec3_scale( &v3, &v3, boneWeight );
			Vec3_add( &accum3, &accum3, &v3 );
		}

		vbuffer[0] = accum3.x - spriteR.x;
		vbuffer[1] = accum3.y - spriteR.y;
		vbuffer[2] = accum3.z - spriteR.z;

		vbuffer[3] = accum3.x + spriteR.x;
		vbuffer[4] = accum3.y + spriteR.y;
		vbuffer[5] = accum3.z + spriteR.z;

		vbuffer = (float*)((u8*)vbuffer + stride*2);
	}
}


int main(int argc, char* argv[])
{
//	logSetMessageThreshold(5);

	setupCallbacks();

	// setup GU

	void* fbp0 = getStaticVramBuffer(BUF_WIDTH,SCR_HEIGHT,GU_PSM_8888);
	void* fbp1 = getStaticVramBuffer(BUF_WIDTH,SCR_HEIGHT,GU_PSM_8888);
	void* zbp = getStaticVramBuffer(BUF_WIDTH,SCR_HEIGHT,GU_PSM_4444);

	pspDebugScreenInit();
	sceGuInit();

	sceGuStart(GU_DIRECT,list);
	sceGuDrawBuffer(GU_PSM_8888,fbp0,BUF_WIDTH);
	sceGuDispBuffer(SCR_WIDTH,SCR_HEIGHT,fbp1,BUF_WIDTH);
	sceGuDepthBuffer(zbp,BUF_WIDTH);
	sceGuOffset(2048 - (SCR_WIDTH/2),2048 - (SCR_HEIGHT/2));
	sceGuViewport(2048,2048,SCR_WIDTH,SCR_HEIGHT);
	sceGuDepthRange(65535,0);
	sceGuScissor(0,0,SCR_WIDTH,SCR_HEIGHT);
	sceGuEnable(GU_SCISSOR_TEST);
	sceGuDepthFunc(GU_GEQUAL);
	sceGuEnable(GU_DEPTH_TEST);
	sceGuFrontFace(GU_CW);
	sceGuShadeModel(GU_SMOOTH);
	sceGuEnable(GU_CULL_FACE);
	sceGuEnable(GU_TEXTURE_2D);
	sceGuEnable(GU_CLIP_PLANES);
	sceGuFinish();
	sceGuSync(0,0);

	sceDisplayWaitVblankStart();
	sceGuDisplay(GU_TRUE);

	// run sample

	int val = 0;


	std::auto_ptr<mutant::anim_character_set> animCharSet = mutantTest();
	mutant::anim_character& animChar = (*animCharSet)["Mutant"];
	mutant::anim_hierarchy& charHierarchy = animChar.hierarchy( mutant::sTypeNames::HIERARCHY_DEFAULT ) ;
	CTransformArrayAnimator xformArrayAnimator;
	xformArrayAnimator.createFromClip(
		animChar["attack1"], charHierarchy );

	std::auto_ptr<mutant::simple_skinned> skin = skinTest();

	float* vbuffer = 0;
	float* vbuffer2 = 0;
	void* ibuffer = 0;
	skinToBuffers( *skin, (void**)&vbuffer, &ibuffer );
	skinToBuffers2( *skin, (void**)&vbuffer2, &ibuffer );

	BoneMapT boneMap;
	mapSkinnedBonesToHierarchy( skin->bones, skin->boneCount, charHierarchy, boneMap );


	typedef std::vector<CTransform> TransformsT;
	typedef std::vector<CTransform::t_matrix> MatricesT;
	TransformsT transforms;
	MatricesT matrices;
	transforms.resize( charHierarchy.size(), CTransform::identity() );
	matrices.resize( charHierarchy.size() );

	CTransform worldTransform = CTransform::identity();

	SceCtrlData oldPad;
	oldPad.Buttons = 0;
	sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode(0); 

	while(running())
	{
		SceCtrlData pad;
		if(sceCtrlPeekBufferPositive(&pad, 1))
		{
//			if (pad.Buttons != oldPad.Buttons)
			{
				if (pad.Buttons & PSP_CTRL_UP)
					worldTransform.translateBy( CTransform::t_vector( 0, 0.1f, 0.0f ) );
				if (pad.Buttons & PSP_CTRL_DOWN)
					worldTransform.translateBy( CTransform::t_vector( 0, -0.1f, 0.0f ) );
				if (pad.Buttons & PSP_CTRL_LEFT)
					worldTransform.translateBy( CTransform::t_vector( -0.1f, 0.0f, 0.0f ) );
				if (pad.Buttons & PSP_CTRL_RIGHT)
					worldTransform.translateBy( CTransform::t_vector( 0.1f, 0.0f, 0.0f ) );
				if (pad.Buttons & PSP_CTRL_LTRIGGER)
					worldTransform.translateBy( CTransform::t_vector( 0.0f, 0.0f, -0.1f ) );
				if (pad.Buttons & PSP_CTRL_RTRIGGER)
					worldTransform.translateBy( CTransform::t_vector( 0.0f, 0.0f, 0.1f ) );
				if (pad.Buttons & PSP_CTRL_CROSS)
					worldTransform.identify();
			}
			oldPad = pad;
		}

		sceGuStart(GU_DIRECT,list);

		// clear screen

		sceGuClearColor(0xff554433);
		sceGuClearDepth(0);
		sceGuClear(GU_COLOR_BUFFER_BIT|GU_DEPTH_BUFFER_BIT);

		// setup matrices for cube

		sceGumMatrixMode(GU_PROJECTION);
		sceGumLoadIdentity();
		sceGumPerspective(75.0f,16.0f/9.0f,0.5f,1000.0f);

		sceGumMatrixMode(GU_VIEW);
		sceGumLoadIdentity();

		sceGumMatrixMode(GU_MODEL);
		sceGumLoadIdentity();
/*		{
			ScePspFVector3 pos = { 0, 0, -2.5f };
			ScePspFVector3 rot = { val * 0.79f * (GU_PI/180.0f), val * 0.98f * (GU_PI/180.0f), val * 1.32f * (GU_PI/180.0f) };
			sceGumTranslate(&pos);
			sceGumRotateXYZ(&rot);
		}
*/
		// setup texture

		sceGuTexMode(GU_PSM_4444,0,0,0);
		sceGuTexImage(0,64,64,64,logo_start);
		sceGuTexFunc(GU_TFX_ADD,GU_TCC_RGB);
		sceGuTexEnvColor(0xffff00);
		sceGuTexFilter(GU_LINEAR,GU_LINEAR);
		sceGuTexScale(1.0f,1.0f);
		sceGuTexOffset(0.0f,0.0f);
		sceGuAmbientColor(0xffffffff);

		// draw cube

//		sceGumDrawArray(GU_TRIANGLES,GU_TEXTURE_32BITF|GU_COLOR_8888|GU_VERTEX_32BITF|GU_TRANSFORM_3D,12*3,0,vertices);

/*		ScePspFVector3 pos = { worldTransform.translation().x, worldTransform.translation().y, worldTransform.translation().z };
		sceGumTranslate(&pos);

		sceGumDrawArray(
			GU_TRIANGLES,
			GU_VERTEX_32BITF|GU_TRANSFORM_3D|GU_INDEX_16BIT,
			skin->indexCount,
			ibuffer, vbuffer );*/


		pspDebugScreenSetOffset((int)fbp0);
		pspDebugScreenSetXY(0,0);
		pspDebugScreenPrintf("Hello world!");
		pspDebugScreenPrintf("Cubbie is rotating");

		pspDebugScreenPrintf("\n");
		pspDebugScreenPrintf("skin: %d, %d, %d, %d\n", skin->vertexCount, skin->indexCount, skin->boneCount, skin->weightsPerVertex);


		if( animCharSet.get() )
		{
			pspDebugScreenPrintf("\n*) ");
			for( size_t q = 0; q < animCharSet->size(); ++q )
			{
				pspDebugScreenPrintf(" ");
				pspDebugScreenPrintf(animCharSet->charPair(q).first.c_str());
			}

			pspDebugScreenPrintf("\n*) ");
			mutant::anim_character& animChar = (*animCharSet)["Mutant"];
			for( size_t q = 0; q < animChar.size(); ++q )
			{
				pspDebugScreenPrintf(" ");
				pspDebugScreenPrintf(animChar.clipPair(q).first.c_str());
			}

			xformArrayAnimator.updateTransforms( (float)val * 0.03f,
				transforms.begin(), transforms.end() );

			CAnimatorAlgos::transformHierarchy(
				matrices.begin(), matrices.end(),
				transforms.begin(), charHierarchy, worldTransform );

			pspDebugScreenPrintf(" \n");
			for( size_t w = 0; w < 9 && w < transforms.size(); ++w )
			{
/*				pspDebugScreenPrintf("o: %f %f %f --- r: %f %f %f %f\n",
					transforms[w].translation().x, transforms[w].translation().y, transforms[w].translation().z,
					transforms[w].rotation().x, transforms[w].rotation().y, transforms[w].rotation().z, transforms[w].rotation().w
					);*/

/*				pspDebugScreenPrintf("o: %f %f %f\n",
					matrices[w].Move.x, matrices[w].Move.y, matrices[w].Move.z );*/
			}

			pspDebugScreenPrintf("o: %f %f %f\n",
				worldTransform.translation().x, worldTransform.translation().y, worldTransform.translation().z );
//				matrices[w].Move.x, matrices[w].Move.y, matrices[w].Move.z );

			for( MatricesT::iterator i = matrices.begin(); i != matrices.end(); ++i )
			{
				float scale = 0.05f;
				ScePspFMatrix4 m;
				m.x.x = i->Rot.Row[0].x * scale;
				m.x.y = i->Rot.Row[0].y * scale;
				m.x.z = i->Rot.Row[0].z * scale;
				m.x.w = 0.0f;
				m.y.x = i->Rot.Row[1].x * scale;
				m.y.y = i->Rot.Row[1].y * scale;
				m.y.z = i->Rot.Row[1].z * scale;
				m.y.w = 0.0f;
				m.z.x = i->Rot.Row[2].x * scale;
				m.z.y = i->Rot.Row[2].y * scale;
				m.z.z = i->Rot.Row[2].z * scale;
				m.z.w = 0.0f;
				m.w.x = i->Move.x;
				m.w.y = i->Move.y;
				m.w.z = i->Move.z;
				m.w.w = 1.0f;

/*				m.x.x = scale;
				m.x.y = 0;
				m.x.z = 0;
				m.x.w = 0.0f;
				m.y.x = 0;
				m.y.y = scale;
				m.y.z = 0;
				m.y.w = 0.0f;
				m.z.x = 0;
				m.z.y = 0;
				m.z.z = scale;
				m.z.w = 0.0f;
*/

				sceGumLoadIdentity();
				sceGumLoadMatrix( &m );

				sceGumDrawArray(GU_TRIANGLES,GU_TEXTURE_32BITF|GU_COLOR_8888|GU_VERTEX_32BITF|GU_TRANSFORM_3D,12*3,0,vertices);
			}

			sceGumLoadIdentity();

//			ScePspFVector3 pos = { worldTransform.translation().x, worldTransform.translation().y, worldTransform.translation().z };
//			sceGumTranslate(&pos);

//			updateSkinMesh( *skin, boneMap, &*matrices.begin(), vbuffer, sizeof(float)*3 );
			updateSkinMesh2( *skin, boneMap, &*matrices.begin(), vbuffer2, sizeof(float)*3, xVec3( 0.03f, 0.03f, 0.0f ) );
/*			sceGumDrawArray(
				GU_TRIANGLES,
				GU_VERTEX_32BITF|GU_TRANSFORM_3D|GU_INDEX_16BIT,
				skin->indexCount,
				ibuffer, vbuffer );
*/
			sceGumDrawArray(
				GU_SPRITES,
				GU_VERTEX_32BITF|GU_TRANSFORM_3D,
				skin->vertexCount,
				0, vbuffer2 );



/*			pspDebugScreenPrintf("\n*) ");
			mutant::anim_clip& animClip = animChar["idle1"];
			for( size_t q = 0; q < animClip.size(); ++q )
			{
				pspDebugScreenPrintf(" ");
				pspDebugScreenPrintf(anim_bundle.clipPair(q).first.c_str());
				
			}*/
		}



		sceGuFinish();
		sceGuSync(0,0);

		sceDisplayWaitVblankStart();
		fbp1 = fbp0;
		fbp0 = sceGuSwapBuffers();

		val++;
	}

	sceGuTerm();

	sceKernelExitGame();
	return 0;
}
/*
int main(void)
{
	logSetMessageThreshold(5);


	if (!(ChunkyBuffer = ChunkyBitMap_create(320, 200)))
	{
		logError(8, "Unable to create chunkybuffer");
		sysExit();
		return 0;
	}

	if (Screen_create(320, 256, 8, 2))
	{
		MouseInputEvent event;

		Screen_setPalette(gTopScreen, &pal);

		while (1) // *((uint8*) 0xbfe001) & 0x40)
		{
			Input_getMouseStatus(gInputHandler, &event);
			if (event.Buttons & Input_LMB)
				break;

			stdClearMem(ChunkyBuffer->Pixels, ChunkyBuffer->Pitch * ChunkyBuffer->Height);

			drawtridi();

			BitMap_blit(gTopScreen->Back, (BitMap*) ChunkyBuffer, 0, 0, ChunkyBuffer->Width, ChunkyBuffer->Height,
				ROUNDDOWN2((gTopScreen->Width - ChunkyBuffer->Width) / 2, 32), (gTopScreen->Height - ChunkyBuffer->Height) / 2);

			Screen_flip(gTopScreen);
		}

		Screen_destroy(gTopScreen);
	}

	ChunkyBitMap_destroy(ChunkyBuffer);

	sysExit();

	return 0;	
}
*/
