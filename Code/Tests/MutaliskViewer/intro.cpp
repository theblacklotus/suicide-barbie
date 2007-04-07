#include <stdio.h>
#include <pspkernel.h>
#include <stdlib.h>
#include <string.h>
#include <pspgu.h>

#include <platform.h>

#include <pspdisplay.h>
#include "intro.h"
#include "AnimCreator.h"

/*

	if (!loadIntro("host1:/DemoTest/intro/psp/"))
		printf("error loading intro\n");

	//---------------------

	while(running())
	{

		mutalisk::setRenderTarget(mainRenderTarget);

		sceGuStart(GU_DIRECT,list);

		renderIntro();

		sceGuFinish();
		sceGuSync(0,0);
		sceDisplayWaitVblankStart();
		mainRenderTarget2.vramAddr = mainRenderTarget.vramAddr;
		mainRenderTarget.vramAddr = sceGuSwapBuffers();

	}

	//---------------------

	unloadIntro();


*/

struct Texture
{
	int format;
	int mipmap;
	int width, height, stride;
	void* data;
	void* vramAddr;
};

static IntroRenderTarget s_renderTarget;
static IntroRenderTarget s_renderTarget2;
void setRenderTarget(IntroRenderTarget& renderTarget)
{
	sceGuDrawBufferList(renderTarget.format,renderTarget.vramAddr,renderTarget.stride);
	sceGuOffset(2048 - (renderTarget.width/2),2048 - (renderTarget.height/2));
	sceGuViewport(2048,2048,renderTarget.width,renderTarget.height);
}

static unsigned int __attribute__((aligned(16))) list[4096];
void renderIntro(float time);
void renderPSPLogo(float time);

int tbl_intro(SceSize args, void *argp)
{
	float t = 0.f;
	bool firstTime = true;
	while(t < 11.f)
	{
		t += 1/60.f;

		sceGuStart(GU_DIRECT,list);
		setRenderTarget(s_renderTarget);

		if (firstTime)
		{
			sceGuClearColor(0xff000000);
			sceGuClearDepth(0xffff);
			sceGuClear(GU_COLOR_BUFFER_BIT|GU_DEPTH_BUFFER_BIT);
			firstTime = false;
		}

		if (t > 8.f)
			renderPSPLogo(t);
		else
			renderIntro(t);

		sceGuFinish();
		sceGuSync(0,0);
		sceDisplayWaitVblankStart();
		s_renderTarget2.vramAddr = s_renderTarget.vramAddr;
		s_renderTarget.vramAddr = sceGuSwapBuffers();

		saveAnimFrame(s_renderTarget2.vramAddr);
	}
	return 0;
}

SceUID startIntro(const IntroRenderTarget renderTarget[2])
{
	s_renderTarget = renderTarget[0];
	s_renderTarget2 = renderTarget[1];
	SceUID introth = sceKernelCreateThread("tbl_intro", tbl_intro, 0x12, 0x10000, PSP_THREAD_ATTR_USER, NULL);

	if (introth < 0)
	{
		printf("Error creating tbl_intro.\n");
		return introth;
	}

	sceKernelStartThread(introth, 0, 0);
	return introth;
}

void uncompressFrame(uint32_t* in, uint32_t* out, int size);
mutalisk::data::psp_texture* loadTexture(const std::string& name);
int loadAnim(const std::string name, std::vector<mutalisk::data::psp_texture*>& textures);
void drawDots(float time, int evenFrame, float fade);

static mutalisk::data::psp_texture* centerDot = 0;
static mutalisk::data::psp_texture* whiteDot = 0;
static mutalisk::data::psp_texture* blueDot = 0;
static mutalisk::data::psp_texture* redDot = 0;
static mutalisk::data::psp_texture* tblTextA = 0;
static mutalisk::data::psp_texture* tblTextB = 0;
static mutalisk::data::psp_texture* pspLogoA = 0;
static mutalisk::data::psp_texture* pspLogoB = 0;
static std::vector<mutalisk::data::psp_texture*> textures128;
static std::vector<mutalisk::data::psp_texture*> textures16;

static const float dotsStartTime = 5.1f;
static const float dotsEndTime = 5.88f;
static const float dotsFlashTime = 0.26f;


