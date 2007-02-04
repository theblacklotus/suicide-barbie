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
		enum { Version = 0x0110 };

		typedef std::string Ref;
		struct Node {
			Ref nodeName;
			Mat16 worldMatrix;
		};
		struct Light : parent<Node> {
			enum Type {
				Directional, Spot, Point
//				DirectionalExt
			};
			Type type;
			Color ambient;
			Color diffuse;
			Color specular;
			Vec3 attenuation;
			float theta;
			float phi;
		};
		struct Camera : parent<Node> {
		};
		struct Actor : parent<Node> {
			struct Material {
				Color ambient;
				Color diffuse;
				Color specular;
				unsigned textureIndex;
				unsigned shaderIndex;
			};
			unsigned meshIndex;
			array<Material> materials;
		};

//		size_t meshCount;
		array<Ref> meshIds;

//		size_t meshCount;
		array<Ref> textureIds;

		array<Ref> shaderIds;

//		size_t lightCount;
		array<Light> lights;

//		size_t cameraCount;
		array<Camera> cameras;
		unsigned defaultCameraIndex;

//		size_t actorCount;
		array<Actor> actors;

		Ref animCharId;
		unsigned defaultClipIndex;

		// memory management
		scene(); ~scene();
	};

	// I/O
	template <typename In> In& operator>> (In& i, scene::Node& data);
	template <typename Out> Out& operator<< (Out& o, scene::Node const& data);
	template <typename In> In& operator>> (In& i, scene& data);
	template <typename Out> Out& operator<< (Out& o, scene const& data);

	// memory management
	void clear(scene& data);

} // namespace data 
} // namespace mutalisk

#include "scene.inl"

#endif // MUTALISK_DATA_SCENE_H_
