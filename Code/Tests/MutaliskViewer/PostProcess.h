#ifndef POST_PROCESS_H_
#define POST_PROCESS_H_

#include "cfg.h"
#include <memory>
#include <pspkernel.h>

namespace mutalisk
{
	void pushState();
	void popState();

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

	struct Region
	{
		ScePspFVector2 offset;
		ScePspFVector2 scale;
	};

	void setTexture(Texture const& texture);
	void setSampler(Sampler const& sampler);
	void setRenderTarget(Texture const& renderTarget);

	void allocPostProcessTargets(int format, int width, int height,
		std::auto_ptr<Texture>& renderTarget0, std::auto_ptr<Texture>& renderTarget1);

	void drawFullscreenQuad(unsigned int color, unsigned int vertexElements = 0);
	void drawFullscreenQuad(Texture const& texture, Sampler const& sampler, Region const& uvRegion, unsigned int color);
	void drawFullscreenQuad(Texture const& texture, Sampler const& sampler, ScePspFVector2 offset, unsigned int color);
	void drawFullscreenQuad(Texture const& texture, Sampler const& sampler, ScePspFVector2 offset);
	void drawFullscreenQuad(Texture const& texture, Sampler const& sampler, Region const& uvRegion);
	void drawFullscreenQuad(Texture const& texture, Sampler const& sampler);

	void gpuBlur(Texture& srcRenderTarget, Texture& dstRenderTarget, float blur);

}

#endif // POST_PROCESS_H_
