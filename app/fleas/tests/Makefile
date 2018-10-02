PRG = fleas_tests
LIB = dmc
LIB2 = market

C_FILES = $(wildcard src/*.c) \
					../src/Flea.c ../src/Gen.c ../src/io.c \
					$(wildcard ../src/data/*.c) \
					$(wildcard ../src/families/*.c) \
					$(wildcard ../src/utils/*.c)
H_FILES = $(wildcard include/*.h) \
					../include/Flea.h ../include/Gen.h ../include/io.h ../include/DEFS.h \
					$(wildcard ../include/families/*.h) \
					$(wildcard ../include/data/*.h) \
					$(wildcard ../include/utils/*.h)

LIB_PATH = ../lib/lib$(LIB)
LIB_INCLUDE = ../lib/lib$(LIB)/include
LIB2_PATH = ../lib/lib$(LIB2)
LIB2_INCLUDE = ../lib/lib$(LIB2)/include

CFLAGS = -Wall -rdynamic
FILES = $(C_FILES) $(H_FILES)
bin/$(PRG): $(FILES) $(LIB_PATH)/lib$(LIB).a $(LIB2_PATH)/lib$(LIB2).a
	gcc $(CFLAGS) $(C_FILES) -o bin/$(PRG) \
	  -Iinclude -I../include -I$(LIB_INCLUDE) -I$(LIB2_INCLUDE) \
		-L$(LIB_PATH) -l$(LIB) \
		-L$(LIB2_PATH) -l$(LIB2) \
		-lgc -lm
