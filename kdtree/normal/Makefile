.PHONY: all check debug profile clean run indent

CFLAGS := -Wall -Wextra -Wpedantic -Waggregate-return
CFLAGS += -Wwrite-strings -Wvla -Wfloat-equal

VAL_FLAGS := -s --leak-check=full --show-leak-kinds=all --track-origins=yes

SRC_DIR := src
OBJ_DIR := obj
TST_DIR := test

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

BIN := kdtree
CHECK := $(BIN)_check
CC:= gcc-9

TSTS := $(wildcard $(TST_DIR)/*.c)
TST_OBJS := $(filter-out $(OBJ_DIR)/$(BIN).o, $(OBJS))
TST_OBJS += $(patsubst $(TST_DIR)/%.c, $(OBJ_DIR)/%.o, $(TSTS))
TST_LIBS := -lcheck -lm -pthread -lrt -lsubunit

all: $(BIN)

debug: CFLAGS += -g3
debug: $(BIN)

check: $(CHECK)

indent:
	indent -linux ./src/*.c ./src/*.h
	@rm ./src/*.c~ ./src/*.h~

run: $(BIN)
	./$(BIN) -x 18.334138 -y -66.066365 -k 3

clean:
	@rm -rf $(OBJ_DIR) $(BIN) $(DRIVER) gmon.out
	@clear

profile: CFLAGS += -g3 -pg
profile: $(BIN)

valgrind: CFLAGS += -g3
valgrind: $(BIN)
	valgrind $(VAL_FLAGS) ./$(BIN) -x 18.334138 -y -66.066365 -k 3

$(OBJ_DIR):
	@mkdir -p $@

$(OBJS): | $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(TST_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ -lm

$(DRIVER): $(DRIVER).c | $(BIN)
	$(CC) $(CFLAGS) $^ -o $@

$(CHECK): $(TST_OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(TST_LIBS)
	./$(CHECK)