#ifndef BALLRENDERER_H
#define BALLRENDERER_H

#include "cfg.h"
#include <player/platform.h>
#include <player/ScenePlayer.h>

namespace mutalisk
{
	void prepareBalls(mutalisk::RenderableScene& scene);
	void renderBalls(mutalisk::RenderableScene const& scene);
}

#endif // BALLRENDERER_H
