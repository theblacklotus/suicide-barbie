#ifndef NEWAGE_SCENEPLAYER_H_
#define NEWAGE_SCENEPLAYER_H_

#include "cfg.h"
#include <cstring>
#include <memory>
#include <mutant/mutant.h>
#include <mutant/io_factory.h>
#include <mutant/reader.h>

#include <mutalisk/array.h>
#include <mutalisk/platform.h>
#include <mutalisk/mutalisk.h>

#include "Animators.h"
#include "AnimatorAlgos.h"

namespace mutalisk
{
#ifndef AP
#define AP_DEFINED_LOCALY
#define AP std::auto_ptr
#endif
void setMatrix(CTransform::t_matrix& matrix, float const* worldMatrixData);

struct RenderableMesh;
struct RenderableTexture;
struct CSkinnedAlgos
{
	typedef std::vector<std::pair<int, int> > BoneMapT;
	static void processSkinMesh(RenderableMesh& mesh, BoneMapT const& boneMap, CTransform::t_matrix const* data);
	static void processSkinMesh(Vec3 const* srcPositions, Vec3 const* srcNormals, float const* srcWeights, unsigned char const* srcBoneIndices,
		Vec3 *dstPositions, Vec3* dstNormals,
		size_t srcVertexStride, size_t srcWeightStride, size_t srcBoneIndexStride, size_t dstVertexStride, size_t vertexCount,
		mutalisk::data::skin_info const& skinInfo, BoneMapT const& boneMap, CTransform::t_matrix const* matrixData);
};

struct RenderableScene
{
	struct SharedResources {
		struct Mesh {
			AP<mutalisk::data::mesh>	blueprint;
			AP<RenderableMesh>			renderable;
		};
		struct Texture {
			AP<mutalisk::data::texture>	blueprint;
			AP<RenderableTexture>		renderable;
		};
		mutalisk::array<Mesh>			meshes;
		mutalisk::array<Texture>		textures;
		AP<mutant::anim_character_set>	animCharSet;
	};
	struct State {
		float							time;
		mutant::anim_character*			character;
		mutant::anim_hierarchy*			hierarchy;
		mutant::anim_clip*				clip;

//		BoneMapT						boneMap;
		CTransformArrayAnimator			xformArrayAnimator;

		typedef std::vector<CTransform> TransformsT;
		typedef std::vector<CTransform::t_matrix> MatricesT;
		TransformsT						transforms;
		MatricesT						matrices;

		unsigned						activeCameraIndex;
		CTransform::t_matrix			projMatrix;
		CTransform::t_matrix			cameraMatrix;

		std::vector<size_t>				light2XformIndex;
		std::vector<size_t>				camera2XformIndex;
		std::vector<size_t>				actor2XformIndex;

		typedef CSkinnedAlgos::BoneMapT	BoneMapT;
		std::vector<BoneMapT>			bone2XformIndex;


		void setClip(mutant::anim_character_set& animCharSet, unsigned int clipIndex, bool looping)
		{
			this->character = &animCharSet["scene"];
			ASSERT(this->character);
			this->hierarchy = &this->character->hierarchy(mutant::sTypeNames::HIERARCHY_DEFAULT);
			ASSERT(this->hierarchy);

			this->transforms.resize(this->hierarchy->size(), CTransform::identity());
			this->matrices.resize(this->hierarchy->size());

			if(clipIndex != ~0U)
			{
				this->clip = &(*this->character)[clipIndex];
				ASSERT(this->clip);
				this->xformArrayAnimator.createFromClip(
					*this->clip,
					*this->hierarchy,
					looping);
			}
			else
				this->clip = 0;

			this->time = 0.0f;
		}
		bool hasAnimation(std::string const& actorName, std::string const& channelName) const
		{
			ASSERT(this->clip);
			if(!this->clip->has(actorName))
				return false;
			if(!(*this->clip)[actorName].has_ff(channelName))
				return false;
			return true;
		}
		float sampleAnimation(std::string const& actorName, std::string const& channelName, float t, float defaultValue = 0.0f) const
		{
			if(!hasAnimation(actorName, channelName))
				return defaultValue;

			typedef knot_data<float,float> t_float_knots;
			typedef linear_evaluator<float> t_float_eval;		
			typedef interpolator1<t_float_knots, t_float_eval, time_algo_cycle> t_interpolator;
			t_interpolator ipol((*this->clip)[actorName].floatFloat(channelName), t_float_eval());
			return ipol.value(t);
		}

