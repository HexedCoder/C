.PHONY: all check debug profile clean run

CFLAGS := -std=c18 -Wall -Wpedantic -Waggregate-return
CFLAGS += -Wwrite-strings -Wfloat-equal -D_DEFAULT_SOURCE

VAL_FLAGS := -s --leak-check=full --show-leak-kinds=all --track-origins=yes

SRC_DIR := src
OBJ_DIR := obj
TST_DIR := test

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

BIN := driver
CHECK := $(BIN)_check
CC:= gcc-9

TSTS := $(wildcard $(TST_DIR)/*.c)
TST_OBJS := $(filter-out $(OBJ_DIR)/$(BIN).o, $(OBJS))
TST_OBJS += $(patsubst $(TST_DIR)/%.c, $(OBJ_DIR)/%.o, $(TSTS))
TST_LIBS := -lcheck -lm -pthread -lrt -lsubunit

all: $(BIN)

debug: CFLAGS += -g3
debug: $(BIN)

run: $(BIN)
	./$(BIN) file.txt


check: $(CHECK)

clean:
	@rm -rf $(OBJ_DIR) $(BIN) $(CHECK) $(OUTPUT_FILE)
	clear

profile: CFLAGS += -g3 -pg
profile: $(BIN)

valgrind: CFLAGS += -g3
valgrind: $(BIN)
	valgrind $(VAL_FLAGS) ./$(BIN)	file.txt

$(OBJ_DIR):
	@mkdir -p $@

$(OBJS): | $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(TST_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

$(CHECK): $(TST_OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(TST_LIBS)
	./$(CHECK) file.txt