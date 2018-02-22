PRG = fleas
LIB = dmc

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
bin/$(PRG): $(C_FILES) $(LIB_PATH)/lib$(LIB).a
	gcc $(CFLAGS) $(FILES) -o bin/$(PRG) \
	  -Iinclude -I$(LIB_INCLUDE) \
		-L$(LIB_PATH) -l$(LIB) \
		-lgc -lm
