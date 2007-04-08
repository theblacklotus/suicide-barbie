#include "TestDemo.h"
#include "BallRenderer.h"
#include "CharRenderer.h"
#include "BlinkyBlinky.h"

/////////////////////////////////////////////////////////////////////////////

#define S_FUNC(f) (&SelfT::f)

void TestDemo::doFrame(float t)
{
	setTime(t + timeOffset);
	timeline.update(*this, frame());
	static char count= 0;
	if (count++ == 3)
	{
		count = 0;
		updateTextures();
	}
	{
		unsigned color;
		float val = getBlinkyValue(time(), color);
		if (val != 0.f)
		{
			FlashScreenJob* job = new FlashScreenJob;
			job->renderContext = &renderContext;
			job->intensity = val;
			job->color = color;
			mJobQueue.push_back(job);
		}
	}
}

namespace
{
	int ms(int v)
	{
		return static_cast<int>(round(static_cast<float>(v) * 0.3f));
	}
}
void TestDemo::onStart()
{
	timeOffset = 0;
	{Item items[] = {
		Item(0,		ms(00),		S_FUNC(walk)),
		Item(11,	ms(00),		S_FUNC(walk_far)),

		Item(23,	ms(05),		S_FUNC(logo)),
		Item(32,	ms(75)/*ms(05)*/,
								S_FUNC(logo_x_flower)),
		Item(35,	ms(71),		S_FUNC(face_on_flower)),
		Item(48,	ms(80),		S_FUNC(flower)),

		Item(50,	ms(00),		S_FUNC(flower_x_spiral)),
		Item(55,	ms(00),		S_FUNC(spiral)),

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

		Item(92,	ms(18),		S_FUNC(text)),
		Item(115,	ms(22),		S_FUNC(jealousy)),

		Item(122,	ms(90),		S_FUNC(beer1)),
		Item(129.75,	ms(71),		S_FUNC(beer2)),
		Item(137.75,	ms(51),		S_FUNC(garlic1)),
		Item(145.60,	ms(32),		S_FUNC(garlic2)),
		Item(153.50,	ms(13),		S_FUNC(mix1)),
		Item(164.25,	ms(13),		S_FUNC(mix2)),
		Item(169.25,	ms(13),		S_FUNC(mix3)),

		Item(184,	ms(34),		S_FUNC(reload)),
		Item(186,	ms(50),		S_FUNC(gun)),
		Item(196,	ms(94),		S_FUNC(m16)),
		Item(207,	ms(38),		S_FUNC(bullet1)),
		Item(217,	ms(06),		S_FUNC(explodeGirl1)),
		Item(225,	ms(35),		S_FUNC(bullet2)),
 		Item(233,	ms(64),		S_FUNC(explodeGirl2)),
 		Item(241,	ms(94),		S_FUNC(windowBarbie1)),
 		Item(245,	ms(94),		S_FUNC(windowBarbie2)),
 		Item(250,	ms(20),		S_FUNC(endBarbie0)),
 		Item(252,	ms(20),		S_FUNC(endBarbie1)),
 		Item(276,	ms(64),		S_FUNC(endBarbie2)),

		Item()
	};
	timeline.addScript(items);}

	mutalisk::gDelayedTextureLoading = true;
	load(scn.walk,		"walk\\psp\\walk.msk");
	load(scn.walkBG,	"walk\\psp\\back.msk");
	load(scn.logo,		"logo\\psp\\logo.msk");

	load(scn.flower,	"flower\\psp\\flower.msk");
	load(scn.face,		"head\\psp\\head.msk");
	prepareBalls(*scn.face.renderable);

	load(scn.spiral,	"snake\\psp\\snake.msk");
	
	load(scn.phone1,	"telephone_s1\\psp\\telephone_s1.msk");
	load(scn.phone2,	"telephone_s2\\psp\\telephone_s2.msk");
	load(scn.phone3,	"telephone_s3\\psp\\telephone_s3.msk");
	load(scn.phone4,	"telephone_s4\\psp\\telephone_s4.msk");
	load(scn.phoneTrans,"telephone_trans\\psp\\telephone_trans.msk");

//	load(scn.textWalk,	"text\\psp\\text.msk");
//	prepareChars(*scn.textWalk.renderable);
	load(scn.textBG,	"text\\psp\\back.msk");
	load(scn.text,		"text\\psp\\undertext.msk");
	load(scn.jealousy,	"jealousy\\psp\\jealousy.msk");

	load(scn.beer1,		"beer\\beer1\\psp\\beer1.msk");
	load(scn.beer2,		"beer\\beer2\\psp\\beer2.msk");
	
	load(scn.garlic1,	"garlic\\garlic1\\psp\\garlic1.msk");
	load(scn.garlic2,	"garlic\\garlic2\\psp\\garlic2.msk");

	load(scn.mix1,		"mix\\mix1\\psp\\mix1.msk");
	load(scn.mix2,		"mix\\mix2\\psp\\mix2.msk");
	load(scn.mix3,		"mix\\mix3\\psp\\mix3.msk");

/*	load(scn.reload,	"reload\\psp\\reload.msk");
	load(scn.m16,		"weapon3\\psp\\weapon3.msk");
	load(scn.gun,		"weapon2\\psp\\gun.msk");
	load(scn.bullet1,	"bull1\\psp\\bull1.msk");
	load(scn.bullet2,	"bull2\\psp\\bull2.msk");
	load(scn.expGirl1BG,"back_01\\psp\\back_01.msk");
	load(scn.expGirl2BG,"back_02\\psp\\back_02.msk");
	load(scn.expGirl1,	"exgirl1\\psp\\exgirl1.msk");
	load(scn.expGirl2,	"exgirl2\\psp\\exgirl2.msk");
	load(scn.windowBarbie,	
						"suicidebarbie1\\psp\\suicidebarbie1.msk");
	load(scn.window,	"window\\psp\\window.msk");
*/

	if (mutalisk::gDelayedTextureLoading == false)
		return;

	{Item items[] = {
		Item(0,		ms(00),		S_FUNC(loadFlower),		Item::Once),
		Item(23,	ms(15),		S_FUNC(loadSpiral),		Item::Once),
		Item(40,	ms(71),		S_FUNC(loadTextScene),		Item::Once),
		Item(44,	ms(00),		S_FUNC(loadPhoneA),		Item::Once),
		Item(55.5,	ms(00),		S_FUNC(loadPhoneB),		Item::Once),

		Item(69,	ms(64),		S_FUNC(loadText),		Item::Once),
		Item(85,	ms(18),		S_FUNC(loadDreamPre),		Item::Once),

		Item(92,	ms(18),		S_FUNC(loadDreamA),		Item::Once),
		Item(115.5,	ms(22),		S_FUNC(loadWeaponScenes),		Item::Once),
		Item(123,	ms(90),		S_FUNC(loadDreamB),		Item::Once),

		Item(154.5,	ms(13),		S_FUNC(loadWeaponA),	Item::Once),
		Item(165,	ms(13),		S_FUNC(loadExploScenes),	Item::Once),
		Item(184,	ms(34),		S_FUNC(loadWeaponB),	Item::Once),

		Item(208,	ms(00),		S_FUNC(loadWindowScenes),	Item::Once),
		Item(226,	ms(00),		S_FUNC(loadWindow),	Item::Once),

 		Item(250,	ms(20),		S_FUNC(loadEndScenes),	Item::Once),
 		Item(251,	ms(20),		S_FUNC(loadEnd),	Item::Once),

		Item()
	};
	timeline.addScript(items);}

	loadTextures(scn.walk, false);
	loadTextures(scn.walkBG, false);
	loadTextures(scn.logo, false);
}

