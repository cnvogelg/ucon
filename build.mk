# Makefile to cross-compile ucon-handler

FLAVOR?=_dbg
BUILD_DIR=BUILD
OBJ_DIR=$(BUILD_DIR)/obj$(FLAVOR)

# parse flavors
ifeq "_dbg" "$(findstring _dbg,$(FLAVOR))"
DEBUG=1
DEFINES+=DEBUG
DEBUGFLAGS=-g
else
OPTFLAGS=-O2
endif

# setup defines
CDEFINES=$(patsubst %,-D%=1,$(DEFINES))
include ../version.mk
QUOTE=\\\"
CDEFINES+=-DMY_VERSION=$(PROJECT_MAJOR)
CDEFINES+=-DMY_REVISION=$(PROJECT_MINOR)
CDEFINES+=-DMY_VERSION_STR=$(QUOTE)$(PROJECT_MAJOR).$(PROJECT_MINOR)$(QUOTE)
CDEFINES+=-DMY_DATE=$(QUOTE)$(PROJECT_DATE)$(QUOTE)
CDEFINES+=-DMY_NAME=$(QUOTE)$(PRJ_NAME)$(QUOTE)

# toolchain dir
# uses default dir of https://github.com/cahirwpz/amigaos-cross-toolchain
TC_DIR=/opt/m68k-amigaos

# cross compiler/assembler setup
CC=vc
CFLAGS=-c99 -sc $(CDEFINES) $(OPTFLAGS) $(DEBUGFLAGS)
CFLAGS+=-I../ucon_lib/include -I../common
LDFLAGS=-nostdlib -lvcs -lamiga $(DEBUGFLAGS)
AS=vasmm68k_mot
ASFLAGS=-quiet -Fhunk -I$(TC_DIR)/os-include -m68000


ifeq "$(DEBUG)" "1"
HDRS+=debug.h
SRCS+=debug.c
endif
HDRS+=compiler.h
ASRCS+=sysbase.s

VPATH+=../common

ifneq "$(VERBOSE)" "1"
V=@
endif

# end must be last
#ASRCS+=end.s

OBJS=$(patsubst %.c,$(OBJ_DIR)/%.o,$(SRCS))
OBJS+=$(patsubst %.s,$(OBJ_DIR)/%.o,$(ASRCS))

OUTPUT=$(BUILD_DIR)/$(PRJ_NAME)$(FLAVOR)

# ----- rules -----

all: $(BUILD_DIR) $(OBJ_DIR) $(OUTPUT)

release:
	$(MAKE) FLAVOR=_rel

flavors:
	@for f in $(FLAVORS) ; do \
		$(MAKE) FLAVOR=$$f || exit 1 ; \
	done
	@echo "---- done flavors: $(FLAVORS) -----"

$(OUTPUT): $(OBJS)
	@echo "  LD  $@"
	$(V)vc $^ -o $@ $(LDFLAGS)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJS): $(SRCS) $(HDRS) $(ASRCS)

$(OBJ_DIR)/%.o: %.c
	@echo "  CC  $<"
	$(V)$(CC) $(CFLAGS) -c -o $@ $<

$(OBJ_DIR)/%.o: %.s
	@echo "  AS  $<"
	$(V)$(AS) $(ASFLAGS) -o $@ $<

clean:
	rm -rf $(BUILD_DIR)

dist:
	@for f in $(FLAVORS) ; do \
		$(MAKE) dist_flavor FLAVOR=$$f || exit 1 ; \
	done

dist_flavor: all
	cp $(OUTPUT) $(DIST_DIR)/$(DIST_SUBDIR)
