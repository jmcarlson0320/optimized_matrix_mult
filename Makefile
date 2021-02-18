CC = gcc
CFLAGS = -g -Wall -pg
LIB = -lpthread
EXE = app

$(EXE): main.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIB)

clean:
	rm *.o $(EXE)
