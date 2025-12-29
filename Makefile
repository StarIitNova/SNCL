-include config.mk

# Compiler settings
CDIALECT = c99
CC       = gcc
AR       = /usr/bin/ar
CFLAGS   = -std=$(CDIALECT) -Wall -Wextra -O0 -g -Iinclude

# Input/output folders
SOURCE_DIR = source
BIN_DIR    = bin

# Source files
SOURCE_FILES = 

ifeq ($(CONFIG_C_LEXER),y)
SOURCE_FILES += source/sncl_clex.c
endif

ifeq ($(CONFIG_ARRAYLIST),y)
SOURCE_FILES += source/sncl_arraylist.c
endif

ifeq ($(CONFIG_CLI_OPTS),y)
SOURCE_FILES += source/sncl_clioptions.c
endif

OBJ_FILES    = $(patsubst $(SOURCE_DIR)/%.c,$(BIN_DIR)/%.o,$(SOURCE_FILES))
DEPS         = $(patsubst $(SOURCE_DIR)/%.c,$(BIN_DIR)/%.d,$(SOURCE_FILES))

.PHONY: all clean dirs menuconfig

all: dirs libsncl.a

dirs:
	@mkdir -p $(BIN_DIR)

libsncl.a: $(OBJ_FILES)
	$(AR) rcs libsncl.a $(OBJ_FILES)

$(BIN_DIR)/%.o: $(SOURCE_DIR)/%.c
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

clean:
	rm -rf $(BIN_DIR) libsncl.a

-include $(DEPS)
