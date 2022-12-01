.PHONY: all check debug profile clean valgrind start
EXE:= driver
CFLAGS:= -Wall -Wextra -Wpedantic -Waggregate-return -Wwrite-strings -Wvla
CFLAGS+= -Wfloat-equal -Winline -std=c18
CC:= gcc
DEBUG:= gdb
OBJS:= 32_bit_random.c count_bit_flips.c print_binary_with_seps.c

all: $(EXE)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(EXE): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f *.o $(EXE)

debug: CFLAGS += -g
debug: all

profile: CFLAGS += -g -pg
profile: all

#check: LDLIBS+= -lcheck
check: $(EXE)
#	./$^ -s
#	./$^ $(VAR) -s
	./$^

start: all
	@./$(EXE) $(VAR) -s

valgrind: all
	valgrind --leak-check=full --show-leak-kinds=all ./$(EXE) $(VAR)