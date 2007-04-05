#include "TestDemo.h"


/////////////////////////////////////////////////////////////////////////////
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

	// @TBD: move code into scene/demo specific file
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

			static float t = 0;
			const static float t1 = 1.f / 520.f;
			static bool fadeOut = false;
			if (t > 5.f)
				fadeOut = true;
			if (t < -1.f)
				fadeOut = false;
			if (fadeOut)
				t -= 1.f / 60.f;
			else
				t += 1.f / 60.f;
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
	// \@TBD

/////////////////////////////////////////////////////////////////////////////

#define S_FUNC(f) (&SelfT::f)

void TestDemo::doFrame(float t)
{
	setTime(t);
	timeline.update(*this, frame());
	static char count= 0;
	if (count++ == 10)
	{
		count = 0;
		updateTextures();
	}
}

/*void TestDemo::onStart()
{
	{Item items[] = {
		Item(0,		0,	S_FUNC(logo)),
		Item(9,		20,	S_FUNC(logo_to_flower)),
		Item(12,	15,	S_FUNC(flower)),
		Item(26,	18,	S_FUNC(flower_to_phone0)),
		Item(32,	0,	S_FUNC(phone1)),
		Item(34,	12,	S_FUNC(phone2)),
		Item(36,	24,	S_FUNC(walk)),
		Item()
	};
	timeline.addScript(items);}

	{Item streamer[] = {
		Item(2,		0,	S_FUNC(loadFlower), 	Item::Once),
		Item(12,	0,	S_FUNC(loadPhone1), 	Item::Once),
		Item(24,	0,	S_FUNC(loadPhone2), 	Item::Once),
		Item(32,	0,	S_FUNC(loadWalk),		Item::Once),
		Item(36,	0,	S_FUNC(loadXXX),		Item::Once),
		Item()
	};
	timeline.addScript(streamer);
	}

	mutalisk::gDelayedTextureLoading = true;
	load(scn.logo,		"head\\psp\\head.msk");
	load(scn.flower,	"flower\\psp\\flower.msk");
	load(scn.phone1,	"telephone_s2\\psp\\telephone_s2.msk");
	load(scn.phone2,	"telephone_s2\\psp\\telephone_s2.msk");
	load(scn.walk,		"walk01\\psp\\walk01.msk");
	loadTextures(scn.logo, false);
}*/

namespace
{
	int ms(int v)
	{
		return static_cast<int>(round(static_cast<float>(v) * 0.3f));
	}
}

void TestDemo::onStart()
{
	{Item items[] = {
		Item(0,		ms(00),			S_FUNC(walk)),

		Item(23,	ms(05),		S_FUNC(logo)),
		Item(32,	ms(75)/*ms(05)*/,		S_FUNC(logo_x_flower)),
		Item(34,	ms(71),		S_FUNC(face_on_flower)),
		Item(44,	ms(80),		S_FUNC(flower)),

		Item(61,	ms(46),		S_FUNC(phone1)),
		Item(68,	ms(64),		S_FUNC(phone1_x__)),

		Item(69,	ms(14),		S_FUNC(phone__x_2)),
		Item(69,	ms(64),		S_FUNC(phone2)),			// 14 + (114 - 64)
		Item(76,	ms(32),		S_FUNC(phone2_x__)),

		Item(76,	ms(82),		S_FUNC(phone__x_3)),
		Item(77,	ms(32),		S_FUNC(phone3)),			// 82 + (82 - 32)
		Item(84,	ms(00),		S_FUNC(phone3_x__)),

		Item(84,	ms(50),		S_FUNC(phone__x_4)),
		Item(85,	ms(00),		S_FUNC(phone4)),				// (32 + 82)/2

		Item()
	};
	timeline.addScript(items);}

	mutalisk::gDelayedTextureLoading = false;
	load(scn.walk,		"walk\\psp\\walk.msk");
	load(scn.walkBG,	"walk\\psp\\back.msk");

	load(scn.logo,		"logo\\psp\\logo.msk");
	load(scn.flower,	"flower\\psp\\flower.msk");
	load(scn.face,		"head\\psp\\head.msk");
	
	load(scn.phone1,	"telephone_s1\\psp\\telephone_s1.msk");
	load(scn.phone2,	"telephone_s2\\psp\\telephone_s2.msk");
	load(scn.phone3,	"telephone_s3\\psp\\telephone_s3.msk");
	load(scn.phone4,	"telephone_s4\\psp\\telephone_s4.msk");
	load(scn.phoneTrans,"telephone_trans\\psp\\telephone_trans.msk");

	//load(scn.walk,		"walk01\\psp\\walk01.msk");
	//loadTextures(scn.logo, false);


	
	for(unsigned q = 0; q < scn.face.renderable->mResources.meshes.size(); ++q)
		prepareBalls(scn.face.renderable->mResources.meshes[q].renderable.get());
}

void TestDemo::walk()
{
	draw(scn.walkBG);
	clearZ();
	draw(scn.walk);
}

void TestDemo::logo()
{
	draw(scn.logo);
}

void TestDemo::logo_x_flower()
{
	draw(scn.flower);
	clearZ();
	draw(scn.logo);
}

void TestDemo::face_on_flower()
{
	draw(scn.flower);
	draw(scn.face, &::renderBalls);
}

void TestDemo::flower()
{
	draw(scn.flower);
}

void TestDemo::phone1()
{
	draw(scn.phone1);
}
void TestDemo::phone2()
{
	draw(scn.phone2);
	restart(scn.phoneTrans);
}
void TestDemo::phone3()
{
	draw(scn.phone3);
	restart(scn.phoneTrans);
}
void TestDemo::phone4()
{
	draw(scn.phone4);
	restart(scn.phoneTrans);
}

void TestDemo::phone1_x__()
{
	draw(scn.phone1);
	clearZ();
	draw(scn.phoneTrans);
}
void TestDemo::phone2_x__()
{
	draw(scn.phone2);
	clearZ();
	draw(scn.phoneTrans);
}
void TestDemo::phone3_x__()
{
	draw(scn.phone3);
	clearZ();
	draw(scn.phoneTrans);
}

void TestDemo::phone__x_2()
{
	draw(scn.phone2);
	clearZ();
	draw(scn.phoneTrans);
}
void TestDemo::phone__x_3()
{
	draw(scn.phone3);
	clearZ();
	draw(scn.phoneTrans);
}
void TestDemo::phone__x_4()
{
	draw(scn.phone4);
	clearZ();
	draw(scn.phoneTrans);
}
















void TestDemo::loadFlower()
{
	printf("%s\n", __FUNCTION__);
	loadTextures(scn.flower);
}

void TestDemo::loadPhone1()
{
	printf("%s\n", __FUNCTION__);
	loadTextures(scn.phone1);
}

void TestDemo::loadPhone2()
{
	printf("%s\n", __FUNCTION__);
	loadTextures(scn.phone2);
	unloadTextures(scn.logo);
}

void TestDemo::loadWalk()
{
	printf("%s\n", __FUNCTION__);
	loadTextures(scn.walk);
	unloadTextures(scn.flower);
}

void TestDemo::loadXXX()
{
	printf("%s\n", __FUNCTION__);
	unloadTextures(scn.phone1);
	unloadTextures(scn.phone2);
}
