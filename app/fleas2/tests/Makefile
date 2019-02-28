PRG = fleas2_tests
LIB = dmc

C_FILES = $(wildcard src/*.c) \
					$(filter-out ../src/fleas.c, $(wildcard ../src/*.c)) \
					 $(wildcard ../src/models/*.c)
H_FILES = $(wildcard include/*.h) \
					$(filter-out ../include/fleas.h, $(wildcard ../include/*.h)) \
					$(wildcard ../include/models/*.h)

LIB_PATH = ../lib/lib$(LIB)
LIB_INCLUDE = ../lib/lib$(LIB)/include

CFLAGS = -Wall -rdynamic
FILES = $(C_FILES) $(H_FILES)

bin/$(PRG) : $(FILES) $(LIB_PATH)/lib$(LIB).a
	gcc $(CFLAGS) $(C_FILES) -o bin/$(PRG) -Iinclude -I../include \
	  -I$(LIB_INCLUDE) -L$(LIB_PATH) -l$(LIB) \
	  -lm

.PHONY: valgrind

valgrind :
	gcc $(CFLAGS) -O0 -g $(C_FILES) -o bin/v_$(PRG) -Iinclude -I../include \
	  -I$(LIB_INCLUDE) -L$(LIB_PATH) -l$(LIB) \
	  -lm
