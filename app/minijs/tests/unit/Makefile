PRG = tests_minijs
LIB = dmc

C_FILES = $(wildcard src/*.c) \
					../../src/Cpath.c \
					$(wildcard ../../src/ast/*.c) \
					$(wildcard ../../src/types/*.c) \
					$(wildcard ../../src/lexer/*.c) \
					$(wildcard ../../src/parser/*.c)
H_FILES = $(wildcard include/*.h) \
					$(wildcard ../../include/ast/*.h) \
					$(wildcard ../../include/types/*.h) \
					$(wildcard ../../include/lexer/*.h) \
					$(wildcard ../../include/parser/*.h)

LIB_PATH = ../../lib/lib$(LIB)
LIB_INCLUDE = ../../lib/lib$(LIB)/include

CFLAGS = -Wall -rdynamic
FILES = $(C_FILES) $(H_FILES)
bin/$(PRG): $(FILES) $(LIB_PATH)/lib$(LIB).a
	gcc $(CFLAGS) $(C_FILES) -o bin/$(PRG) \
	  -Iinclude -I../../include -I$(LIB_INCLUDE) \
		-L$(LIB_PATH) -l$(LIB) \
		-lgc
