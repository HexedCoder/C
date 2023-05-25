.PHONY: check clean

CFLAGS := -std=c18
CFLAGS += -Wall -Wextra -Wpedantic -Wwrite-strings
CFLAGS += -Waggregate-return -Wvla -Wfloat-equal -pthread
CFLAGS += -D_DEFAULT_SOURCE

CHECK := $(BIN)_check
CC:= gcc-9

all:
	$(CC) $(CFLAGS) server_driver.c -o serve
	$(CC) $(CFLAGS) client_driver.c -o client

server: clean
	@$(CC) $(CFLAGS) server_driver.c -o serve
	@./serve

client: clean
	@$(CC) $(CFLAGS) client_driver.c -o client ||:
	@./client

clean:
	@rm -rf serve client

$(OBJ_DIR):
	@mkdir -p $@

$(OBJS): | $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@ -lm

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ -lm
