.PHONY: all

SRCS := $(wildcard *.c)
OBJS := $(patsubst %.c, %.o, $(SRCS))

BIN := driver
CC:= gcc-9

all: $(BIN)
	./$(BIN)
	@rm $(OBJS) $(BIN)

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)
