PRG = ctpl_tests
LIB = dmc

C_FILES = $(wildcard src/*.c) \
	$(filter-out ../src/ctpl.c, $(wildcard ../src/*.c))
H_FILES = $(wildcard include/*.h) \
	$(filter-out ../include/ctpl.h, $(wildcard ../include/*.h))

LIB_PATH = ../lib/lib$(LIB)
LIB_INCLUDE = ../lib/lib$(LIB)/include

CFLAGS = -Wall -rdynamic
FILES = $(C_FILES) $(H_FILES)

bin/$(PRG) : $(FILES) $(LIB_PATH)/lib$(LIB).a
	gcc $(CFLAGS) $(C_FILES) -o bin/$(PRG) \
		-Iinclude -I$(LIB_INCLUDE) \
		-L$(LIB_PATH) -l$(LIB) \
		-I../include

.PHONY: valgrind

valgrind :
	gcc $(CFLAGS) -O0 -g $(C_FILES) -o bin/v_$(PRG) \
		-Iinclude -I$(LIB_INCLUDE) \
		-L$(LIB_PATH) -l$(LIB) \
		-I../include
