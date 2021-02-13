CC = gcc
CFLAGS = -g -Wall
LIB = -lpthread
EXE = app

$(EXE): main.o
	$(CC) -o $@ $^ $(LIB)

clean:
	rm *.o $(EXE)
