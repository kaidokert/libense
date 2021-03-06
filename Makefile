# main targets and particle libraries
TARGETS := main
PARTICLES := ense libcxxabi/src libcxx/src libc/src/fenv libc/src/math

CPU := cortex-m4
FLOAT_ABI := hard
FPU := fpv4-sp-d16
TARGET := arm-none-eabi
PART_FAMILY := stm32f4
PART_SERIES := 0

# default compiler/linker flags
CPPFLAGS += -I libc/include -I libcxx/include
CPPFLAGS += -I include
CPPFLAGS += -I platform/$(PART_FAMILY)$(PART_SERIES) -I platform/$(PART_FAMILY)
CPPFLAGS += -D_XOPEN_SOURCE

CCFLAGS_RELEASE := -O2 -g
CCFLAGS_DEBUG := -O2 -g

#LDFLAGS_RELEASE := --plugin /home/dhivael/local/lib/LLVMgold.so --plugin-opt "mcpu=$(CPU)"
#LDFLAGS_DEBUG :=

# libgcc hast -fshort-enums, and it modifies ABI
ABI_CCFLAGS := -mcpu=$(CPU) -mthumb -mfpu=$(FPU) -mfloat-abi=$(FLOAT_ABI) -fshort-enums
#CCFLAGS += -Werror
CCFLAGS += -Wall -Wextra -pedantic -mstrict-align -fno-common
CCFLAGS += $(ABI_CCFLAGS)
CCFLAGS += -ffreestanding -nostdlib -nostdlibinc
CCFLAGS += -fno-stack-protector
CCFLAGS += -ffunction-sections -fdata-sections
CCFLAGS += -include .ense-config.h
CFLAGS += $(CCFLAGS) -std=c99
CXXFLAGS += $(CCFLAGS) -std=c++1y -fno-exceptions
ASFLAGS := -mcpu=$(CPU) -mthumb -mfloat-abi=$(FLOAT_ABI) -mfpu=$(FPU) -meabi=5

LDFLAGS += -nostdlib --gc-sections

# default values for internal variables
CCPREFIX := $(if $(TARGET),$(TARGET)-,)
AS := $(CCPREFIX)as
CC := clang -target $(TARGET) -integrated-as
CXX := clang++ -target $(TARGET) -integrated-as
CPP := $(CXX) -E
LD := $(CCPREFIX)ld
SED := sed
FIND := find
OBJCOPY := $(CCPREFIX)objcopy

OBJDIR := obj
BINDIR := bin


# extensions of files considered to be code
# supported so far:
# C++: cpp
# C: c
# Assembler: S
override CODE_EXTS := cpp c S

# external libraries used
override LIBRARIES :=
override LIBRARIES_WITHOUT_PKGCONFIG :=

# library version requirements check
define LIBRARY_VERSION_CHECK
endef



###
# here be internals
###

MAKE_DEPS := Makefile ense-config

-include ense-config

.ense-config.h: $(MAKE_DEPS)
	printf "" > $@
	$(foreach conf-var,\
		$(filter CONFIG_%,$(.VARIABLES)),\
		printf "#define $(conf-var) $($(conf-var))\n" >> $@)

TARGET_LDSCRIPT = $(target-objdir)/main.ld

LIBGCC_DIR := $(dir $(shell $(CCPREFIX)gcc $(ABI_CCFLAGS) -print-libgcc-file-name))

V := @
ifneq ($V, @)
	override V :=
endif

# result: cleaned name of particle
# argument 1: makefile name or directory name of particle root
define submk_name
$(subst /,_,$(subst _,__,$(1:/dir.mk=)))
endef

# result: particles library filename
# argument 1: makefile name or directory name of particle root
define sublib_name
lib$(call submk_name,$1).a
endef



ifneq ($(origin RELEASE),undefined)
  CCFLAGS += $(CCFLAGS_RELEASE)
  LDFLAGS += $(LDFLAGS_RELEASE)
  ARFLAGS := $(ARFLAGS_RELEASE)
  target-objdir := $(OBJDIR)/release
  target-bindir := $(BINDIR)/release
else
  CCFLAGS += $(CCFLAGS_DEBUG)
  LDFLAGS += $(LDFLAGS_DEBUG)
  ARFLAGS := $(ARFLAGS_DEBUG)
  target-objdir := $(OBJDIR)/debug
  target-bindir := $(BINDIR)/debug
endif



ram-update:
	$Vopenocd -f ocd/f4.cfg -c "init; ram-update $(target-bindir)/$(word 1,$(TARGETS)).bin; shutdown"

.DEFAULT_GOAL :=



ifdef LIBRARIES
  ifndef LIBRARY_VERSION_CHECK
    LIBRARY_VERSION_CHECK := pkg-config --print-errors $(LIBRARIES)
  endif

  ifndef MAKE_RESTARTS
    version-check-result := $(shell ($(LIBRARY_VERSION_CHECK)); echo $$?)
    ifneq ($(version-check-result),0)
      $(error Version checks failed)
    endif
  endif
endif

SRC := $(foreach ext,$(CODE_EXTS),$(wildcard *.$(ext)))

DEP_SRC := $(SRC)

DIRS := $(target-bindir) $(subst ./,,$(sort $(patsubst %,$(target-objdir)/%,$(dir $(DEP_SRC)))))

PARTICLE_MAKEFILES := $(patsubst %,%/dir.mk,$(PARTICLES))