typedef struct
{
	u16 x;
	u16 y;
	mutalisk::data::psp_texture** p;
} Dot;

static Dot dots[17] = { { 1, 0, &whiteDot},
						{ 0, 1, &whiteDot},

						{ 1, 1, &redDot},

						{ 2, 1, &whiteDot},
						{ 1, 2, &whiteDot},
						{ 3, 0, &whiteDot},

						{ 3, 1, &whiteDot},

						{ 4, 1, &whiteDot},
						{ 3, 2, &whiteDot},

						{ 5, 1, &whiteDot},
						{ 4, 2, &whiteDot},
						{ 6, 0, &whiteDot},

						{ 6, 1, &whiteDot},
						{ 5, 2, &whiteDot},

						{ 6, 2, &whiteDot},

						{ 7, 2, &whiteDot},

						{ 8, 2, &blueDot}};


bool loadIntro(const std::string& prefix)
{
	centerDot = loadTexture(prefix + "o2.mtx");
	whiteDot = loadTexture(prefix + "v.mtx");
	blueDot = loadTexture(prefix + "b.mtx");
	redDot = loadTexture(prefix + "o.mtx");
	tblTextA = loadTexture(prefix + "tA.mtx");
	tblTextB = loadTexture(prefix + "tB.mtx");
	pspLogoA = loadTexture(prefix + "pspA.mtx");
	pspLogoB = loadTexture(prefix + "pspB.mtx");
	if (!loadAnim(prefix + "loop128.bin", textures128))
	{
		printf("error loading 128 loop\n");
		return false;
	}
	if (!loadAnim(prefix + "loop16.bin", textures16))
	{
		printf("error loading 128 loop\n");
		return false;
	}
	{
		int i;
		for( i = 0; i < 17; ++i)
		{
			dots[i].x = 166 + dots[i].x * (whiteDot->width - 1);
			dots[i].y = 175 + dots[i].y * (whiteDot->height - 1);
		}
	}

	return true;
}

void unloadIntro()
{
	delete centerDot; centerDot = 0;
	delete whiteDot; whiteDot = 0;
	delete blueDot; blueDot = 0;
	delete redDot; redDot = 0;
	delete tblTextA; tblTextA = 0;
	delete tblTextB; tblTextB = 0;
	delete pspLogoA; pspLogoA = 0;
	delete pspLogoB; pspLogoB = 0;
	for (uint32_t i = 0; i < textures128.size(); ++i)
	{
		delete textures128[i];
		delete textures16[i];
	}
}

mutalisk::data::psp_texture* loadTexture(const std::string& name)
{
	int fd = sceIoOpen(name.c_str(), PSP_O_RDONLY, 0777);

	if (fd < 0)
	{
		printf("unable to open file %s\n", name.c_str());
	}
	int size = sceIoLseek(fd, 0, PSP_SEEK_END);
	sceIoLseek(fd, 0, PSP_SEEK_SET);
	mutalisk::data::MtxHeader* mtxPtr = (mutalisk::data::MtxHeader*)malloc(size);
	sceIoRead(fd, mtxPtr, size);

	mutalisk::data::psp_texture* texture = new mutalisk::data::psp_texture();
	texture->patchupTextureFromMemory(mtxPtr);
	return texture;
}

