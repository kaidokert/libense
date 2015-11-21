ifeq ($(origin CONFIG_LIBC_OPT_S),file)
CCFLAGS += -Os
endif
