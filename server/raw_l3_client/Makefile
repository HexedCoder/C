.PHONY: debug clean all valgrind profile

CFLAGS := -std=c18
CFLAGS += -Wall -Wextra -Wpedantic -Wwrite-strings
CFLAGS += -Waggregate-return -Wvla -Wfloat-equal
CFLAGS += -D_DEFAULT_SOURCE

VAL_FLAGS := --leak-check=full --show-leak-kinds=all --track-origins=yes

SRC_DIR := .
OBJ_DIR := obj
TST_DIR := test

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

BIN := raw_client
CHECK := $(BIN)_check
CC:= gcc-9

TSTS := $(wildcard $(TST_DIR)/*.c)
TST_OBJS += $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))
TST_OBJS := $(filter-out $(OBJ_DIR)/$(BIN).o, $(OBJS))
TST_OBJS := $(filter-out $(OBJ_DIR)/$(BIN)_driver.o, $(OBJS))
TST_OBJS += $(patsubst $(TST_DIR)/%.c, $(OBJ_DIR)/%.o, $(TSTS))
TST_LIBS := -lcheck -lm -pthread -lrt -lsubunit

all: $(BIN)

run: clean $(BIN)
	sudo ./raw_client 0.0.0.0 23232 localhost 5000

debug: CFLAGS += -g3 -D DDEBUG
debug: $(BIN)

check: $(CHECK)

clean: 
	@rm -rf $(OBJ_DIR) $(BIN) $(CHECK) gmon.out
	clear

profile: CFLAGS += -pg
profile: $(BIN)

valgrind: CFLAGS += -D DDEBUG -g3
valgrind: $(BIN)

$(OBJ_DIR):
	@mkdir -p $@

$(OBJS): | $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@ -lm

$(OBJ_DIR)/%.o: $(TST_DIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@ -lm

$(BIN): $(OBJS)
	@$(CC) $(CFLAGS) $^ -o $@ -lm