int loadAnim(const std::string name, std::vector<mutalisk::data::psp_texture*>& textures)
{
	int fd = sceIoOpen(name.c_str(), PSP_O_RDONLY, 0777);

	if (fd < 0)
	{
		printf("unable to open file\n");
	}

	uint32_t frames;
	uint32_t width;
	uint32_t height;

	sceIoRead(fd, &frames, 4);
	sceIoRead(fd, &width, 4);
	sceIoRead(fd, &height, 4);

	printf("frames = %i\n", frames);
	printf("width = %i\n", width);
	printf("height = %i\n", height);

	uint32_t frameSizeUnpacked = width * height;
	uint32_t frameSizePacked = width * height / 4;
	uint32_t mtxSize = frameSizeUnpacked + sizeof(mutalisk::data::MtxHeader) + sizeof(uint32_t) * 8;

	uint32_t* framePtr = (uint32_t*)malloc(frameSizePacked);
	uint32_t* tempPtr = (uint32_t*)malloc(frameSizeUnpacked);

	textures.resize(0);
	textures.reserve(frames);

	for (uint32_t i = 0; i < frames; ++i)
	{
		sceIoRead(fd, framePtr, frameSizePacked);
		uncompressFrame(framePtr, tempPtr, frameSizePacked / 4);

		mutalisk::data::MtxHeader* mtxPtr = (mutalisk::data::MtxHeader*)malloc(mtxSize);
		memset(mtxPtr, 0x00, sizeof(mutalisk::data::MtxHeader));
		mtxPtr->signature		= mutalisk::data::MtxHeader::Signature_CurrentVersion;
		mtxPtr->pixelFormat		= GU_PSM_T8;
		mtxPtr->textureWidth	= width;
		mtxPtr->textureHeight	= height;
		mtxPtr->textureStride	= width;
		mtxPtr->vramAllocationSize = frameSizeUnpacked;
		mtxPtr->clutFormat		= GU_PSM_8888;
		mtxPtr->clutEntries		= 1;
		mtxPtr->paletteOffset	= frameSizeUnpacked;

		//swizzle here
		memcpy(mtxPtr+1, tempPtr, (width * height));
		uint32_t* palette = (uint32_t*)((uint32_t)(mtxPtr+1) + frameSizeUnpacked);
		memset(palette, 0x00, sizeof(uint32_t)*8);
		*palette++ = 0xff000000;
		*palette++ = 0xff555555;
		*palette++ = 0xffaaaaaa;
		*palette++ = 0xffffffff;
//		*palette++ = 0xffffffff;

		mutalisk::data::psp_texture* texture = new mutalisk::data::psp_texture();
		texture->patchupTextureFromMemory(mtxPtr);
		textures.push_back(texture);
	}

	sceIoClose(fd);
	return frames;
}

void uncompressFrame(uint32_t* in, uint32_t* out, int size)
{
/*
	original: 000000aa 000000bb 000000cc 000000dd
	compress: aabbccdd eeffgghh iijjkkll mmnnoopp
	swizzled: aaeeiimm bbffjjnn ccggkkoo ddhhllpp
	decomprs: 0aa00000 0bb00000 0cc00000 0dd00000
*/
	for(int i = 0; i < size; ++i)
	{
		uint32_t input = *in++;
		*out++ = ((input & 0xc0c0c0c0) >> 6);
		*out++ = ((input & 0x30303030) >> 4);
		*out++ = ((input & 0x0c0c0c0c) >> 2);
		*out++ = ((input & 0x03030303));
	}
}

void drawQuad(mutalisk::data::psp_texture& texture, uint32_t x, uint32_t y, int blurSrcModifier = 200, int blurDstModifier = 160);
void renderIntro(float time)
{
	float out = 1.f;
	if (time > 6.5)
		out = 1.f - (time-6.5);
	if (out < 0.f)
		out = 0.f;
	{
		float t = time;
		if (t > 1.f)
			t = 1.f;
		t *= out;
		static size_t i = 0;
		if (++i == textures128.size())
			i = 0;
		drawQuad(*textures128[i], 168,     48, 200 * t, 160 * t);
		drawQuad(*textures16[i],  168+128, 48, 200 * t, 160 * t);
	}
	{
		float t = time * 2;
		if (t > 1.f)
			t = 1.f;
		t *= out;
		drawQuad(*centerDot, 168+49, 88, 0xff * t, 0x00);
	}
	{
		static int evenFrame = 0;
		evenFrame++;
		drawDots(time+3.f, (evenFrame >> 2)&1, out);
	}

	if (time > 3.5f)
	{
		float t = time - 3.5f;
		if (t > 1.f)
			t = 1.f;
		t *= out;
		unsigned alpha = 0xff * t;
		drawQuad(*tblTextA, 160,     225, alpha, 0x00);
		drawQuad(*tblTextB, 160+128, 225, alpha, 0x00);
	}
}

void renderPSPLogo(float time)
{
	sceGuClearColor(0xff000000);
	sceGuClearDepth(0xffff);
	sceGuClear(GU_COLOR_BUFFER_BIT|GU_DEPTH_BUFFER_BIT);

	if (time > 9.5f)
		time -= (time - 9.5f) * 2;

	float t = time - 8.f;
	if (t > 1.f)
		t = 1.f;
	else if (t < 0.f)
		t = 0.f;

	drawQuad(*pspLogoA, 90,     72, 0xff * t, 0x00);
	drawQuad(*pspLogoB, 90+256, 72, 0xff * t, 0x00);
}


