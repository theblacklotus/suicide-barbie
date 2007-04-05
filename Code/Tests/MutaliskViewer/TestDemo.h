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
	};
	Scenes							scn;
	TimelineT						timeline;
	Scene moo;

public:
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

	// load points
	void loadFlower();
	void loadPhone1();
	void loadPhone2();
	void loadWalk();
	void loadXXX();
};

#endif // TEST_DEMO_H_
