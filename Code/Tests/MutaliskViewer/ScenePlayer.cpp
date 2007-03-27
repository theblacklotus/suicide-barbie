/*
 * Test of the Screen class
 */
//#include <mutant/reader.h>
//#include <mutant/io_factory.h>

#include <pspkernel.h>
#include <pspdisplay.h>
#include <psprtc.h>
#include <pspdebug.h>

#include <pspctrl.h>
#include <pspgu.h>
#include <pspgum.h>

#include "callbacks.h"
#include "vram.h"

#include <mutalisk/psp/pspPlatform.h>
#include <mutalisk/mutalisk.h>

extern "C" {
	#include <Base/Math/Math.h>
	#include <Base/Std/Std.h>
	#include <Base/Math/Lin.h>
	#include <Base/Math/Quat.h>
}

#include <player/ScenePlayer.h>
#include <player/psp/pspScenePlayer.h>

#include "TimeBlock.h"

PSP_MODULE_INFO("ScenePlayer", 0x1000, 1, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER);

static unsigned int __attribute__((aligned(16))) list[262144];
extern unsigned char logo_start[];


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

		scene.blueprint = mutalisk::loadResource<mutalisk::data::scene>(pathPrefix + sceneName);
		scene.renderable = prepare(renderContext, *scene.blueprint, pathPrefix);
	}

	void setProjMatrix(ScePspFMatrix4 const& projMatrix)
	{
		renderContext.projMatrix = projMatrix;
	}

	void update(float time) { scene.renderable->update(time); }
	void process() { scene.renderable->process(); }
	void render(int maxActors = -1, int maxLights = -1) { 
		mutalisk::render(renderContext, *scene.renderable, maxActors); }

	struct Scene
	{
		std::auto_ptr<mutalisk::data::scene> blueprint;
		std::auto_ptr<mutalisk::RenderableScene> renderable;
	};
	
	mutalisk::RenderContext	renderContext;
	Scene					scene;
};
std::auto_ptr<ScenePlayerApp> scenePlayerApp;
std::string gSceneFileName = "flower.msk";
std::string gPathPrefix = "host1:DemoTest/flower/psp/";//"ms0:PSP/TESTDATA/";


struct Texture
{
	int format;
	int mipmap;
	int width, height, stride;
	void* data;
	void* vramAddr;
};

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

u64 gStartTick = 0;
void startTime()
{
	sceRtcGetCurrentTick(&gStartTick);
}

float getTime()
{
	u64 currTick;
	sceRtcGetCurrentTick(&currTick);
	return static_cast<float>(currTick - gStartTick) / mutalisk::tickResolution();
}

