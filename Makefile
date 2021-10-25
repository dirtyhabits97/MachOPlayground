CC = clang
RM = rm -f
OUTPUT = dump

default: build

build:
	$(CC) source/*.c dump.c -o $(OUTPUT)

clean:
	$(RM) $(OUTPUT)
