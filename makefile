EXE := mandelbrot
SRC_DIR := src
OBJ_DIR := obj

SRC := $(wildcard $(SRC_DIR)/*)
OBJ := $(SRC:$(SRC_DIR)/%.[ch]=$(OBJ_DIR)/%.o)

CFLAGS := #-Wall -Werror -Wextra
LDLIBS := -limago -lm

ll: $(EXE)
$(EXE): $(OBJ)
	$(CC) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.[ch]
	$(CC) $(CFLAGS) -c $< -o $@
