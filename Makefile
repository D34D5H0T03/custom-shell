#compiler
CC = gcc

#compilation flags
CFLAGS = -Wall -Iinclude

all: custom_shell

#executable
custom_shell: src/main.c src/parser.c src/executor.c
	$(CC) $(CFLAGS) src/main.c src/parser.c src/executor.c -o custom_shell

#for clean [make clean]
clean:
	rm -f custom_shell