PRG = tests
LIB = dmc

C_FILES = $(wildcard src/*.c) \
					$(filter-out ../src/main.c, $(wildcard ../src/*.c)) \
					$(wildcard ../src/tpls/*.c)
H_FILES = $(wildcard include/*.h) \
					$(filter-out ../include/main.h, $(wildcard ../include/*.h)) \
					$(wildcard ../include/tpls/*.h)

LIB_PATH = ../lib/lib$(LIB)
LIB_INCLUDE = ../lib/lib$(LIB)/include

CFLAGS = -Wall -rdynamic
FILES = $(C_FILES) $(H_FILES)
bin/$(PRG): $(FILES) $(LIB_PATH)/lib$(LIB).a
	gcc $(CFLAGS) $(FILES) -o bin/$(PRG) \
	  -Iinclude -I../include -I$(LIB_INCLUDE) \
		-L$(LIB_PATH) -l$(LIB) \
		-lgc -lm
