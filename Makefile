CC = gcc
CFLAGS = -g -Wall
EXE = app

$(EXE): main.o
	$(CC) -o $@ $^

clean:
	rm *.o $(EXE)
