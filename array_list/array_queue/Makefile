.PHONY: all check debug profile clean run indent

CFLAGS := -std=c18 -Wall -Wpedantic -Waggregate-return
CFLAGS += -Wwrite-strings -Wfloat-equal -D_DEFAULT_SOURCE

LDLIBS := -lm

VAL_FLAGS := -s --leak-check=full --show-leak-kinds=all --track-origins=yes

SRCS := $(wildcard *.c)
OBJS := $(patsubst %.c, %.o, $(SRCS))

BIN := driver
CC:= gcc

all: $(BIN)
	./$(BIN)

debug: CFLAGS += -g3
debug: $(BIN)

run: $(BIN)
	./$(BIN)

check: $(CHECK)

indent:
	indent -linux ./*.c ./*.h
	@rm ./*.c~ ./*.h~

clean:
	@rm -rf $(BIN) $(OBJS)
	clear

profile: CFLAGS += -g3 -pg
profile: $(BIN)

valgrind: CFLAGS += -g3
valgrind: $(BIN)
	valgrind $(VAL_FLAGS) ./$(BIN)

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

$(CHECK): $(TST_OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)
	./$(BIN)