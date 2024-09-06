PRG = mkso

C_FILES = $(wildcard src/*.c)

CFLAGS = -Wall -rdynamic

FILES = $(C_FILES)
bin/$(PRG): $(FILES)
	gcc $(CFLAGS) $(FILES) -o bin/$(PRG) -lm -ldl
