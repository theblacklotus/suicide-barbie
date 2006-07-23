.SUFFIXES:

.PHONY: debug release clean libs libs_debug elf elf_debug
.EXPORT_ALL_VARIABLES:

all: debug release

debug: CONFIG = DEBUG
debug: elf_debug
release: elf

libs_debug:
	@$(MAKE) $(MAKEFLAGS) -C Base LIB

libs:
	@$(MAKE) $(MAKEFLAGS) -C Base LIB

elf_debug: libs_debug

elf: libs

clean:
	rm -rf ../Build ../Output
