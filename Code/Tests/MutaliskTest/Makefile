SRCS = $(wildcard *.c)
SRCS+= $(wildcard *.cpp)

CC_FLAGS=\
	-Wno-unused\

INCLUDE=\
	-I"$(ROOT)/Code/Modules"\
	-I"$(ROOT)/Code/Modules/mutant"\

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
	$(OUTDIR)/zlib.lib\
	$(OUTDIR)/Base.lib\
	$(OUTDIR)/player.lib\
	$(OUTDIR)/mutalisk.lib\
	$(OUTDIR)/mutant.lib\
	../ScreenTest/logo.o

include ../../build.mak
