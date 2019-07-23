LIB_PATH_dmc = lib/libdmc
LIB_INCLUDE_dmc = $(LIB_PATH_dmc)/include

CFLAGS = -Wall -rdynamic
bin/cnewfile : src/cnewfile.c include/cnewfile.h lib/liblocalcompilation/liblocalcompilation.a
	gcc $(CFLAGS) src/cnewfile.c -o bin/cnewfile \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc) \
		-Llib/liblocalcompilation -llocalcompilation \
		-L$(LIB_PATH_dmc) -ldmc \
		-lgc -lpthread -lm

lib/liblocalcompilation/liblocalcompilation.a : lib/liblocalcompilation/obj/cnewfile.o
	ar rcs lib/liblocalcompilation/liblocalcompilation.a lib/liblocalcompilation/obj/cnewfile.o

lib/liblocalcompilation/obj/cnewfile.o : include/cnewfile.h src/cnewfile.c
	gcc $(CFLAGS) -c src/cnewfile.c -o lib/liblocalcompilation/obj/cnewfile.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

