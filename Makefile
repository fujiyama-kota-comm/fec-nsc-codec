CC = gcc
CFLAGS = -O2 -Wall -Iinclude

SRC = src/nsc_encoder.c src/nsc_decoder.c src/trellis.c examples/test_nsc.c
OBJ = $(SRC:.c=.o)

BIN_DIR = bin
TARGET = $(BIN_DIR)/test_nsc

# ------------------------------------------------------------
# Build rules
# ------------------------------------------------------------
all: $(TARGET)

$(BIN_DIR):
	@if not exist $(BIN_DIR) mkdir $(BIN_DIR)

# Link → 必ず bin/test_nsc に出力する
$(TARGET): $(OBJ) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

# Compile .c → .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# ------------------------------------------------------------
# Clean
# ------------------------------------------------------------
clean:
	del /Q src\*.o 2>nul || true
	del /Q examples\*.o 2>nul || true
	del /Q include\*.o 2>nul || true
	del /Q $(BIN_DIR)\*.exe 2>nul || true

.PHONY: all clean