void drawDots(float time, int evenFrame, float fade)
{
	int i;
	int w = whiteDot->width;
	int h = whiteDot->height;

	float t = (time - dotsStartTime) / (dotsEndTime - dotsStartTime);
	float t1 = t - dotsFlashTime;
	if (t < 0.f)		t = 0.f;
	else if (t > 1.f)	t = 1.f;
	if (t1 < 0.f)		t1 = 0.f;
	else if (t1 > 1.f)	t1 = 1.f;

	int visibleDots = (int)((float)(sizeof(dots) / sizeof(Dot)) * t);
	int flashDots = (int)((float)(sizeof(dots) / sizeof(Dot)) * t1);
	static int lastFlashDots = 0;
	for( i = 0/*lastFlashDots*/; i < visibleDots; ++i)
	{
		int x = dots[i].x;
		int y = dots[i].y;
		if (i < 0 || i> 16)
			continue;
		mutalisk::data::psp_texture* v = *dots[i].p;
		if (i < flashDots || (evenFrame ^ (i & 0x01)))
			drawQuad(*v, x, y, 0xff * fade, 0x00);
		else
			drawQuad(*v, x, y, 0x00, 0x00);
	}
	lastFlashDots = flashDots - 1;
}


void drawQuad(mutalisk::data::psp_texture& texture, uint32_t x, uint32_t y, int blurSrcModifier, int blurDstModifier)
{
//	int blurSrcModifier = 200;
//	int blurDstModifier = 160;
uint32_t color = 0x20ffffff;
					sceGuEnable(GU_BLEND);
					unsigned int srcFix = GU_ARGB(0, blurSrcModifier, blurSrcModifier, blurSrcModifier);
					unsigned int dstFix = GU_ARGB(0, blurDstModifier, blurDstModifier, blurDstModifier);
			if (blurDstModifier != 0x00)
					sceGuBlendFunc(GU_ADD, GU_FIX, GU_FIX, srcFix, dstFix);
			else
			{
					sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);
					color = GU_ARGB(blurSrcModifier, blurSrcModifier, blurSrcModifier, blurSrcModifier);
			}
//	sceGuAlphaFunc(GU_ALWAYS, 0,0);
	sceGuAmbientColor(~0U);
	sceGuTexMode(texture.format,0,0,texture.swizzled);
	sceGuTexImage(texture.mipmap,texture.width,texture.height,texture.stride,texture.data);
	sceGuTexFilter(GU_LINEAR,GU_LINEAR);
	sceGuTexWrap(GU_CLAMP, GU_CLAMP);
//	sceGuTexFunc(GU_TFX_MODULATE,GU_TCC_RGB);
	sceGuTexFunc(GU_TFX_MODULATE,GU_TCC_RGBA);
//	sceGuTexScale(1.0f,1.0f);
	sceGuEnable(GU_TEXTURE_2D);
	sceGuColor(color);
	sceGuDisable(GU_LIGHTING);
	sceGuDisable(GU_DEPTH_TEST);
	sceGuDepthMask(1);
	sceGuClutMode(texture.clutFormat,0,0xff,0);
	sceGuClutLoad(texture.clutEntries,texture.clut);
	
	struct QuadVertexTex
	{
		float u, v;
		short x,y,z;
	};
	QuadVertexTex* vertices = reinterpret_cast<QuadVertexTex*>(sceGuGetMemory(2 * sizeof(QuadVertexTex)));
	vertices[0].u = 0; vertices[0].v = 0;
	vertices[0].x = x; vertices[0].y = y; vertices[0].z = 0;
	vertices[1].u = texture.width;
	vertices[1].v = texture.height;
	vertices[1].x = vertices[0].x + texture.width; vertices[1].y = vertices[0].y + texture.height; vertices[1].z = 0;

	sceGuDrawArray(GU_SPRITES,GU_TEXTURE_32BITF|GU_VERTEX_16BIT|GU_TRANSFORM_2D,2,0,vertices);

	sceGuDepthMask(0);
}
