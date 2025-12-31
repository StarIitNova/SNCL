# Compiler settings
CDIALECT = c99
CC       = gcc
CFLAGS   = -std=$(CDIALECT) -Wall -Wextra -O0 -g -I../include

# Directories
BIN_DIR = bin

# Tests
TO_TEST = arraylist clioptions linkedlist
TEST_EXECUTABLES = $(patsubst %,$(BIN_DIR)/test_%,$(TO_TEST))

.PHONY: all clean dirs run

all: dirs tests

dirs:
	@mkdir -p $(BIN_DIR)

tests: $(TEST_EXECUTABLES)

$(BIN_DIR)/test_%: test_%.c ../source/sncl_%.c ../source/sncl_test.c
	$(CC) $(CFLAGS) $^ -o $@

run: $(TEST_EXECUTABLES)
	@echo
	@for exe in $^; do \
		echo "Running $$exe"; \
		./$$exe; \
	done

clean:
	rm -rf $(BIN_DIR)

-include $(DEPS)