TARGET_EXECUTABLES := $(patsubst %,$(target-bindir)/%,$(TARGETS))

.PHONY: all
all: $(TARGET_EXECUTABLES) $(patsubst %,%.bin,$(TARGET_EXECUTABLES))

debug:
	@$(MAKE) all

release:
	@$(MAKE) all RELEASE=

-include $(PARTICLE_MAKEFILES)


# result: cflags to also generate a dependency file for make
# argument 1: output object file
gendep_cflags = -MD -MP -MT "$(strip $1 $1.d)" -MF $1.d

-include $(foreach ext,$(CODE_EXTS),$(patsubst %.$(ext),$(target-objdir)/%.o.d,$(filter %.$(ext),$(DEP_SRC))))

PARTICLE_LIBRARY_NAMES := $(foreach lib,$(PARTICLES),$(call sublib_name,$(lib)))
PARTICLE_LIBRARIES := $(foreach lib,$(PARTICLES),-l$(call submk_name,$(lib)))

LDFLAGS += -L $(target-objdir) -L$(LIBGCC_DIR)
LDFLAGS += $(patsubst %,-l%,$(LIBRARIES_WITHOUT_PKGCONFIG))
ifneq (,$(LIBRARIES))
  CCFLAGS += `pkg-config --cflags $(LIBRARIES)`
  LDFLAGS += `pkg-config --libs $(LIBRARIES)`
endif
LDFLAGS += --start-group $(PARTICLE_LIBRARIES) --end-group -lgcc
LDFLAGS += -T $(target-objdir)/main.ld


$(BINDIR)/%.bin: $(BINDIR)/%
	@echo "[OBJCPY]" $@
	$V$(OBJCOPY) -O binary $< $@

$(target-bindir)/%: $(target-objdir)/%.o $(patsubst %,$(target-objdir)/%,$(PARTICLE_LIBRARY_NAMES)) $(TARGET_LDSCRIPT) | $(target-bindir)
	@echo "[LD]	" $@
	$V$(LD) -o $@ $< $(LDFLAGS)

.SECONDARY: $(patsubst %,$(target-objdir)/%.o,$(TARGETS))

.PHONY: clean
clean:
	-$(RM) -r $(OBJDIR) $(BINDIR) $(PARTICLE_MAKEFILES)

.PHONY: depclean
depclean:
	-$(FIND) $(OBJDIR) -iname '*.d' -delete

.PHONY: distclean
distclean:
	-$(RM) -r $(BINDIR)

$(target-objdir)/%.o: %.cpp $(MAKE_DEPS) | $(DIRS)
	@echo "[CXX]	" $<
	$V$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(call gendep_cflags, $@) -c -o $@ $<

$(target-objdir)/%.o: %.c $(MAKE_DEPS) | $(DIRS)
	@echo "[CC]	" $<
	$V$(CC) $(CPPFLAGS) $(CFLAGS) $(call gendep_cflags, $@) -c -o $@ $<

$(target-objdir)/%.o: %.S $(MAKE_DEPS) | $(DIRS)
	@echo "[AS]	" $<
	$V$(AS) $(ASFLAGS) -o $@ $<

.PHONY: $(DIRS)
$(DIRS):
	@mkdir -p $@

$(PARTICLE_MAKEFILES): $(MAKE_DEPS)
	@echo "[GEN]	" $@
	$(file >$@,$(call generate_subdir_makefile,$@))

-include $(TARGET_LDSCRIPT).d
$(TARGET_LDSCRIPT): ldscripts/$(PART_FAMILY)/$(PART_FAMILY)$(PART_SERIES).ld $(MAKE_DEPS) | $(DIRS)
	@echo "[GENLD]	" $<
	$V$(CPP) -x c -nostdinc -I ldscripts -P $(call gendep_cflags, $@) -o $@ $<



# result: shell commands to create a particle Makefile
# argument 1: directory
define generate_subdir_makefile
__objdir := $$(target-objdir)/$(1:/dir.mk=)
DIRS += $$(__objdir)

__vars := $$(filter-out __%, $$(filter %FLAGS, $$(.VARIABLES)))
$$(foreach var, $$(__vars), $$(eval __$$(var) := $$($$(var))))

-include $(1:/dir.mk=/extraflags.mk)

$$(foreach var, $$(filter-out __%, $$(filter %FLAGS, $$(.VARIABLES))), \
	$$(eval $$(__objdir)/%.o: $$(var) := $$($$(var))))
$$(foreach var, $$(__vars), \
	$$(eval $$(var) := $$(__$$(var))))

$(call submk_name,$1)_SRC := $(foreach ext,$(CODE_EXTS),$$(wildcard $(dir $1)*.$(ext)))

$(call submk_name,$1)_OBJ := $(foreach ext,$(CODE_EXTS),$$(patsubst %.$(ext),$$(target-objdir)/%.o,$$(filter %.$(ext),$$($(call submk_name,$1)_SRC))))

ifneq ($$(wildcard $(1:/dir.mk=/extraflags.mk)),)
$$($(call submk_name,$1)_OBJ): $(1:/dir.mk=/extraflags.mk)
endif

DEP_SRC += $$($(call submk_name,$1)_SRC)

$$(target-objdir)/$(call sublib_name,$1): $$($(call submk_name,$1)_OBJ)
	@echo "[AR]	" $$@
	$$V$$(AR) $$(ARFLAGS) -rcs $$@ $$^
endef
