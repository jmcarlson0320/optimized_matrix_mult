OPT_LVL = OPT_2 # optimization level: OPT_0 | OPT_1 | OPT_2
CC = gcc
CFLAGS = -g -Wall -pg -D$(OPT_LVL)
LIB = -lpthread
EXE = app

$(EXE): main.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIB)

clean:
	rm *.o $(EXE)
