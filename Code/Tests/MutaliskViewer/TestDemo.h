#ifndef TEST_DEMO_H_
#define TEST_DEMO_H_

#include "cfg.h"

#include <player/Timeline.h>
#include <player/DemoPlayer.h>


class TestDemo : public mutalisk::BaseDemoPlayer
{
	typedef TestDemo					SelfT;
	typedef mutalisk::Timeline<SelfT>	TimelineT;
	typedef TimelineT::Item				Item;
	struct Scenes
	{
		Scene	walk;
		Scene	walkBG;

		Scene	logo;
		Scene	flower;
		Scene	face;

		Scene	phone1;
		Scene	phone2;
		Scene	phone3;
		Scene	phone4;
		Scene	phoneTrans;

		//
		Scene	beer1;
		Scene	beer2;
		Scene	garlic1;
		Scene	garlic2;
		Scene	mix1;
		Scene	mix2;
		Scene	mix3;

	};
	Scenes							scn;
	TimelineT						timeline;
	float							timeOffset;

public:
	TestDemo() : timeOffset(0) {}
	void doFrame(float t);

protected:
	virtual void onStart();

	void walk();
	void logo();
	void logo_x_flower();
	void face_on_flower();
	void flower();
	void phone1(); void phone2(); void phone3(); void phone4();
	void phone1_x__(); void phone2_x__(); void phone3_x__();
	void phone__x_2(); void phone__x_3(); void phone__x_4();

	void beer1();
	void beer2();
	void garlic1();
	void garlic2();
	void mix1();
	void mix2();
	void mix3();

	// load points
	void loadFlower();
	void loadPhone1();
	void loadPhone2();
	void loadWalk();
	void loadXXX();
};

#endif // TEST_DEMO_H_
