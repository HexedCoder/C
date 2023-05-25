.PHONY: all check debug profile clean run indent

CFLAGS := -std=c18 -Wall -Wpedantic -Waggregate-return
CFLAGS += -Wwrite-strings -Wfloat-equal -D_DEFAULT_SOURCE

VAL_FLAGS := -s --leak-check=full --show-leak-kinds=all --track-origins=yes

SRC_DIR := src
OBJ_DIR := obj
TST_DIR := test

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))
OBJS := $(filter-out $(OBJ_DIR)/$(BIN)_check.o, $(OBJS))

BIN := driver
CHECK := $(BIN)_check
CC:= gcc-9

TSTS := $(wildcard $(TST_DIR)/*.c)
TST_OBJS := $(filter-out $(OBJ_DIR)/llist_$(BIN).o, $(OBJS))
TST_OBJS += $(patsubst $(TST_DIR)/%.c, $(OBJ_DIR)/%.o, $(TSTS))
TST_LIBS := -lcheck -lm -pthread -lrt -lsubunit

all: $(BIN)

debug: CFLAGS += -g3
debug: $(BIN)

run: $(BIN)
	./$(BIN)

check: $(CHECK)

indent:
	indent -linux ./*.c ./*.h
	@rm ./*.c~ ./*.h~

clean:
	@rm -rf $(OBJ_DIR) $(BIN) $(CHECK) gmon.out
	clear

profile: CFLAGS += -g3 -pg
profile: $(BIN)

valgrind: CFLAGS += -g3
valgrind: $(BIN)
	valgrind $(VAL_FLAGS) ./$(BIN)

$(OBJ_DIR):
	@mkdir -p $@

$(OBJS): | $(OBJ_DIR)
$(TST_OBJS): | $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo $<
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(TST_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

OBJS := $(filter-out $(OBJ_DIR)/$(BIN)_test.o, $(OBJS))

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

$(CHECK): CFLAGS += -g3
$(CHECK): $(TST_OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(TST_LIBS)
	valgrind $(VAL_FLAGS) ./$(CHECK)

print:
#	$(info $$CFLAGS is [${CFLAGS}])
	$(info $$SRCS is [${SRCS}])
	$(info $$OBJS is [${OBJS}])
	$(info $$TST_OBJS is [${TST_OBJS}])