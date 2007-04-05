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

#include <player/TimeControl.h>
#include <player/ScenePlayer.h>
#include <player/psp/pspScenePlayer.h>

#include "TimeBlock.h"
#include "PostProcess.h"
#include "BallRenderer.h"

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
//		mutalisk::prepareBalls(*scene.renderable);
	}

	void setProjMatrix(ScePspFMatrix4 const& projMatrix)
	{
		renderContext.projMatrix = projMatrix;
	}

	void update(float time) { scene.renderable->update(time); }
	void process() { scene.renderable->process(); }
	void render(int maxActors = -1, int maxLights = -1) { 
//		mutalisk::renderBalls(*scene.renderable);
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
std::string gConfigFileName = "host1:DemoTest/scenePlayer.cfg";
std::string gSceneFileName = "walk.msk";
std::string gPathPrefix = "host1:DemoTest/walk/psp/";//"ms0:PSP/TESTDATA/";


void readConfigFile(std::string const& configFileName)
{
	SceUID file;
	if((file = sceIoOpen(configFileName.c_str(), PSP_O_RDONLY, 0777)) > 0 )
	{
		SceOff fileSize = sceIoLseek(file, 0, PSP_SEEK_END);
		sceIoLseek(file, 0, PSP_SEEK_SET);

		char* data = new char[fileSize];
		if(sceIoRead(file, data, fileSize) == fileSize)
		{
			std::string str(data);
			if(str.find_first_of(" ") != std::string::npos)
			{
				std::string sceneFileName = str.substr(0, str.find_first_of(" "));
				std::string pathPrefix = str.substr(str.find_last_of(" ") + 1, std::string::npos);

				printf("path-prefix: %s, scene-name: %s\n", gPathPrefix.c_str(), gSceneFileName.c_str());

				if(!sceneFileName.empty())
					gSceneFileName = sceneFileName;
				if(!pathPrefix.empty())
					gPathPrefix = pathPrefix;
			}
		}
		delete[] data;
	}
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

u64 gPrevTick = 0;
float getDeltaTime()
{
	u64 currTick;
	sceRtcGetCurrentTick(&currTick);
	u64 delta = currTick - gPrevTick;
	gPrevTick = currTick;
	return static_cast<float>((delta) * mutalisk::tickFrequency()) / (1000.0f * 1000.0f);
}

mutalisk::TimeControl gTimeControl;

int main(int argc, char* argv[])
{
	setupCallbacks();

	// setup GU
	mutalisk::Texture mainRenderTarget;
	mainRenderTarget.format = GU_PSM_8888;
	mainRenderTarget.mipmap = 0;
	mainRenderTarget.width = SCR_WIDTH;
	mainRenderTarget.height = SCR_HEIGHT;
	mainRenderTarget.stride = BUF_WIDTH;
	mainRenderTarget.vramAddr = getStaticVramBuffer(mainRenderTarget.stride,mainRenderTarget.height,mainRenderTarget.format);
	mainRenderTarget.data = 0;

	mutalisk::Texture mainRenderTarget2 = mainRenderTarget;
	mainRenderTarget2.vramAddr = getStaticVramBuffer(mainRenderTarget2.stride,mainRenderTarget2.height,mainRenderTarget2.format);

	void* zbp = getStaticVramBuffer(BUF_WIDTH,SCR_HEIGHT,GU_PSM_4444);

	// Post process render targets
	std::auto_ptr<mutalisk::Texture> renderTarget;
	std::auto_ptr<mutalisk::Texture> renderTarget2;
	allocPostProcessTargets(GU_PSM_8888, 128, 128, renderTarget, renderTarget2);


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

	readConfigFile(gConfigFileName);

	printf("ScenePlayer: create\n");
	scenePlayerApp.reset(new ScenePlayerApp(gSceneFileName, gPathPrefix));
	printf("ScenePlayer: created and loaded\n");

	SceCtrlData oldPad;
	oldPad.Buttons = 0;
	sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode(0); 


	mutalisk::initTickFrequency(); getDeltaTime();
	startTime();
;;printf("tickResolution: %f, tickFrequency: %f\n", mutalisk::tickResolution(), mutalisk::tickFrequency());
//;;printf("main -- 0\n");


	gTimeControl.restart(true);
	while(running())
	{
//;;printf("main -- while_running\n");

;;mutalisk::TimeBlock updateTime, processTime, renderTime, loopTime, finishAndSyncTime;
;;loopTime.peek();

		bool doPrintInfo = false;
		bool doBlur = false;//true;
		static float blurStrength = 0.2f;
		static unsigned blurThreshold = 114;
		static int blurSrcModifier = 200;
		static int blurDstModifier = 160;
		SceCtrlData pad;
		if(sceCtrlPeekBufferPositive(&pad, 1))
		{
//			if (pad.Buttons != oldPad.Buttons)
			{
				if (pad.Buttons & PSP_CTRL_SELECT && !(oldPad.Buttons & PSP_CTRL_SELECT))
					gTimeControl.pause(!gTimeControl.isPaused());
				if (pad.Buttons & PSP_CTRL_START)// && !(oldPad.Buttons & PSP_CTRL_START))
					doPrintInfo = !doPrintInfo;
				if (pad.Buttons & PSP_CTRL_CROSS)
					doBlur = !doBlur;
				if (pad.Buttons & PSP_CTRL_UP && !(pad.Buttons & PSP_CTRL_TRIANGLE))
					blurStrength += 0.01f;
				if (pad.Buttons & PSP_CTRL_DOWN && !(pad.Buttons & PSP_CTRL_TRIANGLE))
					blurStrength -= 0.05f;
				if (pad.Buttons & PSP_CTRL_UP && (pad.Buttons & PSP_CTRL_TRIANGLE))
					blurThreshold += 1;
				if (pad.Buttons & PSP_CTRL_DOWN && (pad.Buttons & PSP_CTRL_TRIANGLE))
					blurThreshold -= 1;

				if ((pad.Buttons & PSP_CTRL_LEFT) && !(pad.Buttons & PSP_CTRL_TRIANGLE))
					blurSrcModifier -= 2;
				if ((pad.Buttons & PSP_CTRL_RIGHT) && !(pad.Buttons & PSP_CTRL_TRIANGLE))
					blurSrcModifier += 2;
				if ((pad.Buttons & PSP_CTRL_LEFT) && (pad.Buttons & PSP_CTRL_TRIANGLE))
					blurDstModifier -= 2;
				if ((pad.Buttons & PSP_CTRL_RIGHT) && (pad.Buttons & PSP_CTRL_TRIANGLE))
					blurDstModifier += 2;

				if(blurStrength < 0.0f) blurStrength = 0.0f;
				if(blurThreshold < 1) blurThreshold = 1; if(blurThreshold > 0xff) blurThreshold = 0xff;
				if(blurSrcModifier < 1) blurSrcModifier = 1; if(blurSrcModifier > 0xff) blurSrcModifier = 0xff;
				if(blurDstModifier < 1) blurDstModifier = 1; if(blurDstModifier > 0xff) blurDstModifier = 0xff;
			}
			oldPad = pad;
		}


		sceGuStart(GU_DIRECT,list);
//;;printf("main -- guStart\n");

		{
			mutalisk::setRenderTarget(mainRenderTarget);
			//setRenderTarget(renderTarget);

			// clear screen

			//sceGuClearColor(0xff00ff00);
			sceGuClearColor(0xff000000);
			sceGuClearDepth(0xffff);
			sceGuClear(GU_COLOR_BUFFER_BIT|GU_DEPTH_BUFFER_BIT);

			sceGuAmbient(0x00101010);
			sceGuColor(0xffffff);
			sceGuDisable(GU_BLEND);

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
			scenePlayerApp->update(gTimeControl.update(getDeltaTime()));
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

		if(doBlur)
		{
			{
				mutalisk::setRenderTarget(*renderTarget);

				unsigned clearWithBlurThreshold = 0xff000000 |
					(blurThreshold << 16) | (blurThreshold << 8) | blurThreshold;
				sceGuClearColor(clearWithBlurThreshold);
				sceGuClearDepth(0xffff);
				sceGuClear(GU_COLOR_BUFFER_BIT|GU_DEPTH_BUFFER_BIT);

				mutalisk::Texture mainRenderTargetAsSource;
				mainRenderTargetAsSource = mainRenderTarget;
				mainRenderTargetAsSource.width = mainRenderTarget.stride;
				mainRenderTargetAsSource.height = mainRenderTarget.stride;
				mainRenderTargetAsSource.data = mapVramBufferToTexture(mainRenderTarget.vramAddr);

				mutalisk::Sampler sampler;
				sampler.addressU = GU_CLAMP;
				sampler.addressV = GU_CLAMP;
				sampler.minFilter = GU_NEAREST;
				sampler.magFilter = GU_NEAREST;

				mutalisk::Region uvRegion;
				uvRegion.offset.x = uvRegion.offset.y = 0;
				uvRegion.scale.x = (float)SCR_WIDTH/(float)BUF_WIDTH;
				uvRegion.scale.y = (float)SCR_HEIGHT/(float)BUF_WIDTH;

				mutalisk::pushState();
					sceGuEnable(GU_BLEND);
					sceGuBlendFunc(GU_SUBTRACT, GU_FIX, GU_FIX, 0xffffffff, 0xffffffff);
					drawFullscreenQuad(mainRenderTargetAsSource, sampler, uvRegion);
					sceGuDisable(GU_BLEND);

					mutalisk::gpuBlur(*renderTarget, *renderTarget2, blurStrength);
				mutalisk::popState();
			}

			{
				mutalisk::setRenderTarget(mainRenderTarget);

				mutalisk::Sampler sampler;
				sampler.addressU = GU_CLAMP;
				sampler.addressV = GU_CLAMP;
				sampler.minFilter = GU_LINEAR;
				sampler.magFilter = GU_LINEAR;

				mutalisk::pushState();
					sceGuEnable(GU_BLEND);
					unsigned int srcFix = GU_ARGB(0, blurSrcModifier, blurSrcModifier, blurSrcModifier);
					unsigned int dstFix = GU_ARGB(0, blurDstModifier, blurDstModifier, blurDstModifier);
					sceGuBlendFunc(GU_ADD, GU_FIX, GU_FIX, srcFix, dstFix);
					mutalisk::drawFullscreenQuad(*renderTarget, sampler);
					sceGuDisable(GU_BLEND);
				mutalisk::popState();
			}
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
		if(doPrintInfo)
		{
			pspDebugScreenSetOffset((int)mainRenderTarget.vramAddr);
			pspDebugScreenSetXY(0,0);
			pspDebugScreenPrintf("timers: frame(%f) loop(%f) guFinish(%f)", frameTime.ms(), loopTime.ms(), finishAndSyncTime.ms());
			pspDebugScreenPrintf("\n");
			pspDebugScreenPrintf("mutalisk: update(%f) process(%f) render(%f)", updateTime.ms(), processTime.ms(), renderTime.ms());
			pspDebugScreenPrintf("\n");
			pspDebugScreenPrintf("blur: str(%f) thrshld(%d) src(%d) dst(%d)", blurStrength, (int)blurThreshold, (int)blurSrcModifier, (int)blurDstModifier);
		}

		sceDisplayWaitVblankStart();
		mainRenderTarget2.vramAddr = mainRenderTarget.vramAddr;
		mainRenderTarget.vramAddr = sceGuSwapBuffers();

		val++;
	}

	sceGuTerm();

	sceKernelExitGame();
	return 0;
}