int main(int argc, char* argv[])
{
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

	pspDebugScreenInit();
	sceGuInit();

	sceGuStart(GU_DIRECT,list);
	sceGuDrawBuffer(mainRenderTarget.format,mainRenderTarget.vramAddr,mainRenderTarget.stride);
	sceGuDispBuffer(mainRenderTarget2.width,mainRenderTarget2.height,mainRenderTarget2.vramAddr,mainRenderTarget2.stride);
	sceGuDepthBuffer(zbp,BUF_WIDTH);
	sceGuOffset(2048 - (mainRenderTarget.width/2),2048 - (mainRenderTarget.height/2));
	sceGuViewport(2048,2048,mainRenderTarget.width,mainRenderTarget.height);
	sceGuDepthRange(0, 0xffff);
	sceGuScissor(0,0,mainRenderTarget.width,mainRenderTarget.height);
	sceGuEnable(GU_SCISSOR_TEST);
	sceGuEnable(GU_DEPTH_TEST);
	sceGuDepthFunc(GU_EQUAL);
//	sceGuFrontFace(GU_CW);
	sceGuFrontFace(GU_CCW);
	sceGuShadeModel(GU_SMOOTH);
	sceGuEnable(GU_CULL_FACE);
	sceGuDisable(GU_TEXTURE_2D);

	sceGuFinish();
	sceGuSync(0,0);

	sceDisplayWaitVblankStart();
	sceGuDisplay(GU_TRUE);

	// run sample

	int val = 0;

	printf("ScenePlayer: create\n");
	scenePlayerApp.reset(new ScenePlayerApp(gSceneFileName, gPathPrefix));
	printf("ScenePlayer: created and loaded\n");

	SceCtrlData oldPad;
	oldPad.Buttons = 0;
	sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode(0); 


	mutalisk::initTickFrequency();
	startTime();
;;printf("tickResolution: %f, tickFrequency: %f\n", mutalisk::tickResolution(), mutalisk::tickFrequency());

//;;printf("main -- 0\n");


	while(running())
	{
//;;printf("main -- while_running\n");
		SceCtrlData pad;
		if(sceCtrlPeekBufferPositive(&pad, 1))
		{
/*
			//if (pad.Buttons != oldPad.Buttons)
			{
				if (pad.Buttons & PSP_CTRL_UP)
				if (pad.Buttons & PSP_CTRL_DOWN)
				if (pad.Buttons & PSP_CTRL_LEFT)
				if (pad.Buttons & PSP_CTRL_RIGHT)
				if (pad.Buttons & PSP_CTRL_LTRIGGER)
				if (pad.Buttons & PSP_CTRL_RTRIGGER)
				if (pad.Buttons & PSP_CTRL_CROSS)
			}
			oldPad = pad;*/
		}
;;mutalisk::TimeBlock updateTime, processTime, renderTime, loopTime, finishAndSyncTime;
;;loopTime.peek();
		sceGuStart(GU_DIRECT,list);
//;;printf("main -- guStart\n");

		//if(0)
		{

			setRenderTarget(mainRenderTarget);

			// clear screen

			sceGuClearColor(0xff000000);
			sceGuClearDepth(0xffff);
			sceGuClear(GU_COLOR_BUFFER_BIT|GU_DEPTH_BUFFER_BIT);

			sceGuAmbient(0x00101010);
			sceGuColor(0xffffff);

			ScePspFMatrix4 projMatrix;
			gumLoadIdentity(&projMatrix);

//;;printf("main -- projMatrix0\n");

			float fovy = 45.0f;
			float aspect = 16.0f/9.0f;
			float zn = 1.0f;
			float zf = 50.0f;
			{
				ScePspFMatrix4 t;
				float angle = (fovy / 2) * (GU_PI/180.0f);
				float cotangent = cosf(angle) / sinf(angle);

				gumLoadIdentity(&t);

				t.x.x = cotangent / aspect;
				t.y.y = cotangent;
				t.z.z = zf/(zn-zf);// (far + near) / delta_z; // -(far + near) / delta_z
				t.w.z = zn*zf/(zn-zf);//2.0f * (far * near) / delta_z; // -2 * (far * near) / delta_z
				t.z.w = -1.0f;
				t.w.w = 0.0f;

				gumMultMatrix(&projMatrix,&projMatrix,&t);
			}
//;;printf("main -- projMatrix1\n");

			scenePlayerApp->setProjMatrix(projMatrix);
//;;printf("main -- setProjMatrix\n");
;;updateTime.peek();
			scenePlayerApp->update(getTime());
;;updateTime.peek();
//;;printf("main -- update\n");
;;processTime.peek();
			scenePlayerApp->process();
;;processTime.peek();
//;;printf("main -- process\n");
;;renderTime.peek();
			scenePlayerApp->render();
;;renderTime.peek();
//;;printf("main -- render\n");
		}

//;;printf("main -- guFinish0\n");
;;finishAndSyncTime.peek();
		sceGuFinish();
//;;printf("main -- guFinish1\n");
		sceGuSync(0,0);
;;finishAndSyncTime.peek();
//;;printf("main -- guSync\n");

;;loopTime.peek();
;;static mutalisk::TimeBlock frameTime; frameTime.peek();
		pspDebugScreenSetOffset((int)mainRenderTarget.vramAddr);
		pspDebugScreenSetXY(0,0);
		pspDebugScreenPrintf("timers: frame(%f) loop(%f) guFinish(%f)", frameTime.ms(), loopTime.ms(), finishAndSyncTime.ms());
		pspDebugScreenPrintf("\n");
		pspDebugScreenPrintf("mutalisk: update(%f) process(%f) render(%f)", updateTime.ms(), processTime.ms(), renderTime.ms());

		sceDisplayWaitVblankStart();
		mainRenderTarget2.vramAddr = mainRenderTarget.vramAddr;
		mainRenderTarget.vramAddr = sceGuSwapBuffers();

		val++;
	}

	sceGuTerm();

	sceKernelExitGame();
	return 0;
}
