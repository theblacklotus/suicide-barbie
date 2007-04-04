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
		Scene	logo;
		Scene	flower;
		Scene	phone1;
		Scene	phone2;
		Scene	walk;
	};
	Scenes							scn;
	TimelineT						timeline;
	Scene moo;

public:
	void doFrame(float t);

protected:
	virtual void onStart();

	void logo();
	void logo_to_flower();
	void flower();
	void flower_to_phone0();
	void phone1();
	void phone2();
	void walk();

	// load points
	void loadFlower();
	void loadPhone1();
	void loadPhone2();
	void loadWalk();
	void loadXXX();
};

#endif // TEST_DEMO_H_
