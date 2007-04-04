#include "TestDemo.h"

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

void TestDemo::onStart()
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
}

void TestDemo::logo()
{
	draw(scn.logo);
}

void TestDemo::logo_to_flower()
{
	draw(scn.logo);
	clearZ();
	draw(scn.flower);
}

void TestDemo::flower()
{
	draw(scn.flower);
}

void TestDemo::flower_to_phone0()
{
	draw(scn.phone1);
	clearZ();
	draw(scn.flower);
}

void TestDemo::phone1()
{
	draw(scn.phone1);
}

void TestDemo::phone2()
{
	draw(scn.phone2);
}

void TestDemo::walk()
{
	draw(scn.walk);
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
