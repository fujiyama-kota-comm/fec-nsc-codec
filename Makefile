CC      = gcc
CFLAGS  = -O2 -Wall -std=c99 -Iinclude
LDFLAGS = -lm

SRC = \
    src/nsc_encoder.c \
    src/nsc_decoder.c \
    src/trellis.c

OBJ = $(SRC:.c=.o)

TEST_SRC = examples/test_nsc.c
TEST_OBJ = $(TEST_SRC:.c=.o)

BIN_DIR = bin
TARGET  = $(BIN_DIR)/test_nsc

all: $(TARGET)

# Create bin directory (auto-detect platform)
$(BIN_DIR):
	@if [ ! -d "$(BIN_DIR)" ]; then \
		mkdir -p $(BIN_DIR) 2>/dev/null || mkdir $(BIN_DIR); \
	fi

$(TARGET): $(BIN_DIR) $(OBJ) $(TEST_OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(TEST_OBJ) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJ) $(TEST_OBJ) $(TARGET)

.PHONY: all clean run
