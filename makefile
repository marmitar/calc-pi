CC = gcc
CFLAGS = -march=znver1 -O3 -pipe -fstack-protector-strong -fno-plt
CLIBS = -lmpfr -lgmp -lm -lpthread

TARGET = pi
SRC = $(wildcard *.c)
OBJ = $(patsubst %.c,%.o,$(SRC))

all: $(TARGET)
	rm -f *.o

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(CLIBS)

%.o: %.c %.h
	$(CC) $(CFLAGS) $(CLIBS) -c $< -o $@

clean:
	rm -f $(TARGET) *.o
DEBUG_CFLAGS="-g -fvar-tracking-assignments"
