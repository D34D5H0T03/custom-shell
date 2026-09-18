#compiler
CC = gcc

#compilation flags
CFLAGS = -Wall -Iinclude

#target path
TARGET = bin/custom_shell

#source files
SRCS = src/main.c src/parser.c src/executor.c src/builtins.c src/stealth.c

all: $(TARGET)

$(TARGET): $(SRCS)
	mkdir -p bin
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

# For clean [make clean]
clean:
	rm -rf bin/