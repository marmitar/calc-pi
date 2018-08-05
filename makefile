CC = gcc
CFLAGS = -g
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
