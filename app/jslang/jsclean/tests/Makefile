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

lib/liblocalcompilation/liblocalcompilation.a : lib/liblocalcompilation/obj/tests.o lib/liblocalcompilation/obj/params_tests.o lib/liblocalcompilation/obj/io_tests.o
	ar rcs lib/liblocalcompilation/liblocalcompilation.a lib/liblocalcompilation/obj/tests.o lib/liblocalcompilation/obj/params_tests.o lib/liblocalcompilation/obj/io_tests.o

lib/liblocalcompilation/obj/tests.o : include/tests.h src/tests.c include/params_tests.h src/params_tests.c include/io_tests.h src/io_tests.c
	gcc $(CFLAGS) -c src/tests.c -o lib/liblocalcompilation/obj/tests.o \
		-Iinclude \
		-I$(LIB_INCLUDE_local) \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/params_tests.o : include/params_tests.h src/params_tests.c
	gcc $(CFLAGS) -c src/params_tests.c -o lib/liblocalcompilation/obj/params_tests.o \
		-Iinclude \
		-I$(LIB_INCLUDE_local) \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/io_tests.o : include/io_tests.h src/io_tests.c
	gcc $(CFLAGS) -c src/io_tests.c -o lib/liblocalcompilation/obj/io_tests.o \
		-Iinclude \
		-I$(LIB_INCLUDE_local) \
		-I$(LIB_INCLUDE_dmc)

