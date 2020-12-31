#include "PostProcess.h"

#include <vector>
#include <math.h>
#include <pspgu.h>
#include <pspgum.h>
#include "vram.h"

namespace mutalisk
{

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
void setRenderTarget(Texture const& renderTarget)
{
	sceGuDrawBufferList(renderTarget.format,renderTarget.vramAddr,renderTarget.stride);
	sceGuOffset(2048 - (renderTarget.width/2),2048 - (renderTarget.height/2));
	sceGuViewport(2048,2048,renderTarget.width,renderTarget.height);
	sceGuScissor(0,0,renderTarget.width,renderTarget.height);

	gViewportWidth = renderTarget.width;
	gViewportHeight = renderTarget.height;
}

void allocPostProcessTargets(int format, int width, int height,
	std::auto_ptr<Texture>& renderTarget0, std::auto_ptr<Texture>& renderTarget1)
{
	int stride = width;
	renderTarget0.reset(new Texture());
	renderTarget0->format = format;
	renderTarget0->mipmap = 0;
	renderTarget0->width = width;
	renderTarget0->height = height;
	renderTarget0->stride = stride;
	renderTarget0->vramAddr = getStaticVramBuffer(stride, height, format);
	renderTarget0->data = mapVramBufferToTexture(renderTarget0->vramAddr);

	renderTarget1.reset(new Texture());
	*renderTarget1.get() = *renderTarget0.get();
	renderTarget1->vramAddr = getStaticVramBuffer(stride, height, format);
	renderTarget1->data = mapVramBufferToTexture(renderTarget1->vramAddr);
}

void drawFullscreenQuad(unsigned int color, unsigned int vertexElements)
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
	struct QuadVertexTex2
	{
		float u, v;
		unsigned short x,y,z;
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
	{
		QuadVertexTex* vertices = reinterpret_cast<QuadVertexTex*>(sceGuGetMemory(2 * 32 * sizeof(QuadVertexTex)));

		short sx = 0;
		short sliceW = 32;
		float su = 0.0f;
		float sliceU = ceilf((float)sliceW / (float)gViewportWidth);
		int vertexCount = 0;
		for(; sx < gViewportWidth; sx += sliceW)
		{
			if(sx + sliceW > gViewportWidth)
				sliceW = gViewportWidth - sx;

			if(su + sliceU > 1.0f)
				sliceU = 1.0f - su;

			vertices[vertexCount].u = su;
			vertices[vertexCount].v = 0;
			vertices[vertexCount].x = sx;
			vertices[vertexCount].y = 0;
			vertices[vertexCount].z = 0;
			++vertexCount;
			vertices[vertexCount].u = su + sliceU;
			vertices[vertexCount].v = 1;
			vertices[vertexCount].x = sx + sliceW;
			vertices[vertexCount].y = gViewportHeight;
			vertices[vertexCount].z = 0;
			++vertexCount;
		}
		sceGuDrawArray(GU_SPRITES,GU_TEXTURE_32BITF|GU_VERTEX_16BIT|GU_TRANSFORM_2D,vertexCount,0,vertices);
	}
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

/*
	QuadVertexTex* vertices = reinterpret_cast<QuadVertexTex*>(sceGuGetMemory(2 * sizeof(QuadVertexTex)));
	vertices[0].u = 0 + uvRegion.offset.x; vertices[0].v = 0 + uvRegion.offset.y;
	vertices[0].x = 0; vertices[0].y = 0; vertices[0].z = 0;

	vertices[1].u = texture.width * uvRegion.scale.x + uvRegion.offset.x;
	vertices[1].v = texture.height * uvRegion.scale.y + uvRegion.offset.y;
	vertices[1].x = gViewportWidth; vertices[1].y = gViewportHeight; vertices[1].z = 0;

	sceGuDrawArray(GU_SPRITES,GU_TEXTURE_32BITF|GU_VERTEX_16BIT|GU_TRANSFORM_2D,2,0,vertices);*/

	QuadVertexTex* vertices = reinterpret_cast<QuadVertexTex*>(sceGuGetMemory(2 * 32 * sizeof(QuadVertexTex)));

	short sx = 0;
	short sliceW = 32;
	float su = 0.0f;
	float sliceU = 1.0f / ceilf(
		(((float)gViewportWidth / (float)sliceW))
		);
	int vertexCount = 0;
	for(; sx < gViewportWidth; sx += sliceW)
	{
		if(sx + sliceW > gViewportWidth)
			sliceW = gViewportWidth - sx;

		if(su + sliceU > 1.0f)
			sliceU = 1.0f - su;

		vertices[vertexCount].u = ((su)	* texture.width * uvRegion.scale.x) + uvRegion.offset.x;
		vertices[vertexCount].v = uvRegion.offset.y;
		vertices[vertexCount].x = sx;
		vertices[vertexCount].y = 0;
		vertices[vertexCount].z = 0;
		++vertexCount;
		su += sliceU;
		vertices[vertexCount].u = ((su)	* texture.width * uvRegion.scale.x) + uvRegion.offset.x;
		vertices[vertexCount].v = texture.height * uvRegion.scale.y + uvRegion.offset.y;
		vertices[vertexCount].x = sx + sliceW;
		vertices[vertexCount].y = gViewportHeight;
		vertices[vertexCount].z = 0;
		++vertexCount;
	}
	sceGuDrawArray(GU_SPRITES,GU_TEXTURE_32BITF|GU_VERTEX_16BIT|GU_TRANSFORM_2D,vertexCount,0,vertices);

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

void gpuBlur(Texture& srcRenderTarget, Texture& dstRenderTarget, float blur)
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
	float const offsetScaler = blur * 2.0f;

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

void gpuBlurFast(Texture& srcRenderTarget, Texture& dstRenderTarget, float blur, unsigned quality)
{	
	float halfPixelW = 0.5f;
	float halfPixelH = 0.5f;
	float offsetW = halfPixelW;
	float offsetH = halfPixelH;

	float offsets[] = {
		 1.0f,
		-1.0f,
		 3.0f,
		-3.0f,
		 5.0f,
		-5.0f,
		 7.0f,
		-7.0f,
	};
	unsigned int quadCount = sizeof(offsets) / sizeof(float);
	ASSERT(quality > 1);
	ASSERT(quality <= quadCount/2);
	float const offsetScaler = blur * float(2 + quadCount/2 - quality);

	Sampler sampler;
	sampler.addressU = GU_CLAMP;
	sampler.addressV = GU_CLAMP;
	sampler.minFilter = GU_LINEAR;
	sampler.magFilter = GU_LINEAR;

	quadCount = quality * 2;
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

} //namespace mutalisk
