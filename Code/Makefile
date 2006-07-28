.SUFFIXES:

.PHONY: debug release clean libs libs_debug elf elf_debug
.EXPORT_ALL_VARIABLES:

all: debug release

debug: CONFIG = DEBUG
debug: elf_debug
release: elf

libs_debug:
	@$(MAKE) $(MAKEFLAGS) -C Base LIB
	@$(MAKE) $(MAKEFLAGS) -C Modules/mutant LIB

libs:
	@$(MAKE) $(MAKEFLAGS) -C Base LIB
	@$(MAKE) $(MAKEFLAGS) -C Modules/mutant LIB

elf_debug: libs_debug

elf: libs
	@$(MAKE) $(MAKEFLAGS) -C Tests/PSPStubTest ELF

clean:
	rm -rf ../Build ../Output
