LIB_PATH_dmc = lib/libdmc
LIB_INCLUDE_dmc = $(LIB_PATH_dmc)/include

CFLAGS = -Wall -rdynamic
bin/dmstack : src/dmstack.c include/dmstack.h lib/liblocalcompilation/liblocalcompilation.a
	gcc $(CFLAGS) src/dmstack.c -o bin/dmstack \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc) \
		-Llib/liblocalcompilation -llocalcompilation \
		-L$(LIB_PATH_dmc) -ldmc \
		-lgc -lpthread -lm

lib/liblocalcompilation/liblocalcompilation.a : lib/liblocalcompilation/obj/dmstack.o lib/liblocalcompilation/obj/main.o lib/liblocalcompilation/obj/Reader.o lib/liblocalcompilation/obj/DEFS.o lib/liblocalcompilation/obj/tkreader.o lib/liblocalcompilation/obj/Token.o lib/liblocalcompilation/obj/Machine.o lib/liblocalcompilation/obj/primitives.o lib/liblocalcompilation/obj/modules/modsys.o lib/liblocalcompilation/obj/fails.o
	ar rcs lib/liblocalcompilation/liblocalcompilation.a lib/liblocalcompilation/obj/dmstack.o lib/liblocalcompilation/obj/main.o lib/liblocalcompilation/obj/Reader.o lib/liblocalcompilation/obj/DEFS.o lib/liblocalcompilation/obj/tkreader.o lib/liblocalcompilation/obj/Token.o lib/liblocalcompilation/obj/Machine.o lib/liblocalcompilation/obj/primitives.o lib/liblocalcompilation/obj/modules/modsys.o lib/liblocalcompilation/obj/fails.o

lib/liblocalcompilation/obj/dmstack.o : include/dmstack.h src/dmstack.c include/main.h src/main.c include/primitives.h src/primitives.c include/Reader.h src/Reader.c include/Machine.h src/Machine.c
	gcc $(CFLAGS) -c src/dmstack.c -o lib/liblocalcompilation/obj/dmstack.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/main.o : include/main.h src/main.c include/Reader.h src/Reader.c include/Machine.h src/Machine.c
	gcc $(CFLAGS) -c src/main.c -o lib/liblocalcompilation/obj/main.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/Reader.o : include/Reader.h src/Reader.c include/DEFS.h src/DEFS.c include/tkreader.h src/tkreader.c include/Token.h src/Token.c
	gcc $(CFLAGS) -c src/Reader.c -o lib/liblocalcompilation/obj/Reader.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/DEFS.o : include/DEFS.h src/DEFS.c
	gcc $(CFLAGS) -c src/DEFS.c -o lib/liblocalcompilation/obj/DEFS.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/tkreader.o : include/tkreader.h src/tkreader.c include/Reader.h src/Reader.c
	gcc $(CFLAGS) -c src/tkreader.c -o lib/liblocalcompilation/obj/tkreader.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/Token.o : include/Token.h src/Token.c
	gcc $(CFLAGS) -c src/Token.c -o lib/liblocalcompilation/obj/Token.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/Machine.o : include/Machine.h src/Machine.c include/primitives.h src/primitives.c include/fails.h src/fails.c include/Token.h src/Token.c
	gcc $(CFLAGS) -c src/Machine.c -o lib/liblocalcompilation/obj/Machine.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/primitives.o : include/primitives.h src/primitives.c include/modules/modsys.h src/modules/modsys.c include/Machine.h src/Machine.c
	gcc $(CFLAGS) -c src/primitives.c -o lib/liblocalcompilation/obj/primitives.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/modules/modsys.o : include/modules/modsys.h src/modules/modsys.c include/fails.h src/fails.c include/primitives.h src/primitives.c
	gcc $(CFLAGS) -c src/modules/modsys.c -o lib/liblocalcompilation/obj/modules/modsys.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/fails.o : include/fails.h src/fails.c include/Machine.h src/Machine.c
	gcc $(CFLAGS) -c src/fails.c -o lib/liblocalcompilation/obj/fails.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)