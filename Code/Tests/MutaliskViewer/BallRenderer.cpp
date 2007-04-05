#include "BallRenderer.h"
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
	void prepareBalls(mutalisk::RenderableMesh* mesh)
	{
		ASSERT(mesh);
		mutalisk::data::mesh const& data = mesh->mBlueprint;

		;;printf("using \"ball render\"(tm) technique\n");

		if ( (data.vertexDecl & GU_COLOR_8888) == 0 ) 
		{
			printf("ASSERT: need vertex color for ball render!\n");
			assert(false);
		}

		// this is what the input must look like, and what the output will look like.. simple, huh?
		struct Vertex
		{
			float u, v;
			u32 color;
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

		Vertex* src = (Vertex*)data.vertexData;
		u16* index = (u16*)data.indexData;

		u32 skipped = 0;
		float scale = 1000.f;
		float areaThreshold = 0.f;

		std::vector<Ball> balls;
		balls.reserve(data.indexCount / 3);

		for(size_t i = 0; i < data.indexCount; i+=3)
		{
			u32 i0 = *index++;
			u32 i1 = *index++;
			u32 i2 = *index++;

			Vertex v0 = src[i0];
			Vertex v1 = src[i1];
			Vertex v2 = src[i2];

			Vec3& p0 = v0.pos;
			Vec3& p1 = v1.pos;
			Vec3& p2 = v2.pos;

			Vec3_scale(&p0, &p0, scale);
			Vec3_scale(&p1, &p1, scale);
			Vec3_scale(&p2, &p2, scale);

			// calc area

			Vec3 d0, d1, c;
			Vec3_sub(&d0, &p1, &p0);
			Vec3_sub(&d1, &p2, &p0);
			Vec3_cross(&c, &d0, &d1);
			float area = Vec3_length(&c) * 0.5f;

			// calc midpoint (barycentric)

			Vec3 p;
			Vec3_scale(&d0, &d0, 0.5f);
			Vec3_scale(&d1, &d1, 0.5f);
			Vec3_add(&p, &d0, &d1);
			Vec3_add(&p, &p0, &p);

			Vec3_scale(&p, &p, 1.f / scale);

			if (area < areaThreshold)
			{
				skipped++;
				continue;
			}

			Ball ball;
			ball.color = v0.color;
			ball.normal = v0.normal;
			ball.pos = p;
			ball.area = sqrtf(area) / 1300.f;
			ball.area += ball.area * 0.25f;

			balls.push_back(ball);
		}
		printf("%i out of %i triangles skipped\n", skipped, data.indexCount/3);

		std::sort(balls.begin(), balls.end());

		size_t vertexCount = balls.size() * 2 /*one sprite equals 2 verts*/;
		size_t vertexStride = sizeof(Vertex);
		mesh->mAmplifiedVertexData[0] = new unsigned char[vertexStride * vertexCount];
		mesh->mAmplifiedVertexDecl = GU_VERTEX_32BITF | GU_NORMAL_32BITF | GU_TEXTURE_32BITF | GU_COLOR_8888;
		mesh->mAmplifiedVertexStride = vertexStride;
		mesh->mUserData = new unsigned char[sizeof(Data) * vertexCount];

		std::vector<unsigned> random;
		random.resize(balls.size());
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

		Vertex* dst = (Vertex*)mesh->mAmplifiedVertexData[0];
		Data* out   = (Data*)mesh->mUserData;
		Vec3 zeroVec = {0,0,0};
		std::vector<unsigned>::const_iterator rnd = random.begin();
		for(std::vector<Ball>::const_iterator it = balls.begin(); it != balls.end(); ++it)
		{
			const Ball& ball = *it;

			Vertex v0, v1;
			v0.u = v0.v = 0.f;
			v1.u = v1.v = 1.f;
			v0.color = v1.color = ball.color;
			v0.normal = v1.normal = ball.normal;
			v0.pos = v1.pos = zeroVec;
			Data data;
			data.pos = ball.pos;
			data.area = ball.area;
			data.displace = *rnd++;
			*dst++ = v0;
			*dst++ = v1;
			*out++ = data;
		}

		mutalisk::data::mesh& dataHack = const_cast<mutalisk::data::mesh&>(data);
		delete dataHack.indexData; dataHack.indexData = 0;				// $HACK ; will force raw vertex data when rendering
		dataHack.primitiveType = GU_SPRITES;							// $HACK ; primtype is now sprites
		dataHack.vertexCount = ((data.indexCount/3) - skipped) * 2;		// $HACK ; adjust vertex count

		mesh->mAmplifiedVertexData[1] = new unsigned char[vertexStride * vertexCount];
		memcpy(mesh->mAmplifiedVertexData[1], mesh->mAmplifiedVertexData[0], vertexStride * vertexCount);
		mesh->mAmplifiedBufferIndex = 0;

		;;printf("done processing \"ball render\"(tm) technique\n");
	}
}
	void prepareBalls(mutalisk::RenderableScene& scene)
	{
		for(unsigned q = 0; q < scene.mResources.meshes.size(); ++q)
			prepareBalls(scene.mResources.meshes[q].renderable.get());
	}


	void renderBalls(mutalisk::RenderableScene const& scene)
	{
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
			Vec3 unit2d = {1.f, 1.f, 0.f};
			Vec3 vec;
			Vec3_setMat33MulVec3(&vec, &cw, &unit2d);
			struct Vertex
			{
				float u, v;
				u32 color;
				Vec3 normal;
				Vec3 pos;
			};
			mesh.mAmplifiedBufferIndex = 1 - mesh.mAmplifiedBufferIndex;
			Vertex* vertexData = (Vertex*)mesh.mAmplifiedVertexData[mesh.mAmplifiedBufferIndex];
			Data* data = (Data*)mesh.mUserData;
			size_t primCount = mesh.mBlueprint.vertexCount / 2;
			Vec3 p0, p1, v;

			float t = scene.mState.time;
			float halfTime = scene.mState.clip->clip_length() * 0.5f;
			if (t > halfTime)
			{
				float dt = t - halfTime;
				t -= dt * 2;
			}

			const static float t1 = 1.f / 520.f;

			float t0 = t1;
			for (size_t j = 0; j < primCount; ++j)
			{
				unsigned displace = data[j].displace;
				Data& d = data[displace];
				Vec3_scale(&v, &vec, d.area);

				t0 += t1;
				float td = (t - t0) * 2.5f;
				if (td <= 0.f)
					v.x = v.y = v.z = 0.f;
				else if (0.f < td && td <= 1.f)
				{
					float const up = 1.2f;
					float a = 3.142592f - asinf(1.f/up);
					float s = up * sinf(td*a);
					Vec3_scale(&v, &v, s);
				}



				Vec3_sub(&p0, &d.pos, &v);
				Vec3_add(&p1, &d.pos, &v);
				vertexData[displace*2+0].pos = p0;
				vertexData[displace*2+1].pos = p1;
			}
		}
	}
}
