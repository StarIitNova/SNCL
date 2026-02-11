-include config.mk

# Compiler settings
CDIALECT = c99
CC       = gcc
AR       = /usr/bin/ar
CFLAGS   = -std=$(CDIALECT) -Wall -Wextra -O3 -Iinclude

# Input/output folders
SOURCE_DIR = source
BIN_DIR    = bin

# Source files
SOURCE_FILES = 

USE_CXX=n

ifeq ($(CONFIG_C_LEXER),y)
SOURCE_FILES += source/sncl_clex.c
endif

ifeq ($(CONFIG_ARRAYLIST),y)
SOURCE_FILES += source/sncl_arraylist.c
endif

ifeq ($(CONFIG_CLI_OPTS),y)
SOURCE_FILES += source/sncl_clioptions.c
endif

ifeq ($(CONFIG_LINKEDLIST),y)
SOURCE_FILES += source/sncl_linkedlist.c
endif

ifeq ($(CONFIG_YOUTUBE_TOOLS),y)
SOURCE_FILES += source/sncl_youtube.cpp
endif

OBJ_FILES     = $(patsubst $(SOURCE_DIR)/%.c,$(BIN_DIR)/%.o,$(SOURCE_FILES))
OBJ_FILES_CXX = $(patsubst $(SOURCE_DIR)/%.cpp,$(BIN_DIR)/%.o,$(SOURCE_FILES))
DEPS          = $(patsubst $(SOURCE_DIR)/%.c,$(BIN_DIR)/%.d,$(SOURCE_FILES))
DEPS_CXX      = $(patsubst $(SOURCE_DIR)/%.cpp,$(BIN_DIR)/%.d,$(SOURCE_FILES))

.PHONY: all clean dirs menuconfig tests

all: dirs libsncl.a

dirs:
	@mkdir -p $(BIN_DIR)

libsncl.a: $(OBJ_FILES) $(OBJ_FILES_CXX) config.mk Makefile
	$(AR) rcs libsncl.a $(OBJ_FILES) $(OBJ_FILES_CXX)

$(BIN_DIR)/%.o: $(SOURCE_DIR)/%.c
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

$(BIN_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -MMD -MP -o $@

clean:
	rm -rf $(BIN_DIR) libsncl.a

test:
	@make -C tests
	@make -C tests run

-include $(DEPS) $(DEPS_CXX)
