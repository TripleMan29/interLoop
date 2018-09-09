CC=gcc
CFLAG=-pedantic -Wall -Wextra
CFLAGS=$(CFLAG) -c -o

Loop_Interpreter : executor.o expressions.o loader.o loop.o tokenReader.o utilities.o variables.o
$(CC) $(CFLAGS) -o Loop_Interpreter executor.o expressions.o loader.o loop.o tokenReader.o utilities.o variables.o

executor.o : executor.c
$(CC) $(CFLAGS) executor.o executor.c

expressions.o : expressions.c
$(CC) $(CFLAGS) expressions.o expressions.c

loader.o : loader.c
$(CC) $(CFLAGS) loader.o loader.c

loop.o : loop.c
$(CC) $(CFLAGS) loop.o loop.c

tokenReader.o : tokenReader.c
$(CC) $(CFLAGS) tokenReader.o tokenReader.c

utilities.o : utilities.c
$(CC) $(CFLAGS) utilities.o utilities.c

variables.o : variables.c
$(CC) $(CFLAGS) variables.o variables.c