		void updateDt(mutalisk::data::scene const& scene, float deltaTime)
		{
			this->time += deltaTime;
			update(scene, this->time);
		}

		void update(mutalisk::data::scene const& scene, float time)
		{
			this->time = time;
			this->xformArrayAnimator.updateTransforms(this->time,
				this->transforms.begin(), this->transforms.end());
		}

		/*CTransform::t_matrix calcProjectionMatrix(float fov, float aspect)
		{
		}*/

		void processActiveCamera(mutalisk::data::scene const& blueprint)
		{
			static bool animatedActors = true;//gSettings.forceAnimatedActors;
			static bool animatedCamera = true;//gSettings.forceAnimatedCamera;

			if(this->activeCameraIndex == ~0U)
			{
				animatedActors = false;
				animatedCamera = false;
			}

			if(!animatedActors)
				for(size_t q = 0; q < this->actor2XformIndex.size(); ++q)
				{
					setMatrix(
						this->matrices[this->actor2XformIndex[q]],
						blueprint.actors[q].worldMatrix.data);
				}

			if(!animatedCamera)
				for(size_t q = 0; q < this->camera2XformIndex.size(); ++q)
				{
					setMatrix(
						this->matrices[this->camera2XformIndex[q]],
						blueprint.cameras[q].worldMatrix.data);
				}

			if(this->activeCameraIndex != ~0U)
			{
				/*ASSERT(this->activeCameraIndex >= 0 && this->activeCameraIndex < this->camera2XformIndex.size());
				this->projMatrix = 
					calcProjectionMatrix(
						blueprint.cameras[this->activeCameraIndex].fov,
						blueprint.cameras[this->activeCameraIndex].aspect);*/

				if(animatedCamera)
				{
					ASSERT(this->activeCameraIndex >= 0 && this->activeCameraIndex < this->camera2XformIndex.size());
					this->cameraMatrix = this->matrices[this->camera2XformIndex[this->activeCameraIndex]];
				}
				else
				{
					ASSERT(this->activeCameraIndex >= 0 && this->activeCameraIndex < blueprint.cameras.size());
					setMatrix(
						this->cameraMatrix,
						blueprint.cameras[this->activeCameraIndex].worldMatrix.data);
				}
			}
		}

		void process(mutalisk::data::scene const& blueprint, SharedResources& sharedResources)	
		{
//;;printf("process -- 0\n");
			ASSERT(this->hierarchy);
//;;printf("process -- 1\n");
			CAnimatorAlgos::transformHierarchy(
				this->matrices.begin(), this->matrices.end(),
				this->transforms.begin(), *this->hierarchy );
//;;printf("process -- 2\n");

			for(size_t q = 0; q < this->bone2XformIndex.size(); ++q)
				if( !this->bone2XformIndex[q].empty() )
				{
//;;printf("process -- processSkinMesh0\n");
					ASSERT(sharedResources.meshes[q].renderable.get());
					CSkinnedAlgos::processSkinMesh(*sharedResources.meshes[q].renderable, this->bone2XformIndex[q], &this->matrices[0]);
//;;printf("process -- processSkinMesh0\n");
				}
//;;printf("process -- 3\n");

			processActiveCamera(blueprint);
//;;printf("process -- 4\n");
		}
	};

	RenderableScene(mutalisk::data::scene const& blueprint) : mBlueprint(blueprint) {}

	mutalisk::data::scene const&	mBlueprint;
	SharedResources					mResources;
	State							mState;

	template <typename Node>
	void mapNodesToHierarchy(mutant::anim_hierarchy const& hierarchy, Node	const* nodes, size_t nodeCount, std::vector<size_t>& node2XformIndex)
	{
		node2XformIndex.resize(nodeCount);
		for(size_t q = 0; q < nodeCount; ++q)
			node2XformIndex[q] = hierarchy.index_by_name(nodes[q].nodeName);
	}

