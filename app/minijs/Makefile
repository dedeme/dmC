PRG = minijs

C_FILES = $(wildcard src/*.c) \
					$(wildcard src/types/*.c) \
					$(wildcard src/scan/*.c)

H_FILES = $(wildcard include/*.h) \
					$(wildcard include/types/*.c) \
					$(wildcard include/scan/*.c)

CFLAGS = -Wall -rdynamic
FILES = $(C_FILES) $(H_FILES)
bin/$(PRG): $(FILES) lib/libdmc/libdmc.a
	gcc $(CFLAGS) $(C_FILES) -o bin/$(PRG) \
	  -Iinclude \
	  -Ilib/libdmc/include -Llib/libdmc -ldmc \
	  -lgc
