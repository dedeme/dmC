LIB_PATH_dmc = lib/libdmc
LIB_INCLUDE_dmc = $(LIB_PATH_dmc)/include

LIB_PATH_local = lib/liblocal
LIB_INCLUDE_local = $(LIB_PATH_local)/include

CFLAGS = -Wall -rdynamic
bin/tests : src/tests.c include/tests.h lib/liblocalcompilation/liblocalcompilation.a
	gcc $(CFLAGS) src/tests.c -o bin/tests \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc) \
		-I$(LIB_INCLUDE_local) \
		-Llib/liblocalcompilation -llocalcompilation \
		-L$(LIB_PATH_dmc) -ldmc \
		-L$(LIB_PATH_local) -llocal \
		-lgc -lpthread -lm

lib/liblocalcompilation/liblocalcompilation.a : lib/liblocalcompilation/obj/tests.o
	ar rcs lib/liblocalcompilation/liblocalcompilation.a lib/liblocalcompilation/obj/tests.o

lib/liblocalcompilation/obj/tests.o : include/tests.h src/tests.c
	gcc $(CFLAGS) -c src/tests.c -o lib/liblocalcompilation/obj/tests.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc) \
		-I$(LIB_INCLUDE_local)

