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

		scene.blueprint = mutalisk::loadResource<mutalisk::data::scene>(pathPrefix + sceneName);
		scene.renderable = prepare(renderContext, *scene.blueprint, pathPrefix);
	}

	void setProjMatrix(ScePspFMatrix4 const& projMatrix)
	{
		renderContext.projMatrix = projMatrix;
	}


	void update(float deltaTime) { scene.renderable->update(deltaTime); }
	void process() { scene.renderable->process(); }
	void render(int maxActors = -1, int maxLights = -1) { 
//		::render(renderContext, *scene.renderable, true, true, maxActors, maxLights); }
		mutalisk::render(renderContext, *scene.renderable, maxActors); }

	struct Scene
	{
		std::auto_ptr<mutalisk::data::scene> blueprint;
		std::auto_ptr<mutalisk::RenderableScene> renderable;
	};
	
	mutalisk::RenderContext	renderContext;
	Scene			scene;
};
std::auto_ptr<ScenePlayerApp> scenePlayerApp;
std::string gSceneFileName = "doll.msk";
std::string gPathPrefix = "host1:DemoTest/doll/psp/";//"ms0:PSP/TESTDATA/";

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

void drawFullscreenQuad(unsigned int color, unsigned int vertexElements = 0)
{
/*	struct QuadVertex
	{
		float x,y,z;
	};*/
	struct QuadVertex
	{
		unsigned short x,y,z;
	};
	struct QuadVertexTex
	{
		float u, v;
		float x,y,z;
	};

/*	static QuadVertex fsQuadVertices0[4] = {
		{-1, 1, -1}, // 0
		{ 1, 1, -1}, // 3
		{-1,-1, -1}, // 1
		{ 1,-1, -1}, // 2
	};*/
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

void drawFullscreenQuad(Texture const& texture, Sampler const& sampler, ScePspFVector2 uvOffset, unsigned int color, ScePspFVector3 pos)
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
/*/
	sceGuTexOffset(uvOffset.x/texture.width,uvOffset.y/texture.height);
	drawFullscreenQuad(color, GU_TEXTURE_32BITF);
/*/
	sceGuColor(color);
	sceGuDisable(GU_LIGHTING);
	sceGuDisable(GU_DEPTH_TEST);
	sceGuDepthMask(1);
	
	if(0)
	{
		short sx = 0;
		float tu = 0.0f;
		float sliceU = std::min(16.0f, static_cast<float>(texture.width));
		short sliceW = static_cast<short>(static_cast<float>(gViewportWidth) * (sliceU / static_cast<float>(texture.width)));
		for(; tu < texture.width; sx += sliceW, tu += sliceU)
		{
			QuadVertexTex* vertices = reinterpret_cast<QuadVertexTex*>(sceGuGetMemory(2 * sizeof(QuadVertexTex)));
			vertices[0].u = tu + uvOffset.x; vertices[0].v = 0 + uvOffset.y;
			vertices[0].x = sx; vertices[0].y = 0; vertices[0].z = 0;

			vertices[1].u = tu + sliceU + uvOffset.x; vertices[1].v = texture.height + uvOffset.y;
			vertices[1].x = sx + sliceW; vertices[1].y = gViewportHeight; vertices[1].z = 0;

			sceGuDrawArray(GU_SPRITES,GU_TEXTURE_32BITF|GU_VERTEX_16BIT|GU_TRANSFORM_2D,2,0,vertices);
		}
	}
	else
	{
		QuadVertexTex* vertices = reinterpret_cast<QuadVertexTex*>(sceGuGetMemory(2 * sizeof(QuadVertexTex)));
		vertices[0].u = 0 + uvOffset.x; vertices[0].v = 0 + uvOffset.y;
		vertices[0].x = 0; vertices[0].y = 0; vertices[0].z = 0;

		vertices[1].u = texture.width + uvOffset.x; vertices[1].v = texture.height + uvOffset.y;
		vertices[1].x = gViewportWidth; vertices[1].y = gViewportHeight; vertices[1].z = 0;

		sceGuDrawArray(GU_SPRITES,GU_TEXTURE_32BITF|GU_VERTEX_16BIT|GU_TRANSFORM_2D,2,0,vertices);
	}

	sceGuDepthMask(0);
//*/
}

