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

	struct FlashScreenJob : public BaseDemoPlayer::IJob
	{
		mutalisk::RenderContextT*	renderContext;
		float						intensity;
		unsigned					color;
		void process()
		{
		#if defined(MUTALISK_DX9)
		// -- disabled
		#elif defined(MUTALISK_PSP)
			unsigned c = (unsigned)(intensity * 0xff);
			sceGuEnable(GU_BLEND);
			unsigned int srcFix = GU_ARGB(0, c, c, c);
			unsigned int dstFix = 0xffffff;
			if (intensity == 1.f && (color & 0xffffff) != 0xffffff)
				dstFix = 0x000000;
			sceGuBlendFunc(GU_ADD, GU_FIX, GU_FIX, srcFix, dstFix);

			sceGuTexFunc(GU_TFX_MODULATE,GU_TCC_RGB);
			sceGuDisable(GU_TEXTURE_2D);
			sceGuColor(color);
			sceGuDisable(GU_LIGHTING);
			sceGuDisable(GU_DEPTH_TEST);
			sceGuDepthMask(1);
	
			struct QuadVertexTex
			{
				short x,y,z;
			};
			QuadVertexTex* vertices = reinterpret_cast<QuadVertexTex*>(sceGuGetMemory(2 * sizeof(QuadVertexTex)));
			vertices[0].x = 0; vertices[0].y = 0; vertices[0].z = 0;
			vertices[1].x = 480; vertices[1].y = 272; vertices[1].z = 0;

			sceGuDrawArray(GU_SPRITES,GU_VERTEX_16BIT|GU_TRANSFORM_2D,2,0,vertices);
			sceGuDepthMask(0);
			sceGuDisable(GU_BLEND);
			sceGuEnable(GU_DEPTH_TEST);
		#endif
		}
	};

	void walk(); void walk_far();
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
