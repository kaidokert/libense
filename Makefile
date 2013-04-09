# main targets and particle libraries
TARGETS := main
PARTICLES := ense

# default compiler/linker flags
override CPPFLAGS += -I include

ifdef RELEASE
CCFLAGS += -Oz -emit-llvm
LDFLAGS += -plugin /usr/lib/llvm/LLVMgold.so
else
CCFLAGS += -g
ASFLAGS += -g
endif

CCFLAGS += -Wall -Wextra -Werror -pedantic
CCFLAGS += -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -ffreestanding -nostdlib -nostdinc
CCFLAGS += -fno-exceptions -fno-rtti -fno-stack-protector
#CCFLAGS += -ffunction-sections -fdata-sections
CFLAGS += $(CCFLAGS) -std=c99
CXXFLAGS += $(CCFLAGS) -std=c++11

LDFLAGS += -L/usr/lib/gcc/arm-elf/4.7.2/thumb/ -lgcc -T f4.ld

# default values for internal variables
AS := arm-elf-as
CC := clang -target arm-elf-eabi -integrated-as
CXX := clang++ -target arm-elf-eabi -integrated-as
CPP := $(CXX) -E
LD := arm-elf-ld.gold
SED := sed
FIND := find
DOXYGEN := doxygen

OBJCOPY := arm-elf-objcopy

OBJDIR = obj
BINDIR = bin

# extensions of files considered to be code
# supported so far:
# C++: cpp
# C: c
# Assembler: S
override CODE_EXTS := c cpp S

# external libraries used
override LIBRARIES :=
override LIBRARIES_WITHOUT_PKGCONFIG :=

# library version requirements
define LIBRARY_VERSION_CHECK =
endef



###
# here be internals
###

ifdef V
override V :=
else
V := @
endif

# result: cleaned name of particle
# argument 1: makefile name or directory name of particle root
define submk_name =
$(subst /,_,$(subst _,__,$(1:/dir.mk=)))
endef

# result: particles library filename
# argument 1: makefile name or directory name of particle root
define sublib_name =
lib$(call submk_name,$1).a
endef

# result: shell code to check for existence of a certain system header. generates a return code.
# argument 1: name of a header file required by the program
define require_header =
(echo '#include <$1>' | $(CPP) - >/dev/null 2>&1)
endef

# result: shell code to check for existence of a certain symbol. generates a return code.
# argument 1: name of a header file containing the symbol
# argument 2: name of the symbol
define require_symbol_definition =
((echo '#include <$1>'; echo '#ifndef $2'; echo '#error'; echo '#endif') | $(CPP) - >/dev/null 2>&1)
endef

ifndef MAKE_RESTARTS
$(shell touch -r Makefile -d yesterday .depend-check)
include .depend-check
else
$(shell $(RM) .depend-check)
DEPEND_CHECK_DONE := 1
endif



SRC := $(foreach ext,$(CODE_EXTS),$(wildcard *.$(ext)))

DEP_SRC := $(SRC)

DIRS := $(BINDIR) $(OBJDIR) $(subst ./,,$(sort $(patsubst %,$(OBJDIR)/%,$(dir $(DEP_SRC)))))

PARTICLE_MAKEFILES := $(patsubst %,%/dir.mk,$(PARTICLES))

TARGET_EXECUTABLES := $(patsubst %,$(BINDIR)/%,$(TARGETS))

all: $(TARGET_EXECUTABLES) $(patsubst %,%.bin,$(TARGET_EXECUTABLES))

.depend-check: Makefile
	@$(LIBRARY_VERSION_CHECK)
	@touch $@

deps:

ifdef DEPEND_CHECK_DONE
-include $(PARTICLE_MAKEFILES)
-include $(foreach ext,$(CODE_EXTS),$(patsubst %.$(ext),$(OBJDIR)/%.o.d,$(filter %.$(ext),$(DEP_SRC))))
endif

