include ../IncludePre.mk

SRCS = common.c nv_idx.c nv_data.c nv_mm.c nv_map.c nv_backend_fs.c nv_backend_nvdimm.c nv_gc.c

PROJ_ROOT = ..
CC = g++
LIBS = -lpthread
LIB_NAME = NVKVStore
LIB_FULL_NAME = ../lib/lib$(LIB_NAME).a

all: $(LIB_FULL_NAME)

$(LIB_FULL_NAME) : $(OBJS)
	$(AR) rcv $@ $(OBJS)
	$(RANLIB) $@

ifneq ($(MAKECMDGOALS), clean)
ifneq ($(MAKECMDGOALS), depend)
ifneq ($(strip $(SRCS)),)
-include $(DEP_FILE)
endif
endif
endif

