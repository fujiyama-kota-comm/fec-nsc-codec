CC = gcc
CFLAGS = -O2 -Wall -Iinclude

SRC = src/nsc_encoder.c \
      src/nsc_decoder.c \
      src/trellis.c

all: test_nsc

test_nsc: $(SRC) examples/test_nsc.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f test_nsc