	void mapBoneToHierarchy(mutant::anim_hierarchy const& hierarchy,
		mutalisk::data::skin_info::Bone* bones, size_t boneCount, State::BoneMapT& bone2XformIndex )
	{
		bone2XformIndex.reserve( boneCount );

		for( size_t q = 0; q < boneCount; ++q )
		{
			unsigned boneId = 0;
			std::string const& boneName = bones[q].name;

			while( boneId < hierarchy.size() && hierarchy[ boneId ].name != boneName )
				++boneId;

			if( boneId == hierarchy.size() )
				printf( "Couldn't find '%s' in hierarchy", boneName.c_str() );
			else
				bone2XformIndex.push_back( std::make_pair( (int)q, boneId ) );
		}
	}


	void setClip(unsigned int clipIndex, bool looping = true)
	{
		ASSERT(mResources.animCharSet.get());
		mState.setClip(*mResources.animCharSet, clipIndex, looping);

		ASSERT(mState.hierarchy);
		if(!mBlueprint.lights.empty())
			mapNodesToHierarchy(*mState.hierarchy, &mBlueprint.lights[0], mBlueprint.lights.size(), mState.light2XformIndex);
		if(!mBlueprint.cameras.empty())
			mapNodesToHierarchy(*mState.hierarchy, &mBlueprint.cameras[0], mBlueprint.cameras.size(), mState.camera2XformIndex);
		if(!mBlueprint.actors.empty())
			mapNodesToHierarchy(*mState.hierarchy, &mBlueprint.actors[0], mBlueprint.actors.size(), mState.actor2XformIndex);
		
		mState.bone2XformIndex.resize(mBlueprint.meshIds.size());
		for(size_t q = 0; q < mBlueprint.meshIds.size(); ++q)
			if( mResources.meshes[q].blueprint->skinInfo )
				mapBoneToHierarchy(*mState.hierarchy,
					&mResources.meshes[q].blueprint->skinInfo->bones[0], mResources.meshes[q].blueprint->skinInfo->bones.size(),
					mState.bone2XformIndex[q]);

		mState.activeCameraIndex = mBlueprint.defaultClipIndex;
		mState.cameraMatrix = CTransform::identityMatrix();
	}
	unsigned int setActiveCameraIndex(unsigned int cameraIndex)
	{
		std::swap(cameraIndex, mState.activeCameraIndex);
		mState.processActiveCamera(mBlueprint);
		return cameraIndex;
	}
	void update(float time) { mState.update(mBlueprint, time); }
	void process() { mState.process(mBlueprint, mResources); }
};

void setResourcePath(std::string const& path);
std::string getResourcePath();
AP<mutant::mutant_reader> createFileReader(std::string const& fileName);

template <typename ResourceType>
static AP<ResourceType> loadResource(std::string fileName)
{
	;;printf("loadResource<>: $ %s\n", fileName.c_str());
	AP<mutant::mutant_reader> reader = createFileReader(fileName);
	AP<ResourceType> resource(new ResourceType);
	*reader >> *resource;
	;;printf("loadResource<>: ! %s\n", fileName.c_str());
	return resource;
}

template <>
inline AP<mutant::anim_character_set> loadResource(std::string fileName)
{
	;;printf("loadResource<anim_character_set>: $ %s\n", fileName.c_str());
	AP<mutant::mutant_reader> reader = createFileReader(fileName);
	AP<mutant::anim_character_set> resource(new mutant::anim_character_set);
	reader->read(*resource);
	;;printf("loadResource<anim_character_set>: ! %s\n", fileName.c_str());
	return resource;
}

template <>
inline AP<mutalisk::data::texture> loadResource<mutalisk::data::texture>(std::string fileName)
{
	;;printf("loadResource<mutalisk::data::texture>: $ %s\n", fileName.c_str());
	AP<mutant::binary_input> input = AP<mutant::binary_input>(new file_input(getResourcePath() + fileName));
	AP<mutant::mutant_reader> reader(new mutant::mutant_reader(input));
	reader->enableLog(false);

	AP<mutalisk::data::texture> resource(new mutalisk::data::texture);
	*reader >> *resource;
	;;printf("loadResource<mutalisk::data::texture>: ! %s\n", fileName.c_str());
	return resource;
}

#ifdef AP_DEFINED_LOCALY
#undef AP
#endif
} // namespace mutalisk

#endif // NEWAGE_SCENEPLAYER_H_
