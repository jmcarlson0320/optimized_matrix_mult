CC = gcc
CFLAGS = -g -Wall
EXE = mat_mult

$(EXE): main.o
	$(CC) -o $@ $^

clean:
	rm *.o $(EXE)
