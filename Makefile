CC = clang
RM = rm -f
OUTPUT = dump

default: build

build:
	$(CC) dump.c -o $(OUTPUT)

clean:
	$(RM) $(OUTPUT)
