PRG = fleas
LIB = market

C_FILES = $(wildcard src/*.c) \
          $(wildcard src/order/*.c) \
          $(wildcard src/tests/*.c) \
          $(wildcard src/families/*.c)
H_FILES = $(wildcard include/*.h) \
          $(wildcard include/order/*.h) \
          $(wildcard include/tests/*.h) \
          $(wildcard include/families/*.h)

LIB_PATH = lib/lib$(LIB)
LIB_INCLUDE = lib/lib$(LIB)/include

CFLAGS = -Wall -rdynamic
FILES = $(C_FILES) $(H_FILES)
bin/$(PRG): $(FILES) $(LIB_PATH)/lib$(LIB).a
	gcc $(CFLAGS) $(C_FILES) -o bin/$(PRG) \
	  -Iinclude -I$(LIB_INCLUDE) \
		-L$(LIB_PATH) -l$(LIB) \
		-lgc -lm
