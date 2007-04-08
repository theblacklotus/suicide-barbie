#include "CharRenderer.h"
/////////////////////////////////////////////////////////////////////////////
namespace mutalisk
{
	struct Data
	{
		Vec3 pos;
		float area;
		u32 displace;
	};
	struct Ball
	{
		u32 color;
		Vec3 normal;
		Vec3 pos;
		float area;
		bool operator < (const Ball& other) const 
		{
			if (pos.z > other.pos.z)
				return false;
			else
				return true;
		}
	};

namespace
{
	void prepareChars(mutalisk::RenderableMesh* mesh)
	{
		ASSERT(mesh);
		mutalisk::data::mesh const& data = mesh->mBlueprint;

		;;printf("using \"char render\"(tm) technique\n");

		// this is what the input must look like, and what the output will look like.. simple, huh?
		struct Vertex
		{
			float u, v;
			Vec3 normal;
			Vec3 pos;
		};

		if (data.primitiveType != GU_TRIANGLES) 
		{
			printf("ASSERT: input needs to be triangle list!\n");
			assert(false);
		}

		if (data.vertexStride != sizeof(Vertex))
		{
			printf("ASSERT: vertex does not match ball render vertex!\n");
			assert(false);
		}

		if(data.indexData == 0 /*|| (data.vertexDecl & GU_INDEX_16BIT) == 0*/)
		{
			printf("ASSERT: source is not indexed or not 16bits!\n");
			assert(false);
		}

		if (mesh->mAmplifiedVertexData[0] == 0 || mesh->mAmplifiedVertexData[0] == 0)
		{
			printf("ASSERT: source is not skinned!\n");
			assert(false);
		}


		mutalisk::data::mesh& dataHack = const_cast<mutalisk::data::mesh&>(data);
		delete dataHack.indexData; dataHack.indexData = 0;				// $HACK ; will force raw vertex data when rendering
		dataHack.primitiveType = GU_SPRITES;							// $HACK ; primtype is now sprites
		dataHack.vertexCount &= ~0x1;									// $HACK ; make sure we have even number of vertices

		Vertex* vertexData0 = (Vertex*)mesh->mAmplifiedVertexData[0];
		Vertex* vertexData1 = (Vertex*)mesh->mAmplifiedVertexData[1];
		size_t primCount = mesh->mBlueprint.vertexCount / 2;
		for (size_t j = 0; j < primCount; ++j)
		{
			size_t x = rand() & 0x3;
			size_t y = rand() & 0x3;
			float u = 0.25f * (float)x;
			float v = 0.25f * (float)y;
			vertexData0->u = u;			vertexData0->v = v;			++vertexData0;
			vertexData0->u = u + 0.25f;	vertexData0->v = v + 0.25f;	++vertexData0;
			vertexData1->u = u;			vertexData1->v = v;			++vertexData1;
			vertexData1->u = u + 0.25f;	vertexData1->v = v + 0.25f;	++vertexData1;
		}

		;;printf("done processing \"char render\"(tm) technique\n");
	}
}
	void prepareChars(mutalisk::RenderableScene& scene)
	{
		for(unsigned q = 0; q < scene.mResources.meshes.size(); ++q)
			prepareChars(scene.mResources.meshes[q].renderable.get());
	}


	void renderChars(mutalisk::RenderableScene const& scene)
	{
		const mutalisk::array<mutalisk::data::scene::Actor>& actors = scene.mBlueprint.actors;
		mutalisk::array<mutalisk::RenderableScene::SharedResources::Mesh>& meshes = const_cast<mutalisk::RenderableScene&>(scene).mResources.meshes;
		for(size_t q = 0; q < actors.size(); ++q)
		{
			unsigned meshIndex = actors[q].meshIndex;
			ASSERT(meshIndex >= 0 && meshIndex < meshes.size());
			mutalisk::RenderableMesh& mesh = *meshes[meshIndex].renderable;
			if (mesh.mAmplifiedVertexData[mesh.mAmplifiedBufferIndex] == 0)
				continue;
			const CTransform::t_matrix& worldMatrix = scene.mState.matrices[scene.mState.actor2XformIndex[q]];
			Mat33 wc,cw;
			Mat33_transpose(&wc, const_cast<Mat33*>(&worldMatrix.Rot));
			Mat33_mul(&cw, &wc, const_cast<Mat33*>(&scene.mState.cameraMatrix.Rot));
			Vec3 unit2d = {1.f, -1.f, 0.f};
			Vec3 vec;
			Vec3_setMat33MulVec3(&vec, &cw, &unit2d);

			struct Vertex
			{
				float u, v;
				Vec3 normal;
				Vec3 pos;
			};

			Vertex* vertexData = (Vertex*)mesh.mAmplifiedVertexData[mesh.mAmplifiedBufferIndex];
			size_t primCount = mesh.mBlueprint.vertexCount / 2;
			Vec3 p0, p1, v;
			Vec3_scale(&v, &vec, 0.03f);
			for (size_t j = 0; j < primCount; ++j)
			{
				Vec3 pos = vertexData->pos;
				Vec3_sub(&p0, &pos, &v);
				Vec3_add(&p1, &pos, &v);
				vertexData->pos = p0; ++vertexData;
				vertexData->pos = p1; ++vertexData;
			}
		}
	}
}
