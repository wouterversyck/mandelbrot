CC := gcc
EXE := mandelbrot
DEBUG := debug
TEST := test
SRC_DIR := src
OBJ_DIR := obj

SRC_LS := $(shell ls -d src/*)
TEST_LS := $(shell ls -d tests/*)

SRC := $(wildcard $(SRC_DIR)/*)
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

CFLAGS := -Wall -Werror -Wextra
LDLIBS := -limago -lm -lpthread
DBFLAGS := -g

all: $(EXE)
debug: CFLAGS += $(DBFLAGS)
debug: $(EXE)

$(EXE): $(OBJ)
	$(CC) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

test: $(SRC_LS) $(TEST_LS)
	$(CC) $(DBFLAGS) -lcmocka -Wl,--wrap=printf,--wrap=localtime,--wrap=time src/ptime.c tests/ptime.c -o $(TEST)
	./test

.PHONY: clean
clean:
	@echo ''
	@echo '--------------------'
	@echo '- Cleaning project -'
	@echo '--------------------'
	@echo ''
	rm -f $(DEBUG) $(EXE) $(TEST)
	rm -rf obj/*