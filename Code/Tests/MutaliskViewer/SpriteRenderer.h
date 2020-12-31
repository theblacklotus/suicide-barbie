#ifndef SPRITERENDERER_H
#define SPRITERENDERER_H

#include "cfg.h"
#include <player/platform.h>
#include <player/ScenePlayer.h>

namespace mutalisk
{
	void prepareSprites(mutalisk::RenderableScene& scene);
	void renderSprites(mutalisk::RenderableScene const& scene);
}

#endif // SPRITERENDERER_H
