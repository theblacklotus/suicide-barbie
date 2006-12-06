#include "scene.h"

namespace mutalisk { namespace data
{

// scene
//
scene::scene()
: meshCount(0)
, meshIds(0)
, lightCount(0)
, lights(0)
, cameraCount(0)
, cameras(0)
, defaultCameraIndex(~0)
, actorCount(0)
, actors(0)
, animCharId("")
, defaultClipIndex(~0)
{
}

scene::~scene()
{ 
	clear(*this);
}

void clear(scene& data)
{
	delete[] data.meshIds;
	delete[] data.lights;
	delete[] data.cameras;
	delete[] data.actors;
}

} // namespace data 
} // namespace mutalisk
