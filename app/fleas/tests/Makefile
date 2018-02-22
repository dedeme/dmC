PRG = fleas_tests
LIB = dmc

C_FILES = ../src/Gen.c ../src/Quote.c \
					../src/order/*.c \
					../src/Nick.c ../src/io.c ../src/Flea.c  ../src/Stat.c \
					../src/fextra.c ../src/Portfolio.c ../src/Db.c ../src/Trace.c  \
					../src/families/*.c \
          $(wildcard src/*.c)
H_FILES = $(wildcard include/*.h) $(wildcard ../include/*.h)

LIB_PATH = ../lib/lib$(LIB)
LIB_INCLUDE = ../lib/lib$(LIB)/include

CFLAGS = -Wall -rdynamic
FILES = $(C_FILES) $(H_FILES)
bin/$(PRG): $(FILES) $(LIB_PATH)/lib$(LIB).a
	gcc $(CFLAGS) $(C_FILES) -o bin/$(PRG) \
	  -Iinclude -I../include -I$(LIB_INCLUDE) \
		-L$(LIB_PATH) -l$(LIB) \
		-lgc -lm
