LIB_PATH_dmc = lib/libdmc
LIB_INCLUDE_dmc = $(LIB_PATH_dmc)/include

CFLAGS = -Wall -rdynamic
bin/jsclean : src/jsclean.c include/jsclean.h lib/liblocalcompilation/liblocalcompilation.a
	gcc $(CFLAGS) src/jsclean.c -o bin/jsclean \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc) \
		-Llib/liblocalcompilation -llocalcompilation \
		-L$(LIB_PATH_dmc) -ldmc \
		-lgc -lpthread -lm

lib/liblocalcompilation/liblocalcompilation.a : lib/liblocalcompilation/obj/jsclean.o lib/liblocalcompilation/obj/data/Params.o lib/liblocalcompilation/obj/io.o lib/liblocalcompilation/obj/data/Path.o lib/liblocalcompilation/obj/lint.o
	ar rcs lib/liblocalcompilation/liblocalcompilation.a lib/liblocalcompilation/obj/jsclean.o lib/liblocalcompilation/obj/data/Params.o lib/liblocalcompilation/obj/io.o lib/liblocalcompilation/obj/data/Path.o lib/liblocalcompilation/obj/lint.o

lib/liblocalcompilation/obj/jsclean.o : include/jsclean.h src/jsclean.c include/data/Params.h src/data/Params.c include/io.h src/io.c include/lint.h src/lint.c
	gcc $(CFLAGS) -c src/jsclean.c -o lib/liblocalcompilation/obj/jsclean.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/data/Params.o : include/data/Params.h src/data/Params.c
	gcc $(CFLAGS) -c src/data/Params.c -o lib/liblocalcompilation/obj/data/Params.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/io.o : include/io.h src/io.c include/data/Params.h src/data/Params.c include/data/Path.h src/data/Path.c
	gcc $(CFLAGS) -c src/io.c -o lib/liblocalcompilation/obj/io.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/data/Path.o : include/data/Path.h src/data/Path.c
	gcc $(CFLAGS) -c src/data/Path.c -o lib/liblocalcompilation/obj/data/Path.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/lint.o : include/lint.h src/lint.c include/data/Path.h src/data/Path.c
	gcc $(CFLAGS) -c src/lint.c -o lib/liblocalcompilation/obj/lint.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)