namespace {
float gVScale = 1.0f;
void updateAnimatedProperties(mutalisk::RenderableScene const& scene)
{
	float vScale = gVScale;

	const mutalisk::array<mutalisk::data::scene::Actor>& actors = scene.mBlueprint.actors;
	float time = scene.mState.time;

	// update properties
	for(size_t q = 0; q < actors.size(); ++q)
	{
		mutalisk::data::scene::Actor& actor = const_cast<mutalisk::data::scene::Actor&>(actors[q]);

		bool hasUVScroll = scene.mState.hasAnimation(actor.nodeName, "UVScroll");
		float v = scene.mState.sampleAnimation(actor.nodeName, "UVScroll", time);
		float fadeOut = scene.mState.sampleAnimation(actor.nodeName, "Fadeout", time, 0.0f);
		float fadeIn = scene.mState.sampleAnimation(actor.nodeName, "Fadein", time, 1.0f);
		for(size_t w = 0; w < actor.materials.size(); ++w)
		{
			if(hasUVScroll)
			{
				actor.materials[w].shaderInput.vOffset = 1.0f - v*(vScale + 1.0f);
				actor.materials[w].shaderInput.vScale = vScale;
			}
			actor.materials[w].shaderInput.transparency = 1.0f - ((1.0f - fadeOut) * fadeIn);
		}
		//actor.active = (scene.mState.sampleAnimation(actor.nodeName, "Fadein", time+1.0f, 1.0f) > 0.0f);
	}
}

void updateAnimatedProperties2(mutalisk::RenderableScene const& scene)
{
	float vScale = gVScale;

	const mutalisk::array<mutalisk::data::scene::Actor>& actors = scene.mBlueprint.actors;
	float time = scene.mState.time;

	// update properties
	for(size_t q = 0; q < actors.size(); ++q)
	{
		mutalisk::data::scene::Actor& actor = const_cast<mutalisk::data::scene::Actor&>(actors[q]);

		bool hasUVScroll = scene.mState.hasAnimation(actor.nodeName, "UVScroll");
		float v = scene.mState.sampleAnimation(actor.nodeName, "UVScroll", time);
		float fadeOut = scene.mState.sampleAnimation(actor.nodeName, "Fadeout", time, 0.0f);
		float fadeIn = scene.mState.sampleAnimation(actor.nodeName, "Fadein", time, 1.0f);
		for(size_t w = 0; w < actor.materials.size(); ++w)
		{
			if(hasUVScroll)
			{
				actor.materials[w].shaderInput.vOffset = 2.0f - v*2.0f;
				actor.materials[w].shaderInput.vScale = -vScale;
				//actor.materials[w].shaderInput.vOffset = 1.0f - v*(vScale + 1.0f);
				//actor.materials[w].shaderInput.vScale = vScale;
			}
			actor.materials[w].shaderInput.transparency = 1.0f - ((1.0f - fadeOut) * fadeIn);
		}
		//actor.active = (scene.mState.sampleAnimation(actor.nodeName, "Fadein", time+1.0f, 1.0f) > 0.0f);
	}
}
void updateAnimatedProperties3(mutalisk::RenderableScene const& scene)
{
	const mutalisk::array<mutalisk::data::scene::Actor>& actors = scene.mBlueprint.actors;
	float time = scene.mState.time;

	// update properties
	for(size_t q = 0; q < actors.size(); ++q)
	{
		float f = std::min(time * 0.2f, 1.0f);

		mutalisk::data::scene::Actor& actor = const_cast<mutalisk::data::scene::Actor&>(actors[q]);
		for(size_t w = 0; w < actor.materials.size(); ++w)
		{
			actor.materials[w].shaderInput.diffuse.r = f;
			actor.materials[w].shaderInput.diffuse.g = f;
			actor.materials[w].shaderInput.diffuse.b = f;
		}
	}
}


}

