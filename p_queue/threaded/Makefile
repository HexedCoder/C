.PHONY: all check debug profile clean run indent

CFLAGS := -std=c18 -Wall -Wextra -Wpedantic -Waggregate-return
CFLAGS += -Wwrite-strings -Wvla -Wfloat-equal

VAL_FLAGS := -s --leak-check=full --show-leak-kinds=all --track-origins=yes

SRC_DIR := src
OBJ_DIR := obj

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

TST_OBJS := $(filter $(DRIVER).c, $(OBJS))

BIN := driver
DRIVER := $(BIN)_driver
CC:= gcc-9

TST_LIBS := -lcheck -lm -pthread -lrt -lsubunit

all: $(BIN)

debug: CFLAGS += -g3
debug: $(BIN)

check: $(DRIVER)

indent:
	indent -linux ./src/*.c ./src/*.h
	@rm ./src/*.c~ ./src/*.h~

run: $(BIN)
	./$(BIN)

clean:
	@rm -rf $(OBJ_DIR) $(BIN) $(DRIVER) gmon.out
	@clear

profile: CFLAGS += -g3 -pg
profile: $(BIN)

valgrind: CFLAGS += -g3
valgrind: $(BIN)
	valgrind $(VAL_FLAGS) ./$(BIN) test/test1

$(OBJ_DIR):
	@mkdir -p $@

$(OBJS): | $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(TST_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

$(DRIVER): $(DRIVER).c | $(BIN)
	$(CC) $(CFLAGS) $^ -o $@
	./$(DRIVER)