CC = gcc
CFLAGS = -Wall -g
LDLIBS = -lreadline

beershell: shell.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)
