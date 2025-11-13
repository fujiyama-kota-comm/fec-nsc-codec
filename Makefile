CC = gcc
CFLAGS = -O2 -Wall -Iinclude
LIBS = -lm

SRCS = src/nsc_encoder.c src/nsc_decoder.c src/trellis.c

BIN = bin
TEST = $(BIN)/test_nsc

all: $(BIN) $(TEST)

$(BIN):
	mkdir -p $(BIN)

$(TEST): $(SRCS) examples/test_nsc.c
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

clean:
	rm -rf $(BIN)
