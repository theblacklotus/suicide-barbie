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
std::string gSceneFileName = "telephone_s1.msk";
std::string gPathPrefix = "host1:DemoTest/telephone_s1/psp/";//"ms0:PSP/TESTDATA/";



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



/******************************************************************************************/
/*                                                                                        */
/******************************************************************************************/
#if 0
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

struct Sampler
{
	int addressU;
	int addressV;
	int minFilter;
	int magFilter;
};

struct Region
{
	ScePspFVector2 offset;
	ScePspFVector2 scale;
};

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

void drawFullscreenQuad(unsigned int color, unsigned int vertexElements = 0)
{
	struct QuadVertex
	{
		unsigned short x,y,z;
	};
	struct QuadVertexTex
	{
		float u, v;
		float x,y,z;
	};

	static QuadVertexTex fsQuadVerticesTex[4] = {
		{0, 0, -1, 1, -1}, // 0
		{1, 0,  1, 1, -1}, // 3
		{0, 1, -1,-1, -1}, // 1
		{1, 1,  1,-1, -1}, // 2
	};

	sceGumMatrixMode(GU_PROJECTION);
	sceGumLoadIdentity();
	sceGumOrtho(-1.0f,1.0f,-1.0f,1.0f, 0.5f,1000.0f);

	sceGumMatrixMode(GU_VIEW);
	sceGumLoadIdentity();

	sceGumMatrixMode(GU_MODEL);
	sceGumLoadIdentity();

	sceGuColor(color);

	sceGuDisable(GU_LIGHTING);
	sceGuDisable(GU_DEPTH_TEST);
	sceGuDepthMask(1);

	// draw quad
	if(vertexElements & GU_TEXTURE_32BITF)
		sceGumDrawArray(GU_TRIANGLE_STRIP,GU_TEXTURE_32BITF|GU_VERTEX_32BITF|GU_TRANSFORM_3D,4,0,fsQuadVerticesTex);
	else
	{
		QuadVertex* vertices = reinterpret_cast<QuadVertex*>(sceGuGetMemory(2 * 32 * sizeof(QuadVertex)));

		short sx = 0;
		short sliceW = 32;
		int vertexCount = 0;
		for(; sx < gViewportWidth; sx += sliceW)
		{
			if(sx + sliceW > gViewportWidth)
				sliceW = gViewportWidth - sx;

			vertices[vertexCount].x = sx;
			vertices[vertexCount].y = 0;
			vertices[vertexCount].z = 0;
			++vertexCount;
			vertices[vertexCount].x = sx + sliceW;
			vertices[vertexCount].y = gViewportHeight;
			vertices[vertexCount].z = 0;
			++vertexCount;
		}
		sceGuDrawArray(GU_SPRITES,GU_VERTEX_16BIT|GU_TRANSFORM_2D,vertexCount,0,vertices);
	}
	sceGuDepthMask(0);
}

void drawFullscreenQuad(Texture const& texture, Sampler const& sampler, Region const& uvRegion, unsigned int color)
{
	struct QuadVertexTex
	{
		float u, v;
		short x,y,z;
	};

	// setup texture
	setTexture(texture);
	setSampler(sampler);
	sceGuTexFunc(GU_TFX_MODULATE,GU_TCC_RGB);
	sceGuTexScale(1.0f,1.0f);
	sceGuEnable(GU_TEXTURE_2D);

	// draw quad
	sceGuColor(color);
	sceGuDisable(GU_LIGHTING);
	sceGuDisable(GU_DEPTH_TEST);
	sceGuDepthMask(1);
	
	QuadVertexTex* vertices = reinterpret_cast<QuadVertexTex*>(sceGuGetMemory(2 * sizeof(QuadVertexTex)));
	vertices[0].u = 0 + uvRegion.offset.x; vertices[0].v = 0 + uvRegion.offset.y;
	vertices[0].x = 0; vertices[0].y = 0; vertices[0].z = 0;

	vertices[1].u = texture.width * uvRegion.scale.x + uvRegion.offset.x;
	vertices[1].v = texture.height * uvRegion.scale.y + uvRegion.offset.y;
	vertices[1].x = gViewportWidth; vertices[1].y = gViewportHeight; vertices[1].z = 0;

	sceGuDrawArray(GU_SPRITES,GU_TEXTURE_32BITF|GU_VERTEX_16BIT|GU_TRANSFORM_2D,2,0,vertices);

	sceGuDepthMask(0);
}

void drawFullscreenQuad(Texture const& texture, Sampler const& sampler, ScePspFVector2 offset, unsigned int color)
{
	Region uvRegion;
	uvRegion.offset = offset;
	uvRegion.scale.x = uvRegion.scale.y = 1;
	drawFullscreenQuad(texture, sampler, uvRegion, color);
}

void drawFullscreenQuad(Texture const& texture, Sampler const& sampler, ScePspFVector2 offset)
{
	Region uvRegion;
	uvRegion.offset = offset;
	uvRegion.scale.x = uvRegion.scale.y = 1;
	drawFullscreenQuad(texture, sampler, uvRegion, 0xffffffff);
}

void drawFullscreenQuad(Texture const& texture, Sampler const& sampler, Region const& uvRegion)
{
	drawFullscreenQuad(texture, sampler, uvRegion, 0xffffffff);
}

void drawFullscreenQuad(Texture const& texture, Sampler const& sampler)
{
	Region uvRegion;
	uvRegion.offset.x = uvRegion.offset.y = 0;
	uvRegion.scale.x = uvRegion.scale.y = 1;
	drawFullscreenQuad(texture, sampler, uvRegion, 0xffffffff);
}

