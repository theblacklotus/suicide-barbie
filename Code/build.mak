########################################################

.SILENT:
.SUFFIXES:
.SUFFIXES: .c .cpp .cxx .cc .s .asm
.PHONY: LIB ELF PRX PBP

########################################################

ROOT := $(abspath $(addsuffix ..,$(dir $(abspath $(lastword $(MAKEFILE_LIST))))))

PROJECT_PATH := $(abspath $(firstword $(MAKEFILE_LIST)))
PROJECT_DIRNAME := $(patsubst %/,%,$(dir $(PROJECT_PATH)))
PROJECT ?= $(notdir $(PROJECT_DIRNAME))

########################################################

CONFIG?=RELEASE

OUTDIR = $(ROOT)/Output/$(CONFIG)
INTDIR = $(ROOT)/Build/$(CONFIG)/$(PROJECT)

$(info ~~~~~~~~~~~~~ PROJECT = $(PROJECT) ; CONFIG = $(CONFIG))

########################################################

INCLUDE:=\
	-I"$(PSPDEV)/psp/sdk/include"\
	-I"$(ROOT)/Code"\
	$(INCLUDE)\

AS=psp-gcc
CC=psp-gcc
AR=psp-ar
LD=psp-gcc

AS_FLAGS=\
	-DNDEBUG\
	-D__GCC__\
	$(INCLUDE)\
	-I. -c\

CC_FLAGS_COMMON=\
	-D__GCC__\
	$(INCLUDE)\
	-I. -c\
	-include "ForcedInclude.h"\
	-MMD    \
	-Wall\
	-fno-exceptions\

CC_FLAGS_DEBUG=\
	-O0\
	-g\
	-D_DEBUG\

CC_FLAGS_RELEASE=\
	-O3\
	-DNDEBUG\
	-fshort-wchar\
	-ffast-math\
	-fomit-frame-pointer\

CC_FLAGS:=\
	$(CC_FLAGS_$(CONFIG))\
	$(CC_FLAGS_COMMON)\
	$(CC_FLAGS)\

CXX_FLAGS=$(CC_FLAGS)

ifndef LIBS
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

endif

ifndef LD_FLAGS
LD_FLAGS=\
	-L$(PSPDEV)/psp/sdk/lib\
	-lpspuser\
	-lpspkernel\

endif

########################################################

OBJS  = $(addprefix $(INTDIR)/,$(filter %.obj,$(SRCS:.c=.obj)))
OBJS += $(addprefix $(INTDIR)/,$(filter %.obj,$(SRCS:.cpp=.obj)))

COMPILE: $(OBJS)

$(OBJS) : $(firstword $(MAKEFILE_LIST)) $(lastword $(MAKEFILE_LIST))

-include $(OBJS:.obj=.d)

########################################################

$(INTDIR)/%.obj: %.c
	@echo Compiling "$<"
	$(CC) $(CC_FLAGS) "$<" -o $@

$(INTDIR)/%.obj: %.cpp
	@echo Compiling "$<"
	$(CC) $(CC_FLAGS) "$<" -o $@

########################################################

PROJECT_PATH=$(OUTDIR)/$(PROJECT)

########################################################

LIB: $(PROJECT_PATH).lib

$(OUTDIR)/$(PROJECT).lib : $(OBJS)
	@echo Creating Library $@
	$(AR) rvus $@ $(OBJS)

########################################################

ELF: $(PROJECT_PATH).elf

$(PROJECT_PATH).elf : $(OBJS) $(LCFILE) $(LIBS)
	@echo Linking $@
	$(LD) -Wl,--start-group $(OBJS) $(LIBS) -Wl,--end-group $(LD_FLAGS) -o $@
	@echo Fixup imports $@
	psp-fixup-imports $@

########################################################

PRX: $(PROJECT_PATH).prx

$(PROJECT_PATH).prx : $(OBJS) $(LIBS)
	@echo Linking $@.elf
	$(LD) -Wl,--start-group $(OBJS) $(LIBS) -Wl,--end-group $(LD_FLAGS) -o $@.elf
	@echo Fixup imports $@.elf
	psp-fixup-imports $@.elf
	@echo PRXgen $@
	psp-prxgen $@.elf $@

########################################################

ifndef PSP_EBOOT_TITLE
PSP_EBOOT_TITLE = $(PROJECT)
endif

EBOOT_TARGET = $(OUTDIR)/%__SCE__$(PROJECT)/EBOOT.PBP
$(EBOOT_TARGET): $(PROJECT_PATH).elf $(INTDIR)/PARAM.SFO
	@echo Creating '$(EBOOT_TARGET)' PBP
	mkdir -p "$(OUTDIR)/__SCE__$(PROJECT)"
	psp-strip --remove-section=.comment --remove-section=.eh_frame --remove-section=.gcc_except_table --remove-section=.mdebug.eabi32 --remove-section=.gcc_compiled_long32 --remove-section=.lib.ent.top --remove-section=.lib.ent.btm --remove-section=.lib.stub.top --remove-section=.lib.stub.btm --remove-section=.sceStub.text "$(PROJECT_PATH).elf" -o "$(OUTDIR)/__SCE__$(PROJECT)/EBOOT.PBP"
	mkdir -p "$(OUTDIR)/%__SCE__$(PROJECT)"
	pack-pbp "$(EBOOT_TARGET)" "$(INTDIR)/PARAM.SFO" "ICON0.PNG" NULL NULL "PIC1.PNG" NULL "$(OUTDIR)/__SCE__$(PROJECT)/EBOOT.PBP" NULL

PBP: $(EBOOT_TARGET)

########################################################

$(INTDIR)/PARAM.SFO: 
	@echo Creating '$(PSP_EBOOT_TITLE)' SFO
	mksfo '$(PSP_EBOOT_TITLE)' $@


########################################################

$(shell mkdir -p $(INTDIR) $(OUTDIR) $(dir $(OBJS)) > /dev/null)

########################################################