void TestDemo::walk()
{
	scn.walk.znear = 1;
	scn.walk.zfar = 15;

	draw(scn.walkBG);
	clearZ();
	draw(scn.walk);

	ppBloom(0);
}
void TestDemo::walk_far()
{
	scn.walk.znear = 3;
	scn.walk.zfar = 8;

	draw(scn.walkBG);
	clearZ();
	draw(scn.walk);

	ppBloom(0);
}

void TestDemo::logo()
{
	gVScale = 3.0f;
	draw(scn.logo, updateAnimatedProperties);
}

void TestDemo::logo_x_flower()
{
	gVScale = 3.0f;
	draw(scn.flower);
	clearZ();
	draw(scn.logo, updateAnimatedProperties);
}

void TestDemo::face_on_flower_w_logo()
{
	gVScale = 3.0f;
	draw(scn.flower);
	draw(scn.face, &mutalisk::renderBalls);
	clearZ();
	draw(scn.logo, updateAnimatedProperties);
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

void TestDemo::flower_x_spiral()
{
	draw(scn.spiral, updateAnimatedProperties);
	clearZ();
	draw(scn.flower);
}

void TestDemo::spiral()
{
	draw(scn.spiral, updateAnimatedProperties);
}

void TestDemo::phone1()
{
	draw(scn.phone1);
	ppBloom(0.2f, 114, 200, 160);
}
void TestDemo::phone2()
{
	draw(scn.phone2);
	ppBloom(0.2f, 114, 200, 160);
	restart(scn.phoneTrans);
}
void TestDemo::phone3()
{
	draw(scn.phone3);
	ppBloom(0.2f, 114, 200, 160);
	restart(scn.phoneTrans);
}
void TestDemo::phone4()
{
	draw(scn.phone4);
	ppBloom(0.2f, 114, 200, 160);
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


void TestDemo::text()
{
	draw(scn.textBG);
	clearZ();
	draw(scn.text);
	clearZ();
	draw(scn.textWalk, &mutalisk::renderChars);

	ppBloom(0);
}
void TestDemo::jealousy()
{
	gVScale = 3.0f;
	draw(scn.jealousy, updateAnimatedProperties);
	ppBloom(0.2f, 114, 200, 160);
}

void TestDemo::beer1()
{
	draw(scn.beer1);
	ppBloom(0.9f, 1, 212, 114);
}
void TestDemo::beer2()
{
	draw(scn.beer2);
	ppBloom(0.9f, 1, 212, 114);
}
void TestDemo::garlic1()
{
	draw(scn.garlic1);
	ppBloom(0.9f, 1, 212, 114);
}
void TestDemo::garlic2()
{
	draw(scn.garlic2);
	ppBloom(0.9f, 1, 212, 114);
}
void TestDemo::mix1()
{
	draw(scn.mix1);
	ppBloom(0.9f, 1, 212, 114);
}
void TestDemo::mix2()
{
	draw(scn.mix2);
	ppBloom(0.9f, 1, 212, 114);
}
void TestDemo::mix3()
{
	draw(scn.mix3);
	ppBloom(0.9f, 1, 212, 114);
}

void TestDemo::reload()
{
	scn.reload.znear = 0.1;
	scn.reload.zfar = 8;

	draw(scn.reload);
	ppBloom(0);
}
void TestDemo::m16()
{
	gVScale = 1.0f;
	draw(scn.m16, updateAnimatedProperties2);
	ppBloom(0);
}
void TestDemo::gun()
{
	gVScale = 1.0f;
	scn.gun.zfar = 75;
	draw(scn.gun, updateAnimatedProperties2);
	ppBloom(0);
}
void TestDemo::bullet1()
{
	gVScale = 3.0f;
	draw(scn.bullet1, updateAnimatedProperties);
	ppBloom(0);
}
void TestDemo::bullet2()
{
	gVScale = 3.0f;
	scn.bullet2.zfar = 100;
	draw(scn.bullet2, updateAnimatedProperties);
	ppBloom(0);
}
void TestDemo::explodeGirl1()
{
	draw(scn.expGirl1BG);
	clearZ();
	draw(scn.expGirl1);
	ppBloom(0);
}
void TestDemo::explodeGirl2()
{
	draw(scn.expGirl2BG);
	clearZ();
	draw(scn.expGirl2);
	ppBloom(0);
}
void TestDemo::windowBarbie1()
{
	draw(scn.windowBarbie, updateAnimatedProperties3);
	ppBloom(0.2f, 114, 200, 160);
}
void TestDemo::windowBarbie2()
{
	draw(scn.windowBarbie);
	clearZ();
	draw(scn.window, 2.5f);
	ppBloom(0.2f, 114, 200, 160);
}

void TestDemo::endBarbie0()
{
}

void TestDemo::endBarbie1()
{
	draw(scn.endBack);
	clearZ();
	draw(scn.end);
	ppBloom(0.2f, 114, 200, 160);
}

void TestDemo::endBarbie2()
{
	draw(scn.endBack);
	ppBloom(0.2f, 114, 200, 160);
}









void TestDemo::loadFlower()
{
	printf("%s\n", __FUNCTION__);
	loadTextures(scn.flower);
	loadTextures(scn.face);
}

void TestDemo::loadSpiral()
{
	printf("%s\n", __FUNCTION__);
	unloadTextures(scn.walk);
	unloadTextures(scn.walkBG);
	loadTextures(scn.spiral);
}

int TestDemo::loadTextThreaded(SceSize args, void *argp)
{
	TestDemo* demo = *(TestDemo**)argp;

	printf("running loadTextThreaded.\n");
	demo->load(demo->scn.textWalk, "text\\psp\\text.msk");
	printf("done loadTextThreaded.***********************************************\n");
	prepareChars(*demo->scn.textWalk.renderable);
	sceKernelExitDeleteThread(0);
	return 0;
}

void TestDemo::loadTextScene()
{
	unloadTextures(scn.logo);
	scn.walk.renderable->mResources.animCharSet.reset();
	{
		SceUID textth = sceKernelCreateThread("loadTextThreaded", loadTextThreaded, 0x12, 0x10000, PSP_THREAD_ATTR_USER, NULL);
		if (textth < 0)
		{
			printf("Error creating loadTextThreaded.\n");
			load(scn.textWalk,	"text\\psp\\text.msk");
			return;
		}

		void* p = this;
		sceKernelStartThread(textth, sizeof(void*), &p);
		sceKernelChangeThreadPriority(textth, 0x40);
	}
}

void TestDemo::loadPhoneA()
{
	printf("%s\n", __FUNCTION__);
	loadTextures(scn.phone1);
	loadTextures(scn.phoneTrans);
}

void TestDemo::loadPhoneB()
{
	printf("%s\n", __FUNCTION__);
	unloadTextures(scn.flower);
	unloadTextures(scn.face);
	loadTextures(scn.phone2);
	loadTextures(scn.phone3);
	loadTextures(scn.phone4);
}

void TestDemo::loadText()
{
	printf("%s\n", __FUNCTION__);
	scn.spiral.renderable->mResources.animCharSet.reset();
	unloadTextures(scn.spiral);
	unloadTextures(scn.phone1);
	loadTextures(scn.textWalk);
	loadTextures(scn.textBG);
	loadTextures(scn.text);
	loadTextures(scn.jealousy);
}

void TestDemo::loadDreamPre()
{
	printf("%s\n", __FUNCTION__);
	unloadTextures(scn.phone2);
	unloadTextures(scn.phone3);
	loadTextures(scn.beer1);
	loadTextures(scn.beer2);
}

void TestDemo::loadDreamA()
{
	printf("%s\n", __FUNCTION__);
	unloadTextures(scn.phone4);
	loadTextures(scn.garlic1);
	loadTextures(scn.garlic2);
}

void TestDemo::loadDreamB()
{
	printf("%s\n", __FUNCTION__);
	unloadTextures(scn.textWalk);
	unloadTextures(scn.textBG);
	unloadTextures(scn.text);
	unloadTextures(scn.jealousy);
	loadTextures(scn.mix1);
	loadTextures(scn.mix2);
	loadTextures(scn.mix3);
}

int TestDemo::loadWeaponThreaded(SceSize args, void *argp)
{
	TestDemo* demo = *(TestDemo**)argp;

	demo->load(demo->scn.reload,	"reload\\psp\\reload.msk");
	demo->load(demo->scn.m16,		"weapon3\\psp\\weapon3.msk");
	demo->load(demo->scn.gun,		"weapon2\\psp\\gun.msk");

	printf("done loadWeaponThreaded.***********************************************\n");
	sceKernelExitDeleteThread(0);
	return 0;
}

void TestDemo::loadWeaponScenes()
{
	printf("%s\n", __FUNCTION__);
	scn.textWalk.renderable->mResources.animCharSet.reset();

	SceUID th = sceKernelCreateThread("loadWeaponThreaded", loadWeaponThreaded, 0x12, 0x10000, PSP_THREAD_ATTR_USER, NULL);
	if (th < 0)
	{
		printf("Error creating loadWeaponThreaded.\n");
		load(scn.reload,	"reload\\psp\\reload.msk");
		load(scn.m16,		"weapon3\\psp\\weapon3.msk");
		load(scn.gun,		"weapon2\\psp\\gun.msk");
		return;
	}

	void* p = this;
	sceKernelStartThread(th, sizeof(void*), &p);
	sceKernelChangeThreadPriority(th, 0x40);
}

void TestDemo::loadWeaponA()
{
	printf("%s\n", __FUNCTION__);
	unloadTextures(scn.beer1);
	unloadTextures(scn.beer2);
	unloadTextures(scn.garlic1);
	unloadTextures(scn.garlic2);

	loadTextures(scn.reload);
	loadTextures(scn.gun);
	loadTextures(scn.m16);
}

void TestDemo::loadWeaponB()
{
	printf("%s\n", __FUNCTION__);
	unloadTextures(scn.mix1);
	unloadTextures(scn.mix2);
	unloadTextures(scn.mix3);

	loadTextures(scn.bullet1);
	loadTextures(scn.bullet2);
	loadTextures(scn.expGirl1BG);
	loadTextures(scn.expGirl2BG);
	loadTextures(scn.expGirl1);
	loadTextures(scn.expGirl2);
}

int TestDemo::loadExploThreaded(SceSize args, void *argp)
{
	TestDemo* demo = *(TestDemo**)argp;

	printf("running loadExploThreaded.\n");
	demo->load(demo->scn.bullet1,	"bull1\\psp\\bull1.msk");
	demo->load(demo->scn.bullet2,	"bull2\\psp\\bull2.msk");
	demo->load(demo->scn.expGirl1BG,"back_01\\psp\\back_01.msk");
	demo->load(demo->scn.expGirl2BG,"back_02\\psp\\back_02.msk");
	demo->load(demo->scn.expGirl1,	"exgirl1\\psp\\exgirl1.msk");
	demo->load(demo->scn.expGirl2,	"exgirl2\\psp\\exgirl2.msk");

	printf("done loadExploThreaded.***********************************************\n");
	sceKernelExitDeleteThread(0);
	return 0;
}

void TestDemo::loadExploScenes()
{
	scn.phone1.renderable->mResources.animCharSet.reset();	
	scn.phone2.renderable->mResources.animCharSet.reset();	
	scn.phone3.renderable->mResources.animCharSet.reset();	
	scn.phone4.renderable->mResources.animCharSet.reset();	
	scn.phoneTrans.renderable->mResources.animCharSet.reset();
	scn.textBG.renderable->mResources.animCharSet.reset();	
	scn.text.renderable->mResources.animCharSet.reset();		
	scn.jealousy.renderable->mResources.animCharSet.reset();	
	scn.beer1.renderable->mResources.animCharSet.reset();		
	scn.beer2.renderable->mResources.animCharSet.reset();		
	scn.garlic1.renderable->mResources.animCharSet.reset();	
	scn.garlic2.renderable->mResources.animCharSet.reset();	

	SceUID th = sceKernelCreateThread("loadExploThreaded", loadExploThreaded, 0x12, 0x10000, PSP_THREAD_ATTR_USER, NULL);
	if (th < 0)
	{
		printf("Error creating loadWeaponThreaded.\n");
		load(scn.bullet1,	"bull1\\psp\\bull1.msk");
		load(scn.bullet2,	"bull2\\psp\\bull2.msk");
		load(scn.expGirl1BG,"back_01\\psp\\back_01.msk");
		load(scn.expGirl2BG,"back_02\\psp\\back_02.msk");
		load(scn.expGirl1,	"exgirl1\\psp\\exgirl1.msk");
		load(scn.expGirl2,	"exgirl2\\psp\\exgirl2.msk");
		return;
	}

	void* p = this;
	sceKernelStartThread(th, sizeof(void*), &p);
	sceKernelChangeThreadPriority(th, 0x40);
}

int TestDemo::loadWindowThreaded(SceSize args, void *argp)
{
	TestDemo* demo = *(TestDemo**)argp;

	printf("running loadWindowThreaded.\n");
	demo->load(demo->scn.windowBarbie,	
						"suicidebarbie1\\psp\\suicidebarbie1.msk");
	demo->load(demo->scn.window,	"window\\psp\\window.msk");

	printf("done loadWindowThreaded.***********************************************\n");
	sceKernelExitDeleteThread(0);
	return 0;
}

void TestDemo::loadWindowScenes()
{
	scn.mix1.renderable->mResources.animCharSet.reset();	
	scn.mix2.renderable->mResources.animCharSet.reset();	
	scn.mix3.renderable->mResources.animCharSet.reset();	
	scn.reload.renderable->mResources.animCharSet.reset();
	scn.m16.renderable->mResources.animCharSet.reset();		
	scn.gun.renderable->mResources.animCharSet.reset();
//	scn.bullet1.renderable->mResources.animCharSet.reset();
//	scn.expGirl1BG.renderable->mResources.animCharSet.reset();
//	scn.expGirl1.renderable->mResources.animCharSet.reset();	
//	scn.bullet2.renderable->mResources.animCharSet.reset();	

	SceUID th = sceKernelCreateThread("loadWindowThreaded", loadWindowThreaded, 0x12, 0x10000, PSP_THREAD_ATTR_USER, NULL);
	if (th < 0)
	{
		printf("Error creating loadWindowThreaded.\n");
		load(scn.windowBarbie,	
						"suicidebarbie1\\psp\\suicidebarbie1.msk");
		load(scn.window,	"window\\psp\\window.msk");
		return;
	}

	void* p = this;
	sceKernelStartThread(th, sizeof(void*), &p);
	sceKernelChangeThreadPriority(th, 0x40);
}

void TestDemo::loadWindow()
{
	unloadTextures(scn.bullet1);
	unloadTextures(scn.expGirl1BG);
	unloadTextures(scn.expGirl1);

	loadTextures(scn.windowBarbie);
	loadTextures(scn.window);
}

int TestDemo::loadEndThreaded(SceSize args, void *argp)
{
	TestDemo* demo = *(TestDemo**)argp;

	printf("running loadEndThreaded.\n");
	demo->load(demo->scn.endBack,	"suicidebarbie2\\psp\\suicidebarbie_back2.msk");
	demo->load(demo->scn.end,	"suicidebarbie2\\psp\\suicidebarbie2.msk");

	printf("done loadEndThreaded.***********************************************\n");
	sceKernelExitDeleteThread(0);
	return 0;
}

void TestDemo::loadEndScenes()
{
	scn.bullet1.renderable->mResources.animCharSet.reset();
	scn.expGirl1BG.renderable->mResources.animCharSet.reset();
	scn.expGirl1.renderable->mResources.animCharSet.reset();	
	scn.bullet2.renderable->mResources.animCharSet.reset();	
	scn.expGirl2BG.renderable->mResources.animCharSet.reset();
	scn.expGirl2.renderable->mResources.animCharSet.reset();	
	scn.windowBarbie.renderable->mResources.animCharSet.reset();	
	scn.window.renderable->mResources.animCharSet.reset();

	scn.expGirl1BG.renderable->mResources.meshes.resize(0);
	scn.expGirl1.renderable->mResources.meshes.resize(0);	
	scn.bullet2.renderable->mResources.meshes.resize(0);	
	scn.expGirl2BG.renderable->mResources.meshes.resize(0);
	scn.expGirl2.renderable->mResources.meshes.resize(0);	
	scn.windowBarbie.renderable->mResources.meshes.resize(0);	
	scn.window.renderable->mResources.meshes.resize(0);

	unloadTextures(scn.bullet2);
	unloadTextures(scn.expGirl2BG);
	unloadTextures(scn.expGirl2);
	unloadTextures(scn.windowBarbie);
	unloadTextures(scn.window);

		load(scn.endBack,	"suicidebarbie2\\psp\\suicidebarbie_back2.msk");
		load(scn.end,	"suicidebarbie2\\psp\\suicidebarbie2.msk");
	return;

	SceUID th = sceKernelCreateThread("loadEndThreaded", loadEndThreaded, 0x12, 0x10000, PSP_THREAD_ATTR_USER, NULL);
	if (th < 0)
	{
		printf("Error creating loadEndThreaded.\n");
		load(scn.endBack,	"suicidebarbie2\\psp\\suicidebarbie_back2.msk");
		load(scn.end,	"suicidebarbie2\\psp\\suicidebarbie2.msk");
		return;
	}

	void* p = this;
	sceKernelStartThread(th, sizeof(void*), &p);
	sceKernelChangeThreadPriority(th, 0x40);
}

void TestDemo::loadEnd()
{

	loadTextures(scn.endBack);
	loadTextures(scn.end);
}
