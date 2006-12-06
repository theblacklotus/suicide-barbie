#ifndef MUTALISK_DATA_SCENE_H_
#define MUTALISK_DATA_SCENE_H_

#include "common.h"
#include <string>

// scene
//  + addMesh
//  + addLight
//  ...
//
// scene [blit()]=> simple_scene
// simple_scene [>>]=> ostream
// ...
// istream [<<]=> simple_scene
// simple_scene [prepare()]=> renderable_scene
// scene_renderer(renderable_scene)


namespace mutalisk { namespace data
{

	struct scene
	{
		typedef std::string Ref;
		struct Node {
			Ref nodeName;
			Mat16 worldMatrix;
		};
		struct Light : Node {
			enum Type {
				Directional, Spot, Point
			};
			Type type;
			Color ambient;
			Color diffuse;
			Color specular;
			Vec3 attenuation;
			float theta;
			float phi;
		};
		struct Camera : Node {
		};
		struct Actor : Node {
			unsigned meshIndex;
		};

		size_t meshCount;
		Ref* meshIds;

		size_t lightCount;
		Light* lights;

		size_t cameraCount;
		Camera* cameras;
		unsigned defaultCameraIndex;

		size_t actorCount;
		Actor* actors;

		Ref animCharId;
		unsigned defaultClipIndex;

		// memory management
		scene(); ~scene();
	};

	// I/O
	template <typename In> In& operator>> (In& i, scene& data);
	template <typename Out> Out& operator<< (Out& o, scene const& data);

	// memory management
	void clear(scene& data);

} // namespace data 
} // namespace mutalisk

#include "scene.inl"

#endif // MUTALISK_DATA_SCENE_H_
