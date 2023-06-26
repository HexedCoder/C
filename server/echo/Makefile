.PHONY: all check debug clean man valgrind profile

CC := gcc-9
CFLAGS := -std=c18

CFLAGS += -Wall -Wextra -Wpedantic -Wwrite-strings
CFLAGS += -Waggregate-return -Wvla -Wfloat-equal
CFLAGS += -D_GNU_SOURCE

VAL_FLAGS := --leak-check=full --show-leak-kinds=all --track-origins=yes VAL_FLAGS += --trace-children=yes

SRC_DIR := src
OBJ_DIR := obj
TST_DIR := test

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

BIN := server
CHECK :=  $(BIN)_check

TSTS := $(wildcard $(TST_DIR)/*.c)
TST_OBJS := $(filter-out $(OBJ_DIR)/$(BIN).o, $(OBJS))
TST_OBJS += $(patsubst $(TST_DIR)/%.c, $(OBJ_DIR)/%.o, $(TSTS))
TST_LIBS := -lcheck -lm -pthread -lrt -lsubunit

all: $(BIN)

debug: CFLAGS += -g3 -D DDEBUG
debug: clean $(BIN)

man:
	man ./doc/$(BIN).1

clean:
	@rm -rf $(OBJ_DIR) $(BIN) $(CHECK) gmon.out
	clear

profile: CFLAGS += -pg
profile: clean $(BIN)

valgrind: CFLAGS += -D DDEBUG -g3
valgrind: clean $(BIN)

$(OBJ_DIR):
	@mkdir -p $@

$(OBJS): | $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@ -lm

$(OBJ_DIR)/%.o: $(TST_DIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@ -lm

$(BIN): $(OBJS)
	@$(CC) $(CFLAGS) $^ -o $@ -lm -lpthread

$(CHECK): $(TST_OBJS)
	@$(CC) $(CFLAGS) $^ -o $@ $(TST_LIBS)
	@./$(CHECK)