void drawFullscreenQuad(Texture const& texture, Sampler const& sampler, ScePspFVector3 pos)
{
	ScePspFVector2 uvOffset = {0, 0};
	drawFullscreenQuad(texture, sampler, uvOffset, 0xffffffff, pos);
}

void drawFullscreenQuad(Texture const& texture, Sampler const& sampler, ScePspFVector2 uvOffset, unsigned int color)
{
	ScePspFVector3 pos = {0, 0, 0};
	drawFullscreenQuad(texture, sampler, uvOffset, color, pos);
}

void drawFullscreenQuad(Texture const& texture, Sampler const& sampler, ScePspFVector2 uvOffset)
{
	ScePspFVector3 pos = {0, 0, 0};
	drawFullscreenQuad(texture, sampler, uvOffset, 0xffffffff, pos);
}

void drawFullscreenQuad(Texture const& texture, Sampler const& sampler)
{
	ScePspFVector3 pos = {0, 0, 0};
	ScePspFVector2 uvOffset = {0, 0};
	drawFullscreenQuad(texture, sampler, uvOffset, 0xffffffff, pos);
}

void drawTestCube(ScePspFVector3 cameraPos)
{
	// setup matrices for cube

	sceGumMatrixMode(GU_PROJECTION);
	sceGumLoadIdentity();
	sceGumPerspective(35.0f,16.0f/9.0f,0.5f,1000.0f);

	sceGumMatrixMode(GU_VIEW);
	sceGumLoadIdentity();
	sceGumTranslate(&cameraPos);

	sceGumMatrixMode(GU_MODEL);
	sceGumLoadIdentity();
	{
		ScePspFVector3 pos = { 0, 0, -2.5f };
//		ScePspFVector3 rot = { val * 0.79f * (GU_PI/180.0f), val * 0.98f * (GU_PI/180.0f), val * 1.32f * (GU_PI/180.0f) };
//		sceGumTranslate(&pos);
//		sceGumRotateXYZ(&rot);
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
	sceGuEnable(GU_TEXTURE_2D);
//	sceGuFrontFace(GU_CW);
	sceGumDrawArray(GU_TRIANGLES,GU_TEXTURE_32BITF|GU_COLOR_8888|GU_VERTEX_32BITF|GU_TRANSFORM_3D,12*3,0,vertices);
//	sceGuFrontFace(GU_CCW);
	sceGuDisable(GU_TEXTURE_2D);
}

void cpuBlur(unsigned int t, Texture& texture)
{
	unsigned int t0 = t;
	unsigned int t1 = 0x100 - t0;

	unsigned int* asDword = (unsigned int*)texture.data;
	unsigned char* asByte = (unsigned char*)texture.data;
	for(int y = 0; y < texture.height/2+texture.height/4; ++y)
	{
		unsigned int color[4] = {0, 0, 0, 0};
		unsigned int color_[4] = {0, 0, 0, 0};
		for(int q = 0; q < 4; ++q)
			color[q] = asByte[(y * texture.stride + 0) * 4   +q];
		for(int q = 0; q < 4; ++q)
			color_[q] = asByte[(y * texture.stride + (texture.width - 1))* 4   +q];

		for(int x = 0; x < texture.width; ++x)
		{
			//asDword[y * renderTarget.stride + x] = 0x00ff8000;
			for(int q = 0; q < 3; ++q)
			{
				unsigned int color2 = asByte[(y * texture.stride + x) * 4   +q];						
				color[q] = (color[q]*t0 + color2*t1) >> 8;
				asByte[(y * texture.stride + x) * 4   +q] = color[q];
			}
		}

		for(int x = texture.width - 1; x >= 0; --x)
		{
			for(int q = 0; q < 3; ++q)
			{
				unsigned int color2 = asByte[(y * texture.stride + x) * 4   +q];						
				color_[q] = (color_[q]*t0 + color2*t1) >> 8;
				asByte[(y * texture.stride + x) * 4   +q] = color_[q];
			}
		}
	}

	if(0)
	for(int x = 0; x < texture.width; ++x)
	{
		unsigned int color[4] = {0, 0, 0, 0};
		unsigned int color_[4] = {0, 0, 0, 0};
		for(int q = 0; q < 4; ++q)
			color[q] = asByte[(0 * texture.stride + x) * 4   +q];
		for(int q = 0; q < 4; ++q)
			color_[q] = asByte[((texture.height/2+texture.height/4 -1) * texture.stride + x) * 4   +q];

		for(int y = 0; y < texture.height/2+texture.height/4; ++y)
		{
			for(int q = 0; q < 3; ++q)
			{
				unsigned int color2 = asByte[(y * texture.stride + x) * 4   +q];						
				color[q] = (color[q]*t0 + color2*t1) >> 8;
				asByte[(y * texture.stride + x) * 4   +q] = color[q];
			}
		}

		for(int y = texture.height/2+texture.height/4 -1; y >= 0; --y)
		{
			for(int q = 0; q < 3; ++q)
			{
				unsigned int color2 = asByte[(y * texture.stride + x) * 4   +q];						
				color_[q] = (color_[q]*t0 + color2*t1) >> 8;
				asByte[(y * texture.stride + x) * 4   +q] = color_[q];
			}
		}
	}
	sceKernelDcacheWritebackAll();
}

/*
void sceGuClear_(unsigned int clearColor)
{
	struct Vertex
	{
		u32 color;
		u16 x,y,z;
		u16 pad;
	};


	unsigned int filter = clearColor & 0xffffff;
	struct Vertex* vertices;
	int count;

	{
		vertices = (struct Vertex*)sceGuGetMemory(2 * sizeof(struct Vertex));
		count = 2;

		vertices[0].color = 0;
		vertices[0].x = 0;
		vertices[0].y = 0;
		vertices[0].z = 0;

		vertices[1].color = filter;
		vertices[1].x = gu_draw_buffer.width;
		vertices[1].y = gu_draw_buffer.height;
		vertices[1].z = 0;
	}

	sendCommandi(211,((flags & (GU_COLOR_BUFFER_BIT|GU_STENCIL_BUFFER_BIT|GU_DEPTH_BUFFER_BIT)) << 8) | 0x01);
	sceGuDrawArray(GU_SPRITES,GU_COLOR_8888|GU_VERTEX_16BIT|GU_TRANSFORM_2D,count,0,vertices);
	sendCommandi(211,0);
}*/

void gpuBlur(unsigned int t, Texture& srcRenderTarget, Texture& dstRenderTarget)
{
//	float halfPixelW = (1.0f / srcRenderTarget.width) * 0.5f;
//	float halfPixelH = (1.0f / srcRenderTarget.height) * 0.5f;
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
	float const offsetScaler = 1.9f;

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
/*	Texture frameBuffer;
	frameBuffer.format = GU_PSM_8888;
	frameBuffer.mipmap = 0;
	frameBuffer.width = SCR_WIDTH;
	frameBuffer.height = SCR_HEIGHT;
	frameBuffer.stride = BUF_WIDTH;
	frameBuffer.vramAddr = getStaticVramBuffer(frameBuffer.stride,frameBuffer.height,frameBuffer.format);
	frameBuffer.data = mapVramBufferToTexture(frameBuffer.vramAddr);*/

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

//	void* fbp0 = getStaticVramBuffer(BUF_WIDTH,SCR_HEIGHT,GU_PSM_8888);
//	void* fbp1 = getStaticVramBuffer(BUF_WIDTH,SCR_HEIGHT,GU_PSM_8888);
	void* zbp = getStaticVramBuffer(BUF_WIDTH,SCR_HEIGHT,GU_PSM_4444);



	/*			sceGuDrawBufferList(GU_PSM_8888,fbp0,BUF_WIDTH);
			sceGuOffset(2048 - (SCR_WIDTH/2),2048 - (SCR_HEIGHT/2));
			sceGuViewport(2048,2048,SCR_WIDTH,SCR_HEIGHT);*/


/*	Texture renderTarget = {
		GU_PSM_8888, 0,
		256, 256, 256,
		0
	};*/
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

		//*
			setRenderTarget(renderTarget);
		/*/
			sceGuDrawBuffer(GU_PSM_8888,fbp0,BUF_WIDTH);
			sceGuOffset(2048 - (SCR_WIDTH/2),2048 - (SCR_HEIGHT/2));
			sceGuViewport(2048,2048,SCR_WIDTH,SCR_HEIGHT);
		//*/


			// clear screen

			sceGuClearColor(0xff550033);
			sceGuClearDepth(0);
			sceGuClear(GU_COLOR_BUFFER_BIT|GU_DEPTH_BUFFER_BIT);

			sceGuAmbient(0x00101010);
			sceGuColor(0xffffff);

			drawTestCube(cameraPos);//Identity);

			ScePspFMatrix4 projMatrix;
			gumLoadIdentity(&projMatrix);
			gumPerspective(&projMatrix, 25.0f, 16.0f/9.0f,0.5f,1000.0f);
			scenePlayerApp->setProjMatrix(projMatrix);
			scenePlayerApp->update(val * 0.005f);
			scenePlayerApp->process();
			scenePlayerApp->render();

		}

		if(!blurOnCPU)
		{
			pushState();
			gpuBlur(t, renderTarget, renderTarget2);
			popState();
		}

		{
/*			sceGuDrawBufferList(GU_PSM_8888,fbp0,BUF_WIDTH);
			sceGuOffset(2048 - (SCR_WIDTH/2),2048 - (SCR_HEIGHT/2));
			sceGuViewport(2048,2048,SCR_WIDTH,SCR_HEIGHT);*/
			setRenderTarget(mainRenderTarget);

			sceGuClearColor(0xff334455);
			sceGuClearDepth(0);
			sceGuClear(GU_COLOR_BUFFER_BIT|GU_DEPTH_BUFFER_BIT);

			ScePspFVector3 cameraPos2 = cameraPos;
			cameraPos2.x = 0;
			cameraPos2.y = 0;
			cameraPos2.z = 0;

			Texture texture;
			texture.format = GU_PSM_4444;
			texture.mipmap = 0;
			texture.width = 64;
			texture.height = 64;
			texture.stride = 64;
			texture.data = logo_start;

			Sampler sampler;
			sampler.addressU = GU_CLAMP;
			sampler.addressV = GU_CLAMP;
			sampler.minFilter = GU_LINEAR;
			sampler.magFilter = GU_LINEAR;

//			sampler.addressU = GU_REPEAT;
//			sampler.addressV = GU_REPEAT;
			sampler.minFilter = GU_NEAREST;
			sampler.magFilter = GU_NEAREST;


			ScePspFVector2 uvOffset;
			uvOffset.x = cameraPos.x;
			uvOffset.y = cameraPos.y;

//			uvOffset.x *= (1.0f / sysMemRenderTarget.width);
//			uvOffset.y *= (1.0f / sysMemRenderTarget.height);


			pushState();
			if(blurOnCPU)
//				drawFullscreenQuad(sysMemRenderTarget, sampler, cameraPos2);
				drawFullscreenQuad(sysMemRenderTarget, sampler, uvOffset);
			else
				drawFullscreenQuad(renderTarget/*renderTarget2*/, sampler, cameraPos2);
		//	drawFullscreenQuad(renderTarget, cameraPos2);
		//	drawFullscreenQuad(texture, cameraPos2);
			popState();
//			drawTestCube(cameraPos);
		}

		pspDebugScreenSetOffset((int)mainRenderTarget.vramAddr);
		pspDebugScreenSetXY(0,0);
		pspDebugScreenPrintf("Hello world!");
		pspDebugScreenPrintf("Cubbie is rotating");

		pspDebugScreenPrintf("\n");
///		pspDebugScreenPrintf("skin: %d, %d, %d, %d\n", skin->vertexCount, skin->indexCount, skin->boneCount, skin->weightsPerVertex);

		if(blurOnCPU)
		{
			sceGuCopyImage(renderTarget.format,0,0,
				renderTarget.width,renderTarget.height,renderTarget.stride,
				renderTarget.data,0,0,sysMemRenderTarget.stride,sysMemRenderTarget.data);
		}

		sceGuFinish();
		sceGuSync(0,0);

;;pspDebugScreenPrintf("renderTime: %f", peekTime()/1000.0f);

		if(blurOnCPU)
		{
			std::swap(sysMemRenderTarget2, sysMemRenderTarget);
			cpuBlur(t, sysMemRenderTarget2);
;;pspDebugScreenPrintf(" blurTime: %f", peekTime()/1000.0f);
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
