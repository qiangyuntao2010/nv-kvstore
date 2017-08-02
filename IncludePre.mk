CC = gcc
AR = ar
RANLIB = ranlib
RM = rm
MKDIR = mkdir

LIBS = -lNVKVStore 
LIB_DIRS = -L$(PROJ_ROOT)/lib 
OBJS_DIR = obj
TARGET_DIR = bin
OBJS = $(SRCS:%.c=$(OBJS_DIR)/%.o)
TARGET = $(SRCS:%.c=$(TARGET_DIR)/%)
CFLAGS = -I $(PROJ_ROOT)/src/include -g
#CFLAGS = -I $(PROJ_ROOT)/src/include -g -pg
DEP_FILE = $(OBJS_DIR)/dependent_file

$(OBJS_DIR)/%.o: %.c
	@echo "Compiling $@ "
	@`[ -d $(OBJS_DIR) ] || $(MKDIR) $(OBJS_DIR)`
	$(CC) $(CFLAGS) -c $< -o $@

all: $(OBJS)

clean:
	$(RM) -f $(TARGET_DIR)/* $(OBJS_DIR)/*

depend :
	@`[ -d $(OBJS_DIR) ] || $(MKDIR) $(OBJS_DIR)`
	@$(RM) -f $(DEP_FILE)
	@for FILE in $(SRCS:%.c=%); do \
		$(CC) $(CFLAGS) -MM -MT $(OBJS_DIR)/$$FILE.o $$FILE.c >> $(DEP_FILE); \
	done

