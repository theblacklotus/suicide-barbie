#ifndef MUTANT_TYPE_NAMES_H_
#define MUTANT_TYPE_NAMES_H_

namespace mutant
{
	struct sTypeNames
	{
		static const char* VISIBILITY;

		static const char* TRANSLATE_X;
		static const char* TRANSLATE_Y;
		static const char* TRANSLATE_Z;

		static const char* SCALE_X;
		static const char* SCALE_Y;
		static const char* SCALE_Z;

		static const char* ROTATE_X;
		static const char* ROTATE_Y;
		static const char* ROTATE_Z;

		// translation, rotation, scale
		static const char* VEC_QUAT_VEC;

		// translation, rotation
		static const char* VEC_QUAT;

		// main marker track
		static const char* MARKER_MAIN;

		// hierarchy names
		static const char* HIERARCHY_DEFAULT;
	};
}

#endif // MUTANT_TYPE_NAMES_H_
