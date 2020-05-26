CC := gcc
EXE := mandelbrot
DEBUG := debug
SRC_DIR := src
OBJ_DIR := obj

SRC := $(wildcard $(SRC_DIR)/*)
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

CFLAGS := # -Wall -Werror -Wextra
LDLIBS := -limago -lm -lpthread
DBFLAGS := -g

all: $(EXE)
debug: CFLAGS += $(DBFLAGS)
debug: $(EXE)

$(EXE): $(OBJ)
	$(CC) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	@echo ''
	@echo '--------------------'
	@echo '- Cleaning project -'
	@echo '--------------------'
	@echo ''
	rm -f $(DEBUG) $(EXE)
	rm -rf obj/*