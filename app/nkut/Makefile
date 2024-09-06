PRG = nkut
LIB = kut

C_FILES = $(wildcard src/*.c) $(wildcard src/*/*.c) $(wildcard src/*/*/*.c)
H_FILES = $(wildcard include/*.h) $(wildcard include/*/*.h) $(wildcard include/*/*/*.h)

LIB_PATH = lib/lib$(LIB)
LIB_INCLUDE = lib/lib$(LIB)/include

CFLAGS = -Wall -rdynamic -z execstack
FILES = $(C_FILES) $(H_FILES)
bin/$(PRG): $(FILES) $(LIB_PATH)/lib$(LIB).a
	gcc $(CFLAGS) $(FILES) -o bin/$(PRG) \
	  -Iinclude -I$(LIB_INCLUDE) \
		-L$(LIB_PATH) -l$(LIB) \
		-lgc -lpthread -lm
