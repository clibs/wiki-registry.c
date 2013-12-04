
SRC = $(wildcard src/*.c)
SRC += $(wildcard deps/gumbo-parser/*.c)
SRC += $(wildcard deps/*.c)

example: example.c $(SRC)
	$(CC) $^ -std=c99 -o $@ -Isrc -lcurl -Ideps
