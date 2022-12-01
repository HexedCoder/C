.PHONY: debug clean all

CFLAGS += -Wall -Wextra -Wpedantic -Wwrite-strings -Wvla -Winline -Wfloat-equal -Wstack-usage=1024

SRC_DIR := src
OBJ_DIR := obj

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))
BIN := driver

all: $(BIN)

debug: CFLAGS += -g
debug: $(BIN)

clean:
	@rm -rf $(OBJ_DIR)
	@rm -rf $(BIN)

$(OBJ_DIR) $(BIN_DIR):
	@mkdir -p $@

$(OBJS): | $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN): $(OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@
