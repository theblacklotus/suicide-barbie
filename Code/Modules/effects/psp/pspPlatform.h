#ifndef MUTALISK_EFFECTS__PSP_PLATFORM_H_
#define MUTALISK_EFFECTS__PSP_PLATFORM_H_

#include "../cfg.h"
#include <psptypes.h>
#include <pspgu.h>
#include <pspgum.h>
#include <mutalisk/psp/pspPlatform.h>
#include <mutalisk/scene.h>

namespace mutalisk { namespace effects {

	typedef mutalisk::data::scene::Light	LightT;
	typedef data::Texture					TextureT;
	typedef ScePspFMatrix4					MatrixT;
	typedef ScePspFVector4					VecT;
	typedef unsigned						ColorT;

} // namespace effects 
} // namespace mutalisk

#endif // MUTALISK_EFFECTS__PSP_PLATFORM_H_
