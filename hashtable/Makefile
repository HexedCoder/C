.PHONY: check debug clean all valgrind profile run indent man commit_count manual_check manual_valgrind

CFLAGS := -std=c18
CFLAGS += -Wall -Wextra -Wpedantic -Wwrite-strings
CFLAGS += -Waggregate-return -Wvla -Wfloat-equal
CFLAGS += -D_DEFAULT_SOURCE

VAL_FLAGS := --leak-check=full --show-leak-kinds=all --track-origins=yes
# VAL_FLAGS += --verbose -s

SRC_DIR := src
OBJ_DIR := obj
TST_DIR := test

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

BIN := hashtable
CHECK := $(BIN)_check

TSTS := $(wildcard $(TST_DIR)/*.c)
TST_OBJS += $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))
TST_OBJS := $(filter-out $(OBJ_DIR)/$(BIN).o, $(OBJS))
TST_OBJS := $(filter-out $(OBJ_DIR)/$(BIN)_driver.o, $(OBJS))
TST_OBJS += $(patsubst $(TST_DIR)/%.c, $(OBJ_DIR)/%.o, $(TSTS))
TST_LIBS := -lcheck -lm -pthread -lrt -lsubunit

all: $(BIN)

debug: CFLAGS += -g3 -D DDEBUG
debug: $(BIN)

check: $(CHECK)

clean: 
	@rm -rf $(OBJ_DIR) $(BIN) $(CHECK) gmon.out
	clear

profile: CFLAGS += -pg
profile: $(BIN)
	./$(BIN)

valgrind: CFLAGS += -D DDEBUG -g3
valgrind: $(BIN) 
	valgrind $(VAL_FLAGS) ./$(BIN) 


run: clean $(BIN)
	./$(BIN)

# runs indent -linux on all source files, cleans up  *.~ files afterwards
indent:
	indent -linux ./src/*.c ./src/*.h
	@rm ./src/*.c~ ./src/*.h~

# displays current project man page
man:
	man doc/./$(BIN).1

# displays git commits of current branch
commit_count:
	git shortlog -s

$(OBJ_DIR):
	@mkdir -p $@

$(OBJS): | $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ -lm

$(OBJ_DIR)/%.o: $(TST_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ -lm

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ -lm

$(CHECK): $(TST_OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(TST_LIBS)
	./$(CHECK)
