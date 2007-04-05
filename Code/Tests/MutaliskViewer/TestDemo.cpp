#include "TestDemo.h"
#include "BallRenderer.h"

/////////////////////////////////////////////////////////////////////////////

#define S_FUNC(f) (&SelfT::f)

void TestDemo::doFrame(float t)
{
	setTime(t + timeOffset);
	timeline.update(*this, frame());
	static char count= 0;
	if (count++ == 10)
	{
		count = 0;
		updateTextures();
	}
}
namespace
{
	int ms(int v)
	{
		return static_cast<int>(round(static_cast<float>(v) * 0.3f));
	}
}
#if 0
void TestDemo::onStart()
{
	{Item items[] = {
		Item(0,		ms(00),		S_FUNC(walk)),

		Item(23,	ms(05),		S_FUNC(logo)),
		Item(32,	ms(75)/*ms(05)*/,
								S_FUNC(logo_x_flower)),
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
		Item(85,	ms(00),		S_FUNC(phone4)),			// (32 + 82)/2

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

	prepareBalls(*scn.face.renderable)
}
#endif

#if 1
void TestDemo::onStart()
{
	timeOffset = 120;
	{Item items[] = {
		Item(122,	ms(90),		S_FUNC(beer1)),
		Item(130,	ms(71),		S_FUNC(beer2)),
		Item(138,	ms(51),		S_FUNC(garlic1)),
		Item(146,	ms(32),		S_FUNC(garlic2)),
		Item(154,	ms(13),		S_FUNC(mix1)),
		Item(164,	ms(13),		S_FUNC(mix2)),
		Item(169,	ms(13),		S_FUNC(mix3)),

		Item()
	};
	timeline.addScript(items);}

	mutalisk::gDelayedTextureLoading = false;
	load(scn.beer1,		"beer\\beer1\\psp\\beer1.msk");
	load(scn.beer2,		"beer\\beer2\\psp\\beer2.msk");
	
	load(scn.garlic1,	"garlic\\garlic1\\psp\\garlic1.msk");
	load(scn.garlic2,	"garlic\\garlic2\\psp\\garlic2.msk");

	load(scn.mix1,		"mix\\mix1\\psp\\mix1.msk");
	load(scn.mix2,		"mix\\mix2\\psp\\mix2.msk");
	load(scn.mix3,		"mix\\mix3\\psp\\mix3.msk");
}
#endif

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


void TestDemo::beer1()
{
	draw(scn.beer1);
}
void TestDemo::beer2()
{
	draw(scn.beer2);
}
void TestDemo::garlic1()
{
	draw(scn.garlic1);
}
void TestDemo::garlic2()
{
	draw(scn.garlic2);
}
void TestDemo::mix1()
{
	draw(scn.mix1);
}
void TestDemo::mix2()
{
	draw(scn.mix2);
}
void TestDemo::mix3()
{
	draw(scn.mix3);
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
