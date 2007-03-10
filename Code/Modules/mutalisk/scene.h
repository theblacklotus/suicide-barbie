#ifndef MUTALISK_DATA_SCENE_H_
#define MUTALISK_DATA_SCENE_H_

#include "common.h"
#include "shader.h"
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
		enum { Version = 0x0117 };

		typedef std::string Ref;
		typedef unsigned Id;
		struct Node {
			Id id;
			Ref nodeName;
			Mat16 worldMatrix;
		};
		struct Light : parent<Node> {
			enum nType {
				Directional, Spot, Point,
				DirectionalExt
			};
			nType type;
			Color ambient;
			Color diffuse;
			Color specular;
			Color diffuseAux0;
			Color diffuseAux1;
			Vec3 attenuation;
			float theta;
			float phi;
		};
		struct Camera : parent<Node> {
		};
		struct Actor : parent<Node> {
			struct Material {
				unsigned shaderIndex;
				//unsigned inputIndex;
				shader_fixed shaderInput;
			};
			unsigned meshIndex;
			array<Material> materials;
		};

		array<Ref> meshIds;
		array<Ref> textureIds;

		unsigned shaderLibraryVersion;
		// array<shader_fixed> shaderInputs;

		array<Light> lights;
		array<Camera> cameras;
		unsigned defaultCameraIndex;

		array<Actor> actors;

		Ref animCharId;
		unsigned defaultClipIndex;

		// memory management
		scene();
	};

	// I/O
	template <typename In> In& operator>> (In& i, scene::Node& data);
	template <typename Out> Out& operator<< (Out& o, scene::Node const& data);
	template <typename In> In& operator>> (In& i, scene& data);
	template <typename Out> Out& operator<< (Out& o, scene const& data);

} // namespace data 
} // namespace mutalisk

#include "scene.inl"

#endif // MUTALISK_DATA_SCENE_H_
