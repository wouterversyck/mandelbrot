CC := gcc
EXE := mandelbrot
DEBUG := debug
TEST := test
SRC_DIR := src
TEST_DIR := tests
OBJ_DIR := obj

SRC_LS := $(shell ls -d src/*)
TEST_LS := $(shell ls -d tests/*)

SRC := $(wildcard $(SRC_DIR)/*)
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

CFLAGS := -Wall -Werror -Wextra -pedantic
LDLIBS := -limago -lm -lpthread
DBFLAGS := -g

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

debug: $(SRC_LS)
	$(CC) $^ $(DBFLAGS) $(CFLAGS) $(LDLIBS) -o debug

.PHONY: valgrind
valgrind: $(EXE)
	valgrind --leak-check=full --show-leak-kinds=all ./$(EXE)

.PHONY: clean
clean:
	@echo ''
	@echo '--------------------'
	@echo '- Cleaning project -'
	@echo '--------------------'
	@echo ''
	rm -f $(DEBUG) $(EXE) $(TEST)
	rm -rf obj/*

include tests/makefile