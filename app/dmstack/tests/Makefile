LIB_PATH_local = lib/liblocal
LIB_INCLUDE_local = $(LIB_PATH_local)/include

LIB_PATH_dmc = lib/libdmc
LIB_INCLUDE_dmc = $(LIB_PATH_dmc)/include

CFLAGS = -Wall -rdynamic
bin/tests : src/tests.c include/tests.h lib/liblocalcompilation/liblocalcompilation.a
	gcc $(CFLAGS) src/tests.c -o bin/tests \
		-Iinclude \
		-I$(LIB_INCLUDE_local) \
		-I$(LIB_INCLUDE_dmc) \
		-Llib/liblocalcompilation -llocalcompilation \
		-L$(LIB_PATH_local) -llocal \
		-L$(LIB_PATH_dmc) -ldmc \
		-lgc -lpthread -lm

lib/liblocalcompilation/liblocalcompilation.a : lib/liblocalcompilation/obj/tests.o lib/liblocalcompilation/obj/reader_tests.o
	ar rcs lib/liblocalcompilation/liblocalcompilation.a lib/liblocalcompilation/obj/tests.o lib/liblocalcompilation/obj/reader_tests.o

lib/liblocalcompilation/obj/tests.o : include/tests.h src/tests.c include/reader_tests.h src/reader_tests.c
	gcc $(CFLAGS) -c src/tests.c -o lib/liblocalcompilation/obj/tests.o \
		-Iinclude \
		-I$(LIB_INCLUDE_local) \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/reader_tests.o : include/reader_tests.h src/reader_tests.c
	gcc $(CFLAGS) -c src/reader_tests.c -o lib/liblocalcompilation/obj/reader_tests.o \
		-Iinclude \
		-I$(LIB_INCLUDE_local) \
		-I$(LIB_INCLUDE_dmc)

