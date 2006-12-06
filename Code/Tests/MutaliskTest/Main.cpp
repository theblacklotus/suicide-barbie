/*
 * Test of the Screen class
 */
//#include <mutant/reader.h>
//#include <mutant/io_factory.h>

//#include <Base/BitMap/BitMap.h>

#include <pspkernel.h>
#include <pspdisplay.h>
#include <psprtc.h>
#include <pspdebug.h>

#include <pspctrl.h>
#include <pspgu.h>
#include <pspgum.h>

#include "callbacks.h"
#include "vram.h"

#include <animator/Animators.h>
#include <animator/AnimatorAlgos.h>

#include <mutalisk/player/ScenePlayer.h>
#include <mutalisk/player/pspScenePlayer.h>

extern "C" {
	#include <Base/Math/Math.h>
	#include <Base/Std/Std.h>
	#include <Base/Math/Lin.h>
	#include <Base/Math/Quat.h>
}

//#include "Skin.h"

PSP_MODULE_INFO("MutaliskTest", 0x1000, 1, 1);
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


struct ScenePlayerApp
{
	ScenePlayerApp(std::string const& sceneName, std::string const& pathPrefix)
	{
		ScePspFMatrix4 identityMatrix;
		gumLoadIdentity(&identityMatrix);

		renderContext.viewProjMatrix = identityMatrix;
		renderContext.projMatrix = identityMatrix;

		scene.blueprint = loadResource<mutalisk::data::scene>(pathPrefix + sceneName);
		scene.renderable = prepare(renderContext, *scene.blueprint, pathPrefix);
	}

	void setViewProjMatrix(ScePspFMatrix4 const& viewProjMatrix)
	{
		renderContext.viewProjMatrix = viewProjMatrix;
	}

	void setProjMatrix(ScePspFMatrix4 const& projMatrix)
	{
		renderContext.projMatrix = projMatrix;
	}


	void update(float deltaTime) { scene.renderable->update(deltaTime); }
	void process() { scene.renderable->process(); }
	void render(int maxActors = -1, int maxLights = -1) { ::render(renderContext, *scene.renderable, true, true, maxActors, maxLights); }

	struct Scene
	{
		std::auto_ptr<mutalisk::data::scene> blueprint;
		std::auto_ptr<RenderableScene> renderable;
	};
	
	RenderContext	renderContext;
	Scene			scene;
};
std::auto_ptr<ScenePlayerApp> scenePlayerApp;
std::string gSceneFileName = "test_baked.msk";
std::string gPathPrefix = "host1:PSP/TESTDATA/";//"ms0:PSP/TESTDATA/";

/*
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
*/


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
	sceGuFrontFace(GU_CCW);
	sceGuShadeModel(GU_SMOOTH);
	sceGuEnable(GU_CULL_FACE);
	sceGuEnable(GU_TEXTURE_2D);
	sceGuEnable(GU_CLIP_PLANES);

//	sceGuEnable(GU_LIGHTING);
//	sceGuEnable(GU_LIGHT0);

	sceGuFinish();
	sceGuSync(0,0);

	sceDisplayWaitVblankStart();
	sceGuDisplay(GU_TRUE);

	// run sample

	int val = 0;

	printf("ScenePlayer: create\n");
	scenePlayerApp.reset(new ScenePlayerApp(gSceneFileName, gPathPrefix));
	printf("ScenePlayer: created and loaded\n");

/*	std::auto_ptr<mutant::anim_character_set> animCharSet = mutantTest();
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
*/
	CTransform cameraTransform = CTransform::identity();

	SceCtrlData oldPad;
	oldPad.Buttons = 0;
	sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode(0); 

	while(running())
	{
		bool drawCube = false;
		SceCtrlData pad;
		if(sceCtrlPeekBufferPositive(&pad, 1))
		{
//			if (pad.Buttons != oldPad.Buttons)
			{
				if (pad.Buttons & PSP_CTRL_UP)
					cameraTransform.translateBy( CTransform::t_vector( 0, 0.1f, 0.0f ) );
				if (pad.Buttons & PSP_CTRL_DOWN)
					cameraTransform.translateBy( CTransform::t_vector( 0, -0.1f, 0.0f ) );
				if (pad.Buttons & PSP_CTRL_LEFT)
					cameraTransform.translateBy( CTransform::t_vector( -0.1f, 0.0f, 0.0f ) );
				if (pad.Buttons & PSP_CTRL_RIGHT)
					cameraTransform.translateBy( CTransform::t_vector( 0.1f, 0.0f, 0.0f ) );
				if (pad.Buttons & PSP_CTRL_LTRIGGER)
					cameraTransform.translateBy( CTransform::t_vector( 0.0f, 0.0f, -0.1f ) );
				if (pad.Buttons & PSP_CTRL_RTRIGGER)
					cameraTransform.translateBy( CTransform::t_vector( 0.0f, 0.0f, 0.1f ) );
				if (pad.Buttons & PSP_CTRL_CROSS)
					cameraTransform.identify();
				drawCube = (pad.Buttons & PSP_CTRL_SQUARE);
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
		sceGumPerspective(45.0f,16.0f/9.0f,0.5f,1000.0f);

		sceGumMatrixMode(GU_VIEW);
		sceGumLoadIdentity();

		ScePspFVector3 pos = { cameraTransform.translation().x, cameraTransform.translation().y, cameraTransform.translation().z };
		sceGumTranslate(&pos);

		sceGumMatrixMode(GU_MODEL);
		sceGumLoadIdentity();
		{
			ScePspFVector3 pos = { 0, 0, -2.5f };
//			ScePspFVector3 rot = { val * 0.79f * (GU_PI/180.0f), val * 0.98f * (GU_PI/180.0f), val * 1.32f * (GU_PI/180.0f) };
			sceGumTranslate(&pos);
//			sceGumRotateXYZ(&rot);
		}

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

		if(drawCube)
		{
			sceGuFrontFace(GU_CW);
			sceGumDrawArray(GU_TRIANGLES,GU_TEXTURE_32BITF|GU_COLOR_8888|GU_VERTEX_32BITF|GU_TRANSFORM_3D,12*3,0,vertices);
			sceGuFrontFace(GU_CCW);
		}

/*		ScePspFVector3 dir = { 0, 1, 0 };
		sceGuLight(0, GU_DIRECTIONAL, GU_DIFFUSE, &dir);
		sceGuLightColor(0, GU_DIFFUSE, 0x00ff8080);
		sceGuLightAtt(0, 1.0f, 0.0f, 0.0f);
*/		sceGuAmbient(0x00101010);
		sceGuColor(0xffffff);

		scenePlayerApp->update(val * 0.005f);
		scenePlayerApp->process();
		scenePlayerApp->render();

		pspDebugScreenSetOffset((int)fbp0);
		pspDebugScreenSetXY(0,0);
		pspDebugScreenPrintf("Hello world!");
		pspDebugScreenPrintf("Cubbie is rotating");

		pspDebugScreenPrintf("\n");
///		pspDebugScreenPrintf("skin: %d, %d, %d, %d\n", skin->vertexCount, skin->indexCount, skin->boneCount, skin->weightsPerVertex);

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
