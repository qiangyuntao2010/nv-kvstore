include ../IncludePre.mk

CC = g++
SRCS = test_nv_idx.c test_nv_data.c test_common.c test_nv_entry.c kvstore_status.c get_by_key.c get_by_index.c cpu_check.c clear_nvdimm.c latency_test.c workload_test.c put.c
LIBS += -pthread -lgtest -lgtest_main -lnvdimm
PROJ_ROOT = ..
 
all: $(TARGET)
 
$(TARGET): $(OBJS)
	@echo "Buliding $@"
	@`[ -d $(TARGET_DIR) ] || $(MKDIR) $(TARGET_DIR)`
	$(CC) $(CFLAGS) -o $@ $(@:bin/%=$(OBJS_DIR)/%.o) $(LIB_DIRS) $(LIBS)
 
ifneq ($(MAKECMDGOALS), clean)
ifneq ($(MAKECMDGOALS), depend)
ifneq ($(strip $(SRCS)),)
-include $(DEP_FILE)
endif
endif
endif

