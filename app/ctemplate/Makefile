PRG = ctemplate
LIB = dmc

C_FILES = $(wildcard src/*.c) \
					$(wildcard src/readers/*.c) \
					$(wildcard src/writers/*.c)
H_FILES = $(wildcard include/*.h) \
					$(wildcard include/readers/*.h) \
					$(wildcard include/writers/*.h)

LIB_PATH = lib/lib$(LIB)
LIB_INCLUDE = lib/lib$(LIB)/include

CFLAGS = -Wall -rdynamic
FILES = $(C_FILES) $(H_FILES)
bin/$(PRG): $(FILES) $(LIB_PATH)/lib$(LIB).a
	gcc $(CFLAGS) $(C_FILES) -o bin/$(PRG) \
	  -Iinclude -I$(LIB_INCLUDE) \
		-L$(LIB_PATH) -l$(LIB) \
		-lgc
