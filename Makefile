CC=gcc
CFLAGS=-Wall
OBJS=shell.o
EXE=minishell

$(EXE): $(OBJS)
	$(CC) $(CFLAGS) -o $(EXE) $(OBJS)

.c.o:
	$(CC) $(CFLAGS) -c $<

clean:
	rm *.o $(EXE)
