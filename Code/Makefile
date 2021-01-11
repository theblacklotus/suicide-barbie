.SUFFIXES:

.PHONY: debug release debug_oe release_oe clean libs libs_debug elf elf_debug prx prx_debug
.EXPORT_ALL_VARIABLES:

all: debug release debug_oe release_oe

debug: CONFIG = DEBUG
debug: elf_debug
release: elf
release_oe: BUILD_OE = 1
release_oe: prx
debug_oe: CONFIG = DEBUG
debug_oe: BUILD_OE = 1
debug_oe: prx_debug

libs_debug:
	@$(MAKE) $(MAKEFLAGS) -C zlib LIB
	@$(MAKE) $(MAKEFLAGS) -C Base LIB
	@$(MAKE) $(MAKEFLAGS) -C Modules/animator LIB
	@$(MAKE) $(MAKEFLAGS) -C Modules/effects LIB
	@$(MAKE) $(MAKEFLAGS) -C Modules/mutalisk LIB
	@$(MAKE) $(MAKEFLAGS) -C Modules/mutant LIB
	@$(MAKE) $(MAKEFLAGS) -C Modules/player LIB

libs:
	@$(MAKE) $(MAKEFLAGS) -C zlib LIB
	@$(MAKE) $(MAKEFLAGS) -C Base LIB
	@$(MAKE) $(MAKEFLAGS) -C Modules/animator LIB
	@$(MAKE) $(MAKEFLAGS) -C Modules/effects LIB
	@$(MAKE) $(MAKEFLAGS) -C Modules/mutalisk LIB
	@$(MAKE) $(MAKEFLAGS) -C Modules/mutant LIB
	@$(MAKE) $(MAKEFLAGS) -C Modules/player LIB

elf_debug: libs_debug
	@$(MAKE) $(MAKEFLAGS) -C Tests/MutaliskViewer PBP

elf: libs
	@$(MAKE) $(MAKEFLAGS) -C Tests/AnimationTest ELF
	@$(MAKE) $(MAKEFLAGS) -C Tests/BlurTest ELF
	@$(MAKE) $(MAKEFLAGS) -C Tests/MutaliskTest ELF
	@$(MAKE) $(MAKEFLAGS) -C Tests/MutaliskViewer ELF
	@$(MAKE) $(MAKEFLAGS) -C Tests/PSPStubTest ELF
	@$(MAKE) $(MAKEFLAGS) -C Tests/ScreenTest ELF
	@$(MAKE) $(MAKEFLAGS) -C Tests/WavTest ELF
	@$(MAKE) $(MAKEFLAGS) -C Tests/MutaliskViewer PBP

prx: libs
	@$(MAKE) $(MAKEFLAGS) -C Tests/MutaliskViewer PBP

prx_debug: libs_debug
	@$(MAKE) $(MAKEFLAGS) -C Tests/MutaliskViewer PBP

clean:
	rm -rf ../Build ../Output
