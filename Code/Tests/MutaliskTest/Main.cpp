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

#include <mutalisk/psp/pspPlatform.h>
#include <mutalisk/mutalisk.h>
#include <player/ScenePlayer.h>
#include <player/psp/pspScenePlayer.h>

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

	void setProjMatrix(ScePspFMatrix4 const& projMatrix)
	{
		renderContext.projMatrix = projMatrix;
	}


	void update(float deltaTime) { scene.renderable->update(deltaTime); }
	void process() { scene.renderable->process(); }
	void render(int maxActors = -1, int maxLights = -1) { 
		::render(renderContext, *scene.renderable, true, true, maxActors, maxLights); }

	struct Scene
	{
		std::auto_ptr<mutalisk::data::scene> blueprint;
		std::auto_ptr<RenderableScene> renderable;
	};
	
	RenderContext	renderContext;
	Scene			scene;
};
std::auto_ptr<ScenePlayerApp> scenePlayerApp;
std::string gSceneFileName = "doll.msk";
std::string gPathPrefix = "host1:DemoTest/doll/psp/";//"ms0:PSP/TESTDATA/";


struct Texture
{
	int format;
	int mipmap;
	int width, height, stride;
	void* data;
	void* vramAddr;
};

struct Sampler
{
	int addressU;
	int addressV;
	int minFilter;
	int magFilter;
};

void sceGuSetAllStatusCached(int status)
{
	int currentStatus = sceGuGetAllStatus();
	unsigned int i;
	unsigned int mask = 1;
	for (i = 0; i < 22; ++i)
	{
		if (status & currentStatus & mask)
			continue;

		if (status & mask)
			sceGuEnable(i);
		else
			sceGuDisable(i);

		mask >>= 1;
	}
}

std::vector<int> gStateStack;
void pushState()
{
	gStateStack.push_back(sceGuGetAllStatus());
}

void popState()
{
	sceGuSetAllStatus(gStateStack.back());
	gStateStack.pop_back();
}

void setTexture(Texture const& texture)
{
	sceGuTexMode(texture.format,0,0,0);
	sceGuTexImage(texture.mipmap,texture.width,texture.height,texture.stride,texture.data);
}

void setSampler(Sampler const& sampler)
{
	sceGuTexFilter(sampler.minFilter,sampler.magFilter);
	sceGuTexWrap(sampler.addressU,sampler.addressV);
}

int gViewportWidth;
int gViewportHeight;
void setRenderTarget(Texture& renderTarget)
{
	sceGuDrawBufferList(renderTarget.format,renderTarget.vramAddr,renderTarget.stride);
	sceGuOffset(2048 - (renderTarget.width/2),2048 - (renderTarget.height/2));
	sceGuViewport(2048,2048,renderTarget.width,renderTarget.height);

	gViewportWidth = renderTarget.width;
	gViewportHeight = renderTarget.height;
}

u64 gTick = 0;
float gTickFrequency = 0;

float peekTime()
{
	if(gTickFrequency == 0)
	{
		gTickFrequency = sceRtcGetTickResolution();
		printf("TickFrequency: %f", gTickFrequency);
		gTickFrequency /= (1000 * 1000);
		printf("TickFrequency: %f\n", gTickFrequency);
		gTickFrequency = 1.0f / gTickFrequency;

		sceRtcGetCurrentTick(&gTick);
	}

	u64 prevTick = gTick;
	sceRtcGetCurrentTick(&gTick);

	return static_cast<float>(gTick - prevTick) * gTickFrequency;
}

