#include "dxstdafx.h"
#include "TestDemo.h"

#define S_FUNC(f) (&SelfT::f)

void TestDemo::doFrame(float t)
{
	setTime(t);
	timeline.update(*this, frame());
}

void TestDemo::onStart()
{
	{Item items[] = {
		Item(0,		0,	S_FUNC(logo)),
		Item(9,		20,	S_FUNC(logo_to_flower)),
		Item(12,	15,	S_FUNC(flower)),
		Item(26,	18,	S_FUNC(flower_to_phone0)),
		Item(32,	0,	S_FUNC(phone0)),
		Item(34,	12,	S_FUNC(walk)),
		Item()
	};
	timeline.addScript(items);}

	load(scn.logo,		"logo\\dx9\\logo.msk");
	load(scn.flower,	"flower\\dx9\\flower.msk");
	load(scn.phone0,	"telephone_s1\\dx9\\telephone_s1.msk");
	load(scn.walk,		"walk01\\dx9\\walk01.msk");
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
	draw(scn.phone0);
	clearZ();
	draw(scn.flower);
}

void TestDemo::phone0()
{
	draw(scn.phone0);
}

void TestDemo::walk()
{
	draw(scn.walk);
}
