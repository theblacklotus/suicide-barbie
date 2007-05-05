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
	std::vector<unsigned> random;

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
		if( random.size() < primCount)
		{
			srand(0);
			random.resize(primCount);
			for (size_t i = 0 ; i < random.size(); ++i)
			{
				random[i] = i;
			}

			for (size_t i = 0 ; i < random.size(); ++i)
			{
				size_t j = rand() % random.size();
				unsigned x = random[i];
				random[i] = random[j];
				random[j] = x;
			}
		}
		srand(0);
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
		struct Vertex
		{
			float u, v;
			Vec3 normal;
			Vec3 pos;
		};


		float time = scene.mState.time;
		float t = sinf(time += 0.1f);
		if (t < 0)
			t = 0;
		float loop = 5.f;
		t = fmodf(time*.6f * 2.f, loop);
		if (t > loop-0.5f)
			t = 0.f;
		if (t > loop-1.f)
			t = loop-1.f;


		/*
			t E [0, t0]			wipe in

			t E [t1, t2]		wipe out
			t E [t2, t3]		wipe in

			t E [t3, oo]		disable
		*/

		const float t0 = 1.f;
		const float t1 = 10.46f;
		const float t2 = 11.46f;
		const float t3 = 12.46f;

		uint state = 0;
/*
		static bool firstTime = true;
		if (firstTime == true)
		{
			firstTime = false;
			time = 0.f;
		}
*/
		if (0.f <= time && time < t0)
		{
			t = 2.f * time / t0;
			state = 1;
		}
		else if (t0 <= time && time < t1)
		{
			t = 2.f * 1.f;
			state = 2;
		}
		else if (t1 <= time && time < t2)
		{
			t = 1.f + (1.f - (time - t1) / (t2-t1));
			state = 3;
		}
		else if (t2 <= time && time < t3)
		{
			t = 1.f + (time - t2) / (t3-t2);
			state = 4;
		}

		size_t globPrimCount = 0xffffffff;

		// pre-pass - transform between subspace clusters
		const mutalisk::array<mutalisk::data::scene::Actor>& actors = scene.mBlueprint.actors;
		mutalisk::array<mutalisk::RenderableScene::SharedResources::Mesh>& meshes = const_cast<mutalisk::RenderableScene&>(scene).mResources.meshes;

	if (!false)
	{
		for(size_t q = 0; q < actors.size(); ++q)
		{
			unsigned meshIndex = actors[q].meshIndex;
			ASSERT(meshIndex >= 0 && meshIndex < meshes.size());
			mutalisk::RenderableMesh& mesh = *meshes[meshIndex].renderable;
			if (mesh.mAmplifiedVertexData[mesh.mAmplifiedBufferIndex] == 0)
				continue;
			size_t xform0 = scene.mState.actor2XformIndex[q];
			size_t xform1;

			int meshIndex2 = -1;
			for(size_t w = q+1; w < actors.size(); ++w)
			{
				unsigned meshIndex = actors[w].meshIndex;
				ASSERT(meshIndex >= 0 && meshIndex < meshes.size());
				mutalisk::RenderableMesh& mesh = *meshes[meshIndex].renderable;
				if (mesh.mAmplifiedVertexData[mesh.mAmplifiedBufferIndex] == 0)
					continue;
				meshIndex2 = meshIndex;
				xform1 = scene.mState.actor2XformIndex[w];
				break;
			}
			if (meshIndex2 == -1)
			{
				for(size_t w = 0; w < actors.size(); ++w)
				{
					unsigned meshIndex = actors[w].meshIndex;
					ASSERT(meshIndex >= 0 && meshIndex < meshes.size());
					mutalisk::RenderableMesh& mesh = *meshes[meshIndex].renderable;
					if (mesh.mAmplifiedVertexData[mesh.mAmplifiedBufferIndex] == 0)
						continue;
					meshIndex2 = meshIndex;
					xform1 = scene.mState.actor2XformIndex[w];
					break;
				}
			}
			if (meshIndex == meshIndex2)
				break;
			ASSERT(meshIndex2 >= 0 && meshIndex2 < meshes.size());
			mutalisk::RenderableMesh& mesh2 = *meshes[meshIndex2].renderable;

//			printf("%i > %i = %i > %i\n", meshIndex, meshIndex2, mesh.mBlueprint.vertexCount, mesh2.mBlueprint.vertexCount);


			Vertex* vertexData0 = (Vertex*)mesh.mAmplifiedVertexData[mesh.mAmplifiedBufferIndex];
			Vertex* vertexData1 = (Vertex*)mesh2.mAmplifiedVertexData[mesh2.mAmplifiedBufferIndex];
			size_t primCount = std::min(mesh.mBlueprint.vertexCount, mesh2.mBlueprint.vertexCount) / 2;
			globPrimCount = std::min(globPrimCount, primCount);
			Vec3 pivot0 = vertexData0->pos;
			Vec3 pivot1 = vertexData1->pos;

			for (size_t j = 0; j < primCount; ++j)
			{
				Vec3 pos0 = vertexData0->pos;
				Vec3 pos1 = vertexData1->pos;

				if (state == 1)
				{
					pos1 = pos0;
					pos0.z -= 1.f;
				}
				else if (state == 3)
				{
					pos1 = pos0;
					pos0.z += 2.f;
				}
				if (state == 4)
				{
					pos1 = pos0;
					pos0.z -= 2.f;
				}

				Vec3 pos3, pivot3;
				float displace = random[j];
				float td = t - 0.5f * (displace/(float)primCount);
				td = std::max(td, 0.f);
				td = std::min(td, 1.f);
				td = sinf(((td-0.5f) * 3.1415926535f)) * 0.5f + 0.5f;
				Vec3_sub(&pos3, &pos1, &pos0);
				Vec3_scale(&pos3, &pos3, td);
				Vec3_add(&pos3, &pos3, &pos0);

				if (state == 0)
				{

				Vec3_sub(&pivot3, &pivot1, &pivot0);
				Vec3_scale(&pivot3, &pivot3, td);
				Vec3_add(&pivot3, &pivot3, &pivot0);

				td = sinf(((td) * 3.1415926535f)) * 0.75f;
				Vec3_sub(&pivot3, &pivot3, &pos3);
				Vec3_scale(&pivot3, &pivot3, td);
				Vec3_add(&pos3, &pivot3, &pos3);
				}

				++vertexData0;
				vertexData0->pos = pos3; ++vertexData0;
				++vertexData1;
				++vertexData1;
			}

		}

	}
//		printf("globPrimCount = %i\n", globPrimCount);

		static bool s_firstTime = true;
		for(size_t q = 0; q < actors.size(); ++q)
		{
			bool firstTime = s_firstTime;
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

again:
			Vertex* vertexData = (Vertex*)mesh.mAmplifiedVertexData[mesh.mAmplifiedBufferIndex];
			size_t primCount = mesh.mBlueprint.vertexCount / 2;
			Vec3 p0, p1, v;
			Vec3_scale(&v, &vec, 0.03f);
			for (size_t j = 0; j < primCount; ++j)
			{
				Vec3 pos = (vertexData+1)->pos;
				Vec3_sub(&p0, &pos, &v);
				Vec3_add(&p1, &pos, &v);
				if (j >= globPrimCount /*|| (j & 0x1)*/)
				{
					Vec3_scale(&p0, &p0, 0);
					Vec3_scale(&p1, &p1, 0);
				}
				vertexData->pos = p0; ++vertexData;
				vertexData->pos = p1; ++vertexData;
			}
			if (firstTime)
			{
				mesh.mAmplifiedBufferIndex = 1 - mesh.mAmplifiedBufferIndex;
				firstTime = false;
				goto again;
			}
		}
		s_firstTime = false;
	}
}
