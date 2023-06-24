PRG = cmarket
LIB1 = kut
LIB2 = mkt

C_FILES = $(wildcard src/*.c) $(wildcard src/*/*.c) $(wildcard src/*/*/*.c) $(wildcard src/*/*/*/*.c)
H_FILES = $(wildcard include/*.h) $(wildcard include/*/*.h) $(wildcard include/*/*/*.h) $(wildcard include/*/*/*/*.h)

LIB1_PATH = lib/lib$(LIB1)
LIB1_INCLUDE = lib/lib$(LIB1)/include
LIB2_PATH = lib/lib$(LIB2)
LIB2_INCLUDE = lib/lib$(LIB2)/include

CFLAGS = -Wall -rdynamic
FILES = $(C_FILES) $(H_FILES)
bin/$(PRG): $(FILES) $(LIB1_PATH)/lib$(LIB1).a $(LIB2_PATH)/lib$(LIB2).a
	gcc $(CFLAGS) $(FILES) -o bin/$(PRG) \
	  -Iinclude -I$(LIB1_INCLUDE) -I$(LIB2_INCLUDE) \
		-L$(LIB1_PATH) -l$(LIB1) \
		-L$(LIB2_PATH) -l$(LIB2) \
		-lgc -lm
