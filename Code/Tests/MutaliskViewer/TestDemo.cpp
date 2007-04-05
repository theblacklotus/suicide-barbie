#include "TestDemo.h"
#include "BallRenderer.h"

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
		Item(48,	ms(80),		S_FUNC(flower)),

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
	
	prepareBalls(*scn.face.renderable);
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
	draw(scn.face, &mutalisk::renderBalls);
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
