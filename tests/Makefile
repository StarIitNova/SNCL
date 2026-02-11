# Compiler settings
CDIALECT = c99
CC       = gcc
CFLAGS   = -std=$(CDIALECT) -Wall -Wextra -O0 -g -I../include
CXX        = g++
CXXDIALECT = c++20
CXXFLAGS   = -std=$(CXXDIALECT) -Wall -Wextra -O0 -g -I../include

# Directories
BIN_DIR = bin

# Tests
TO_TEST = arraylist clioptions linkedlist
TO_TEST_CXX = youtube
TEST_EXECUTABLES = $(patsubst %,$(BIN_DIR)/test_%,$(TO_TEST))
TEST_EXECUTABLES_CXX = $(patsubst %,$(BIN_DIR)/testxx_%,$(TO_TEST_CXX))

.PHONY: all clean dirs run

all: dirs tests

dirs:
	@mkdir -p $(BIN_DIR)

tests: $(TEST_EXECUTABLES) $(TEST_EXECUTABLES_CXX)

$(BIN_DIR)/test_%: test_%.c ../source/sncl_%.c ../source/sncl_test.c
	$(CC) $(CFLAGS) $^ -o $@

$(BIN_DIR)/testxx_%: test_%.cpp ../source/sncl_%.c ../source/sncl_test.c
	$(CXX) $(CXXFLAGS) $^ -o $@

run: $(TEST_EXECUTABLES) $(TEST_EXECUTABLES_CXX)
	@echo
	@for exe in $^; do \
		echo "Running $$exe"; \
		./$$exe; \
	done

clean:
	rm -rf $(BIN_DIR)

-include $(DEPS)
