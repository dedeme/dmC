PRG = fleas
LIB = dmc
LIB2 = market

C_FILES = $(wildcard src/*.c)
H_FILES = $(wildcard include/*.h)

LIB_PATH = lib/lib$(LIB)
LIB_INCLUDE = lib/lib$(LIB)/include
LIB2_PATH = lib/lib$(LIB2)
LIB2_INCLUDE = lib/lib$(LIB2)/include

CFLAGS = -Wall -rdynamic
FILES = $(C_FILES) $(H_FILES)

bin/$(PRG): $(FILES) $(LIB_PATH)/lib$(LIB).a $(LIB2_PATH)/lib$(LIB2).a
	gcc $(CFLAGS) $(C_FILES) -o bin/$(PRG) -Iinclude \
	  -I$(LIB2_INCLUDE) -L$(LIB2_PATH) -l$(LIB2) \
	  -I$(LIB_INCLUDE) -L$(LIB_PATH) -l$(LIB) \
	  -lm

valgrind :
	gcc $(CFLAGS) -O0 -g $(C_FILES) -o bin/v_$(PRG) -Iinclude  \
	  -I$(LIB2_INCLUDE) -L$(LIB2_PATH) -l$(LIB2) \
	  -I$(LIB_INCLUDE) -L$(LIB_PATH) -l$(LIB) \
	  -lm
