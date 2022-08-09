CC = gcc
CFLAGS = #-Wall -Wextra
DEPS = common.h simple.h
OBJ = common.o simple.o tictactoe.c

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

tictactoe: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f tictactoe *.o