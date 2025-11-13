CC      = gcc
CFLAGS  = -O2 -Wall -std=c99 -Iinclude
LDFLAGS = -lm

# NSC library sources
SRC = \
    src/nsc_encoder.c \
    src/nsc_decoder.c \
    src/trellis.c

OBJ = $(SRC:.c=.o)

# Test program
TEST_SRC = examples/test_nsc.c
TEST_OBJ = $(TEST_SRC:.c=.o)

BIN_DIR = bin
TARGET = $(BIN_DIR)/test_nsc

all: $(BIN_DIR) $(TARGET)

# Windows + Linux 両対応 mkdir（Windows優先）
$(BIN_DIR):
	@if not exist $(BIN_DIR) mkdir $(BIN_DIR)

$(TARGET): $(OBJ) $(TEST_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run:
	./$(TARGET)

clean:
	rm -f $(OBJ) $(TEST_OBJ) $(TARGET)

.PHONY: all clean run