void gpuBlur(float blur, Texture& srcRenderTarget, Texture& dstRenderTarget)
{
	float halfPixelW = 0.5f;
	float halfPixelH = 0.5f;
	float offsetW = halfPixelW;
	float offsetH = halfPixelH;

	float offsets[] = {
		 1.0f,
		 3.0f,
		 5.0f,
		 7.0f,
		-1.0f,
		-3.0f,
		-5.0f,
		-7.0f,
	};
	float const offsetScaler = blur * 2.0f;// 1.9f;

	Sampler sampler;
	sampler.addressU = GU_CLAMP;
	sampler.addressV = GU_CLAMP;
	sampler.minFilter = GU_LINEAR;
	sampler.magFilter = GU_LINEAR;

	unsigned int quadCount = sizeof(offsets) / sizeof(float);
	unsigned int blendMultiplier = GU_ARGB(0, 0x100 / quadCount, 0x100 / quadCount, 0x100 / quadCount);
	for(int q = 0; q < 2; ++q)
	{
		{
			setRenderTarget(dstRenderTarget);

			sceGuDisable(GU_BLEND);
			drawFullscreenQuad(0);

			sceGuEnable(GU_BLEND);
			sceGuBlendFunc(GU_ADD, GU_FIX, GU_FIX, blendMultiplier, 0xffffffff);

			for(unsigned int w = 0; w < quadCount; ++w)
			{
				ScePspFVector2 uvOffsetsW = { offsetW * offsets[w] * offsetScaler, 0.0f };
				drawFullscreenQuad(srcRenderTarget, sampler, uvOffsetsW);
			}

			sceGuDisable(GU_BLEND);
		}

		if(1)
		{
			setRenderTarget(srcRenderTarget);

			sceGuDisable(GU_BLEND);
			drawFullscreenQuad(0);

			sceGuEnable(GU_BLEND);
			sceGuBlendFunc(GU_ADD, GU_FIX, GU_FIX, blendMultiplier, 0xffffffff);

			for(unsigned int w = 0; w < quadCount; ++w)
			{
				ScePspFVector2 uvOffsetH = { 0.0f, offsetH * offsets[w] * offsetScaler};
				drawFullscreenQuad(dstRenderTarget, sampler, uvOffsetH);
			}

			sceGuDisable(GU_BLEND);
		}
	}

	std::swap(srcRenderTarget, dstRenderTarget);
}

#endif

/******************************************************************************************/
/*                                                                                        */
/******************************************************************************************/



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

	/**
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
	*/

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

		bool doBlur = true;
		static float blurStrength = 0.2f;
		static unsigned blurThreshold = 100;
		static int blurSrcModifier = 255;
		static int blurDstModifier = 160;
		SceCtrlData pad;
		if(sceCtrlPeekBufferPositive(&pad, 1))
		{
//			if (pad.Buttons != oldPad.Buttons)
			{
				if (pad.Buttons & PSP_CTRL_SELECT && !(oldPad.Buttons & PSP_CTRL_SELECT))
					gTimeControl.pause(!gTimeControl.isPaused());
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
				uvRegion.scale.x = 480.0f/512.0f;
				uvRegion.scale.y = 272.0f/512.0f;

				mutalisk::pushState();
				sceGuEnable(GU_BLEND);
				sceGuBlendFunc(GU_SUBTRACT, GU_FIX, GU_FIX, 0xffffffff, 0xffffffff);
				drawFullscreenQuad(mainRenderTargetAsSource, sampler, uvRegion);
				sceGuDisable(GU_BLEND);
				mutalisk::popState();

				mutalisk::pushState();
				mutalisk::gpuBlur(*renderTarget, *renderTarget2, blurStrength);
				mutalisk::popState();
			}

			{
				mutalisk::setRenderTarget(mainRenderTarget);

//				sceGuClearColor(0xffff00ff);
//				sceGuClearDepth(0xffff);
//				sceGuClear(GU_COLOR_BUFFER_BIT|GU_DEPTH_BUFFER_BIT);

				mutalisk::Sampler sampler;
				sampler.addressU = GU_CLAMP;
				sampler.addressV = GU_CLAMP;
				sampler.minFilter = GU_LINEAR;
				sampler.magFilter = GU_LINEAR;

	//			sampler.minFilter = GU_NEAREST;
	//			sampler.magFilter = GU_NEAREST;
				
				mutalisk::pushState();
//				scenePlayerApp->render();

				sceGuEnable(GU_BLEND);
				unsigned int blendValue = 0x40;
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
		pspDebugScreenSetOffset((int)mainRenderTarget.vramAddr);
		pspDebugScreenSetXY(0,0);
		pspDebugScreenPrintf("timers: frame(%f) loop(%f) guFinish(%f)", frameTime.ms(), loopTime.ms(), finishAndSyncTime.ms());
		pspDebugScreenPrintf("\n");
		pspDebugScreenPrintf("mutalisk: update(%f) process(%f) render(%f)", updateTime.ms(), processTime.ms(), renderTime.ms());
		pspDebugScreenPrintf("\n");
		pspDebugScreenPrintf("blur: str(%f) thrshld(%d) src(%d) dst(%d)", blurStrength, (int)blurThreshold, (int)blurSrcModifier, (int)blurDstModifier);

		sceDisplayWaitVblankStart();
		mainRenderTarget2.vramAddr = mainRenderTarget.vramAddr;
		mainRenderTarget.vramAddr = sceGuSwapBuffers();

		val++;
	}

	sceGuTerm();

	sceKernelExitGame();
	return 0;
}
