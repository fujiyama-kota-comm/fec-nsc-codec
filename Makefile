CC = gcc
CFLAGS = -O2 -Wall -std=c99 -Iinclude
LDFLAGS = -lm

SRC = src/nsc_encoder.c src/nsc_decoder.c src/trellis.c
OBJ = $(SRC:.c=.o)

TEST_SRC = examples/test_nsc.c
TEST_OBJ = $(TEST_SRC:.c=.o)

TARGET = test_nsc

all: $(TARGET)

$(TARGET): $(OBJ) $(TEST_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run:
	./$(TARGET)

clean:
	rm -f $(OBJ) $(TEST_OBJ) $(TARGET)

.PHONY: all clean run
