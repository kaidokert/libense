# main targets and particle libraries
TARGETS := main
PARTICLES := ense

CPU := cortex-m4
FLOAT_ABI := hard
FPU := vfp
TARGET := arm-none-eabi

# default compiler/linker flags
CPPFLAGS += -I include -I ../libcxx/include

CCFLAGS_RELEASE := -O2 -emit-llvm
CCFLAGS_DEBUG := -O2 -g

LDFLAGS_RELEASE := -plugin /usr/lib/LLVMgold.so -plugin-opt "mcpu=$(CPU)"
LDFLAGS_DEBUG :=

#CCFLAGS += -Werror
CCFLAGS += -Wall -Wextra -pedantic -mstrict-align
CCFLAGS += -mcpu=$(CPU) -mthumb -mfloat-abi=$(FLOAT_ABI)
CCFLAGS += -ffreestanding -nostdlib
CCFLAGS += -fno-exceptions -fno-rtti -fno-stack-protector
CCFLAGS += -ffunction-sections -fdata-sections
CFLAGS += $(CCFLAGS) -std=c99
CXXFLAGS += $(CCFLAGS) -std=c++11 -nostdinc++
ASFLAGS := -mcpu=$(CPU) -mthumb -mfloat-abi=$(FLOAT_ABI) -mfpu=$(FPU) -meabi=5

LDFLAGS += -nostdlib -L/usr/lib/gcc/arm-none-eabi/4.9.0/thumb/$(CPU)/ -lgcc -T ldscripts/stm32f4/f4.ld

# default values for internal variables
CCPREFIX := $(if $(TARGET),$(TARGET)-,)
AS := $(CCPREFIX)as
CC := clang -target $(TARGET) -integrated-as
CXX := clang++ -target $(TARGET) -integrated-as
CPP := $(CXX) -E
LD := $(CXX)
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

TARGET_NAME := $(if $(RELEASE),release,debug)
TARGET_NAME_FILE = $(target-objdir)/.target

V := @

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
  target-objdir := $(OBJDIR)/release
  target-bindir := $(BINDIR)/release
else
  CCFLAGS += $(CCFLAGS_DEBUG)
  LDFLAGS += $(LDFLAGS_DEBUG)
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

ifneq ($(MAKECMDGOALS),clean)
ifndef MAKE_RESTARTS
$(shell \
	mkdir -p $(target-objdir); \
	if [ ! -f $(TARGET_NAME_FILE) ]; then echo $(TARGET_NAME) > $(TARGET_NAME_FILE); fi; \
	if [ x`cat $(TARGET_NAME_FILE)` != x$(TARGET_NAME) ]; then echo $(TARGET_NAME) > $(TARGET_NAME_FILE); fi \
)
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
-include $(foreach ext,$(CODE_EXTS),$(patsubst %.$(ext),$(target-objdir)/%.o.d,$(filter %.$(ext),$(DEP_SRC))))

PARTICLE_LIBRARY_NAMES := $(foreach lib,$(PARTICLES),$(call sublib_name,$(lib)))
PARTICLE_LIBRARIES := $(foreach lib,$(PARTICLES),-l$(call submk_name,$(lib)))

LDFLAGS += -L $(target-objdir)
LDFLAGS += $(patsubst %,-l%,$(LIBRARIES_WITHOUT_PKGCONFIG))
ifneq (,$(LIBRARIES))
  CCFLAGS += `pkg-config --cflags $(LIBRARIES)`
  LDFLAGS += `pkg-config --libs $(LIBRARIES)`
endif
LDFLAGS += -Wl,--start-group $(PARTICLE_LIBRARIES) -Wl,--end-group


$(BINDIR)/%.bin: $(BINDIR)/%
	@echo "[OBJCPY]" $@
	$V$(OBJCOPY) -O binary $< $@

$(target-bindir)/%: $(target-objdir)/%.o $(patsubst %,$(target-objdir)/%,$(PARTICLE_LIBRARY_NAMES)) $(TARGET_NAME_FILE) | $(target-bindir)
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

$(target-objdir)/%.o: %.cpp Makefile | $(DIRS)
	@echo "[CXX]	" $<
	$V$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<
	$(call generate_depfile,$<,$@,$(CXXFLAGS))

$(target-objdir)/%.o: %.c Makefile | $(DIRS)
	@echo "[CC]	" $<
	$V$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<
	$(call generate_depfile,$<,$@,$(CFLAGS))

$(target-objdir)/%.o: %.S Makefile | $(DIRS)
	@echo "[AS]	" $<
	$V$(AS) $(ASFLAGS) -o $@ $<

.PHONY: $(DIRS)
$(DIRS):
	@mkdir -p $@

$(PARTICLE_MAKEFILES): Makefile
	@echo "[GEN]	" $@
	$(file >$@,$(call generate_subdir_makefile,$@))



# result: shell command to create a dependency file
# argument 1: input source file
# argument 2: output object file
# argument 3: extra cpp flags
define generate_depfile
	$V$(CPP) -MM -MP -MT $2 $(CPPFLAGS) $3 $1 > $2.d \
		&& $(SED) -e 's@^\(.*\)\.o:@\1.d \1.o:@' -i $2.d
endef

# result: shell commands to create a particle Makefile
# argument 1: directory
define generate_subdir_makefile
DIRS += $$(target-objdir)/$(1:/dir.mk=)

$(call submk_name,$1)_SRC := $(foreach ext,$(CODE_EXTS),$$(wildcard $(dir $1)*.$(ext)))

$(call submk_name,$1)_OBJ := $(foreach ext,$(CODE_EXTS),$$(patsubst %.$(ext),$$(target-objdir)/%.o,$$(filter %.$(ext),$$($(call submk_name,$1)_SRC))))

DEP_SRC += $$($(call submk_name,$1)_SRC)

$$(target-objdir)/$(call sublib_name,$1): $$($(call submk_name,$1)_OBJ)
	@echo "[AR]	" $$@
	$$V$$(AR) -rcs $$@ $$^
endef
