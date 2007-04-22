#include "SpriteRenderer.h"
/////////////////////////////////////////////////////////////////////////////
namespace mutalisk
{
	struct Data
	{
		Vec3 pos;
		float dx, dy;
	};
	struct Sprite
	{
		Vec3 pos;
		float dx, dy;
		bool operator < (const Sprite& other) const 
		{
			if (pos.z > other.pos.z)
				return false;
			else
				return true;
		}
	};

namespace
{
	void prepareSprite(mutalisk::RenderableMesh* mesh)
	{
		ASSERT(mesh);
		mutalisk::data::mesh const& data = mesh->mBlueprint;

		;;printf("using \"sprite render\"(tm) technique\n");

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
			printf("ASSERT: vertex does not match sprite render vertex!\n");
			assert(false);
		}

		if(data.indexData == 0 /*|| (data.vertexDecl & GU_INDEX_16BIT) == 0*/)
		{
			printf("ASSERT: source is not indexed or not 16bits!\n");
			assert(false);
		}

		Vertex* src = (Vertex*)data.vertexData;
		u16* index = (u16*)data.indexData;

		std::vector<Sprite> sprites;
		sprites.reserve(data.indexCount / 6);

		for(size_t i = 0; i < data.indexCount; i+=6)
		{
			u32 i0 = *index++;
			u32 i1 = *index++;
			u32 i2 = *index++;
			u32 i3 = *index++;
			u32 i4 = *index++;
			u32 i5 = *index++;

			Vertex v0 = src[i0];
			Vertex v1 = src[i1];
			Vertex v2 = src[i2];

			Vec3& p0 = v0.pos;
			Vec3& p1 = v1.pos;
			Vec3& p2 = v2.pos;

			Vec3 p, dx, dy;

			Vec3_sub(&p, &p2, &p0);
			Vec3_scale(&p, &p, 0.5f);
			Vec3_add(&p, &p0, &p);

			Vec3_sub(&dx, &p2, &p1);
			Vec3_scale(&dx, &dx, 0.5f);

			Vec3_sub(&dy, &p1, &p0);
			Vec3_scale(&dy, &dy, 0.5f);

			Sprite sprite;
			sprite.pos = p;
			sprite.dx = Vec3_length(&dx);
			sprite.dy = Vec3_length(&dy);

			sprites.push_back(sprite);
		}

		size_t vertexCount = sprites.size() * 2 /*one sprite equals 2 verts*/;
		size_t vertexStride = sizeof(Vertex);
		mesh->mAmplifiedVertexData[0] = new unsigned char[vertexStride * vertexCount];
		mesh->mAmplifiedVertexDecl = GU_VERTEX_32BITF | GU_NORMAL_32BITF | GU_TEXTURE_32BITF;
		mesh->mAmplifiedVertexStride = vertexStride;
		mesh->mUserData = new unsigned char[sizeof(Data) * vertexCount];

		Vertex* dst = (Vertex*)mesh->mAmplifiedVertexData[0];
		Data* out   = (Data*)mesh->mUserData;
		Vec3 zeroVec = {0,0,0};
		for(std::vector<Sprite>::const_iterator it = sprites.begin(); it != sprites.end(); ++it)
		{
			const Sprite& sprite = *it;

			Vertex v0, v1;
			v0.u = v0.v = 0.f;
			v1.u = v1.v = 1.f;
			v0.pos = v1.pos = zeroVec;
			Data data;
			data.pos = sprite.pos;
			data.dx = sprite.dx;
			data.dy = sprite.dy;
			*dst++ = v0;
			*dst++ = v1;
			*out++ = data;
		}

		mutalisk::data::mesh& dataHack = const_cast<mutalisk::data::mesh&>(data);
		delete dataHack.indexData; dataHack.indexData = 0;				// $HACK ; will force raw vertex data when rendering
		dataHack.primitiveType = GU_SPRITES;							// $HACK ; primtype is now sprites
		dataHack.vertexCount = ((data.indexCount/6)) * 2;				// $HACK ; adjust vertex count

		mesh->mAmplifiedVertexData[1] = new unsigned char[vertexStride * vertexCount];
		memcpy(mesh->mAmplifiedVertexData[1], mesh->mAmplifiedVertexData[0], vertexStride * vertexCount);
		mesh->mAmplifiedBufferIndex = 0;

		;;printf("done processing \"ball render\"(tm) technique\n");
	}
}
	void prepareSprites(mutalisk::RenderableScene& scene)
	{
		for(unsigned q = 0; q < scene.mResources.meshes.size(); ++q)
		{
			if (!scene.mResources.meshes[q].blueprint->sprite)
				continue;
			prepareSprite(scene.mResources.meshes[q].renderable.get());
		}
	}


	void renderSprites(mutalisk::RenderableScene const& scene)
	{
		static bool firstTime = true;
		const mutalisk::array<mutalisk::data::scene::Actor>& actors = scene.mBlueprint.actors;
		mutalisk::array<mutalisk::RenderableScene::SharedResources::Mesh>& meshes = const_cast<mutalisk::RenderableScene&>(scene).mResources.meshes;
		for(size_t q = 0; q < actors.size(); ++q)
		{
			unsigned meshIndex = actors[q].meshIndex;
			ASSERT(meshIndex >= 0 && meshIndex < meshes.size());
			mutalisk::RenderableMesh& mesh = *meshes[meshIndex].renderable;
			if (mesh.mUserData == 0)
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
again:
			mesh.mAmplifiedBufferIndex = 1 - mesh.mAmplifiedBufferIndex;
			Vertex* vertexData = (Vertex*)mesh.mAmplifiedVertexData[mesh.mAmplifiedBufferIndex];
			Data* data = (Data*)mesh.mUserData;
			size_t primCount = mesh.mBlueprint.vertexCount / 2;
			Vec3 p0, p1, v;

			for (size_t j = 0; j < primCount; ++j)
			{
				Data& d = data[j];
				Vec3_scale(&v, &vec, d.dx);
				Vec3_sub(&p0, &d.pos, &v);
				Vec3_add(&p1, &d.pos, &v);
				vertexData[j*2+0].pos = p0;
				vertexData[j*2+1].pos = p1;
			}
			if (firstTime)
			{
				firstTime = false;
				goto again;
			}
		}
	}
}