int main(int argc, char* argv[])
{
//	logSetMessageThreshold(5);

	setupCallbacks();

	// setup GU
	Texture mainRenderTarget;
	mainRenderTarget.format = GU_PSM_8888;
	mainRenderTarget.mipmap = 0;
	mainRenderTarget.width = SCR_WIDTH;
	mainRenderTarget.height = SCR_HEIGHT;
	mainRenderTarget.stride = BUF_WIDTH;
	mainRenderTarget.vramAddr = getStaticVramBuffer(mainRenderTarget.stride,mainRenderTarget.height,mainRenderTarget.format);
	mainRenderTarget.data = 0;

	Texture mainRenderTarget2 = mainRenderTarget;
	mainRenderTarget2.vramAddr = getStaticVramBuffer(mainRenderTarget2.stride,mainRenderTarget2.height,mainRenderTarget2.format);

	void* zbp = getStaticVramBuffer(BUF_WIDTH,SCR_HEIGHT,GU_PSM_4444);

	Texture renderTarget;
	renderTarget.format = GU_PSM_8888;
	renderTarget.mipmap = 0;
	renderTarget.width = 128;
	renderTarget.height = 128;
	renderTarget.stride = 128;
	renderTarget.vramAddr = getStaticVramBuffer(renderTarget.stride,renderTarget.height,renderTarget.format);
	renderTarget.data = mapVramBufferToTexture(renderTarget.vramAddr);

	Texture renderTarget2 = renderTarget;
	renderTarget2.vramAddr = getStaticVramBuffer(renderTarget2.stride,renderTarget2.height,renderTarget2.format);
	renderTarget2.data = mapVramBufferToTexture(renderTarget2.vramAddr);

	Texture renderTarget3 = renderTarget;
	renderTarget3.vramAddr = getStaticVramBuffer(renderTarget3.stride,renderTarget3.height,renderTarget3.format);
	renderTarget3.data = mapVramBufferToTexture(renderTarget3.vramAddr);

	unsigned int renderTargetSize = renderTarget.height * renderTarget.stride * sizeof(int);
	void* mbp0 = malloc(renderTargetSize);
	void* mbp1 = malloc(renderTargetSize);

	Texture sysMemRenderTarget = renderTarget;
	sysMemRenderTarget.vramAddr = 0;
	sysMemRenderTarget.data = mbp1;
	Texture sysMemRenderTarget2 = sysMemRenderTarget;
	sysMemRenderTarget2.vramAddr = 0;
	sysMemRenderTarget2.data = mbp0;


	pspDebugScreenInit();
	sceGuInit();

	sceGuStart(GU_DIRECT,list);
	sceGuDrawBuffer(mainRenderTarget.format,mainRenderTarget.vramAddr,mainRenderTarget.stride);
	sceGuDispBuffer(mainRenderTarget2.width,mainRenderTarget2.height,mainRenderTarget2.vramAddr,mainRenderTarget2.stride);
	sceGuDepthBuffer(zbp,BUF_WIDTH);
	sceGuOffset(2048 - (mainRenderTarget.width/2),2048 - (mainRenderTarget.height/2));
	sceGuViewport(2048,2048,mainRenderTarget.width,mainRenderTarget.height);
	sceGuDepthRange(65535,0);
	sceGuScissor(0,0,mainRenderTarget.width,mainRenderTarget.height);
	sceGuEnable(GU_SCISSOR_TEST);
	sceGuDepthFunc(GU_GEQUAL);
	sceGuEnable(GU_DEPTH_TEST);
	sceGuFrontFace(GU_CCW);
	sceGuShadeModel(GU_SMOOTH);
//	sceGuEnable(GU_CULL_FACE);
	sceGuDisable(GU_CULL_FACE);
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

	CTransform cameraTransform = CTransform::identity();

	SceCtrlData oldPad;
	oldPad.Buttons = 0;
	sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode(0); 


;;peekTime();

	while(running())
	{
		bool drawCube = true;
		bool blurOnCPU = false;
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
		ScePspFVector3 cameraPos = { cameraTransform.translation().x, cameraTransform.translation().y, cameraTransform.translation().z };
		ScePspFVector3 cameraPosIdentity = { 0, 0, 0 };

		static unsigned int t = 0x80;
		t++;
		if(t > 0xff) t = 0x80;


;;peekTime();
		sceGuStart(GU_DIRECT,list);

		//if(0)
		{

			setRenderTarget(mainRenderTarget);

			// clear screen

			sceGuClearColor(0xff550033);
			sceGuClearDepth(0);
			sceGuClear(GU_COLOR_BUFFER_BIT|GU_DEPTH_BUFFER_BIT);

			sceGuAmbient(0x00101010);
			sceGuColor(0xffffff);

//			drawTestCube(cameraPos);//Identity);

			ScePspFMatrix4 projMatrix;
			gumLoadIdentity(&projMatrix);
			gumPerspective(&projMatrix, 45.0f, 16.0f/9.0f,0.5f,1000.0f);
			scenePlayerApp->setProjMatrix(projMatrix);
			scenePlayerApp->update(val * 0.005f);
			scenePlayerApp->process();
			scenePlayerApp->render();

		}


		pspDebugScreenSetOffset((int)mainRenderTarget.vramAddr);
		pspDebugScreenSetXY(0,0);
		pspDebugScreenPrintf("Hello world!");
		pspDebugScreenPrintf("Cubbie is rotating");

		pspDebugScreenPrintf("\n");
///		pspDebugScreenPrintf("skin: %d, %d, %d, %d\n", skin->vertexCount, skin->indexCount, skin->boneCount, skin->weightsPerVertex);

		sceGuFinish();
		sceGuSync(0,0);

;;pspDebugScreenPrintf("renderTime: %f", peekTime()/1000.0f);

		sceDisplayWaitVblankStart();
		mainRenderTarget2.vramAddr = mainRenderTarget.vramAddr;
		mainRenderTarget.vramAddr = sceGuSwapBuffers();

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
