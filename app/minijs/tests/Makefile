PRG = tests_minijs
LIB = dmc

C_FILES = $(wildcard src/*.c) \
					../src/Achar.c \
					../src/Mchar.c \
					../src/Pos.c \
					../src/Cpath.c \
					$(wildcard ../src/builtin/*.c) \
					$(wildcard ../src/lexer/*.c) \
					$(wildcard ../src/parser/*.c) \
					$(wildcard ../src/ast/*.c)
H_FILES = $(wildcard include/*.h) \
					../include/Achar.h \
					../include/Mchar.h \
					../include/Pos.h \
					../include/Cpath.h \
					$(wildcard ../include/builtin/*.h) \
					$(wildcard ../include/lexer/*.h) \
					$(wildcard ../include/parser/*.h) \
					$(wildcard ../include/ast/*.h)

LIB_PATH = ../lib/lib$(LIB)
LIB_INCLUDE = ../lib/lib$(LIB)/include

CFLAGS = -Wall -rdynamic
FILES = $(C_FILES) $(H_FILES)
bin/$(PRG): $(FILES) $(LIB_PATH)/lib$(LIB).a
	gcc $(CFLAGS) $(C_FILES) -o bin/$(PRG) \
	  -Iinclude -I../include -I$(LIB_INCLUDE) \
		-L$(LIB_PATH) -l$(LIB) \
		-lgc