PARTICLE_LIBRARY_NAMES := $(foreach lib,$(PARTICLES),$(call sublib_name,$(lib)))
PARTICLE_LIBRARIES := $(foreach lib,$(PARTICLES),-l$(call submk_name,$(lib)))

override LDFLAGS += -L $(OBJDIR)
ifneq (,$(LIBRARIES))
override CXXFLAGS += `pkg-config --cflags $(LIBRARIES)`
override LDFLAGS += `pkg-config --libs $(LIBRARIES)`
endif
override LDFLAGS += $(patsubst %,-l%,$(LIBRARIES_WITHOUT_PKGCONFIG)) --start-group $(PARTICLE_LIBRARIES) --end-group


$(BINDIR)/%.bin: $(BINDIR)/%
	@echo -e "[OBJCPY]" $@
	$V$(OBJCOPY) -O binary $< $@

$(BINDIR)/%: $(OBJDIR)/%.o $(patsubst %,$(OBJDIR)/%,$(PARTICLE_LIBRARY_NAMES)) | $(BINDIR)
	@echo -e "[LD]\t" $@
	$V$(LD) -o $@ $< $(LDFLAGS)

.SECONDARY: $(patsubst %,$(OBJDIR)/%.o,$(TARGETS))

clean:
	-$(RM) -r $(OBJDIR) $(BINDIR) $(PARTICLE_MAKEFILES)
   
depclean:
	-$(FIND) $(OBJDIR) -iname '*.d' -delete

distclean:
	-$(RM) -r $(BINDIR)

$(OBJDIR)/%.o: %.cpp Makefile | $(DIRS)
	@echo -e "[CXX]\t" $<
	$V$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<
	$(call generate_depfile,$<,$@,$(CXXFLAGS))

$(OBJDIR)/%.o: %.c Makefile | $(DIRS)
	@echo -e "[CC]\t" $<
	$V$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<
	$(call generate_depfile,$<,$@,$(CFLAGS))

$(OBJDIR)/%.o: %.S Makefile | $(DIRS)
	@echo -e "[AS]\t" $<
	$V$(AS) $(ASFLAGS) -o $@ $<

$(DIRS):
	@mkdir -p $@

$(PARTICLE_MAKEFILES): Makefile
	@echo -e "[GEN]\t" $@
	$(call generate_subdir_makefile,$@)

html-doc:
	$V$(DOXYGEN) doc/doxy.cfg

.PHONY: clean distclean depclean $(DIRS) deps all html-doc


# result: shell command to create a dependency file
# argument 1: input source file
# argument 2: output object file
# argument 3: extra cpp flags
define generate_depfile =
	$V$(CPP) -MM -MP -MT $2 $(CPPFLAGS) $3 $1 > $2.d \
		&& $(SED) -e 's@^\(.*\)\.o:@\1.d \1.o:@' -i $2.d
endef

# result: shell commands to create a particle Makefile
# argument 1: directory
define generate_subdir_makefile =
	@echo 'DIRS += $$(OBJDIR)/$(1:/dir.mk=)' > $1 
	@echo >> $1
	@echo '$(call submk_name,$1)_SRC := $(foreach ext,$(CODE_EXTS),$$(wildcard $(dir $1)*.$(ext)))' >> $1
	@echo >> $1
	@echo '$(call submk_name,$1)_OBJ := $(foreach ext,$(CODE_EXTS),$$(patsubst %.$(ext),$$(OBJDIR)/%.o,$$(filter %.$(ext),$$($(call submk_name,$1)_SRC))))' >> $1
	@echo >> $1
	@echo 'DEP_SRC += $$($(call submk_name,$1)_SRC)' >> $1
	@echo >> $1
	@echo '$$(OBJDIR)/$(call sublib_name,$1): $$($(call submk_name,$1)_OBJ)' >> $1
	@echo '	@echo -e "[AR]\t" $$@' >> $1
	@echo '	$$V$$(AR) -rcs $$@ $$^' >> $1
endef
