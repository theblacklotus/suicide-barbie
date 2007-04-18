########################################################
PROJECT = SuicideBarbie
ifneq ($(CONFIG),)
PSP_EBOOT_TITLE = Suicide Barbie ($(CONFIG))
else
PSP_EBOOT_TITLE = Suicide Barbie
endif

LCFILE = stripped.ld
LIBS=\
	$(PSPDEV)/psp/lib/libstdc++.a\
	$(PSPDEV)/psp/lib/libm.a\
	$(PSPDEV)/psp/lib/libc.a\
	$(PSPDEV)/psp/sdk/lib/libpspvfpu.a\
	$(PSPDEV)/psp/sdk/lib/libpspaudio.a\
	$(PSPDEV)/psp/sdk/lib/libpspatrac3.a\
	$(PSPDEV)/psp/sdk/lib/libpspsdk.a\
	$(PSPDEV)/psp/sdk/lib/libpsppower.a\
	$(PSPDEV)/psp/sdk/lib/libpsprtc.a\
	$(PSPDEV)/psp/sdk/lib/libpspgum.a\
	$(PSPDEV)/psp/sdk/lib/libpspgu.a\
	$(PSPDEV)/psp/sdk/lib/libpspge.a\
	$(PSPDEV)/psp/sdk/lib/libpspdisplay.a\
	$(PSPDEV)/psp/sdk/lib/libpspctrl.a\
	$(PSPDEV)/psp/sdk/lib/libpspnet.a\
	$(PSPDEV)/psp/sdk/lib/libpspnet_inet.a\
	$(PSPDEV)/psp/sdk/lib/libpspdebug.a\
	$(OUTDIR)/effects.lib\
	$(OUTDIR)/player.lib\
	$(OUTDIR)/zlib.lib\
	$(OUTDIR)/Base.lib\
	$(OUTDIR)/mutalisk.lib\
	$(OUTDIR)/mutant.lib\

INCLUDE=\
	-I"$(ROOT)/Code/Modules"\
	-I"$(ROOT)/Code/Modules/mutant"\
	-I"$(ROOT)/Code/Modules/mutalisk"\

CC_FLAGS=\
	-Wno-unused\
	-Wno-multichar\
	-Wno-unknown-pragmas\

SRCS=\
	AnimCreator.cpp\
	BallRenderer.cpp\
	BlinkyBlinky.cpp\
	callbacks.c\
	CharRenderer.cpp\
	intro.cpp\
	PostProcess.cpp\
	TestDemo.cpp\
	TimeBlock.cpp\
	TimelinePlayer.cpp\
	vram.c\

LD_FLAGS=\
	-T $(LCFILE)\
	-fno-exceptions\
	-Wl,-Map="$(OUTDIR)/$(PROJECT).txt"\
	-L$(PSPDEV)/psp/sdk/lib\
	-lpspuser\
	-lpspkernel\
	-Wl,-s\
	-Wl,-x\
	-Wl,--gc-sections\
	-Wl,--wrap=malloc\
	-Wl,--wrap=free\
	-Wl,--wrap=calloc\
	-Wl,--defsym,sceNetInetGetsockopt=0,--defsym,sceNetInetGetErrno=0,--defsym,sceNetInetSetsockopt=0\

include ../../build.mak
