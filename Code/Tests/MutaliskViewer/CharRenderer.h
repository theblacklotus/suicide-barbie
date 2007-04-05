#ifndef CHARRENDERER_H
#define CHARRENDERER_H

#include "cfg.h"
#include <player/platform.h>
#include <player/ScenePlayer.h>

namespace mutalisk
{
	void prepareChars(mutalisk::RenderableScene& scene);
	void renderChars(mutalisk::RenderableScene const& scene);
}

#endif // CHARRENDERER_H
