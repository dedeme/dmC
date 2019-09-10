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

lib/liblocalcompilation/liblocalcompilation.a : lib/liblocalcompilation/obj/dmstack.o lib/liblocalcompilation/obj/primitives.o lib/liblocalcompilation/obj/modules/modsys.o lib/liblocalcompilation/obj/fails.o lib/liblocalcompilation/obj/Machine.o lib/liblocalcompilation/obj/Reader.o lib/liblocalcompilation/obj/DEFS.o lib/liblocalcompilation/obj/tkreader.o lib/liblocalcompilation/obj/Token.o lib/liblocalcompilation/obj/Symbol.o lib/liblocalcompilation/obj/Heap.o lib/liblocalcompilation/obj/modules/modsys0.o lib/liblocalcompilation/obj/modules/modsys1.o lib/liblocalcompilation/obj/modules/modsys2.o lib/liblocalcompilation/obj/modules/modstk.o lib/liblocalcompilation/obj/modules/modblob.o lib/liblocalcompilation/obj/modules/modint.o lib/liblocalcompilation/obj/modules/modfloat.o lib/liblocalcompilation/obj/modules/modmath.o lib/liblocalcompilation/obj/modules/modlist.o lib/liblocalcompilation/obj/modules/modmap.o lib/liblocalcompilation/obj/modules/modstr.o
	ar rcs lib/liblocalcompilation/liblocalcompilation.a lib/liblocalcompilation/obj/dmstack.o lib/liblocalcompilation/obj/primitives.o lib/liblocalcompilation/obj/modules/modsys.o lib/liblocalcompilation/obj/fails.o lib/liblocalcompilation/obj/Machine.o lib/liblocalcompilation/obj/Reader.o lib/liblocalcompilation/obj/DEFS.o lib/liblocalcompilation/obj/tkreader.o lib/liblocalcompilation/obj/Token.o lib/liblocalcompilation/obj/Symbol.o lib/liblocalcompilation/obj/Heap.o lib/liblocalcompilation/obj/modules/modsys0.o lib/liblocalcompilation/obj/modules/modsys1.o lib/liblocalcompilation/obj/modules/modsys2.o lib/liblocalcompilation/obj/modules/modstk.o lib/liblocalcompilation/obj/modules/modblob.o lib/liblocalcompilation/obj/modules/modint.o lib/liblocalcompilation/obj/modules/modfloat.o lib/liblocalcompilation/obj/modules/modmath.o lib/liblocalcompilation/obj/modules/modlist.o lib/liblocalcompilation/obj/modules/modmap.o lib/liblocalcompilation/obj/modules/modstr.o

lib/liblocalcompilation/obj/dmstack.o : include/dmstack.h src/dmstack.c include/primitives.h src/primitives.c include/Reader.h src/Reader.c include/Machine.h src/Machine.c include/fails.h src/fails.c include/DEFS.h src/DEFS.c
	gcc $(CFLAGS) -c src/dmstack.c -o lib/liblocalcompilation/obj/dmstack.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/primitives.o : include/primitives.h src/primitives.c include/modules/modsys.h src/modules/modsys.c include/modules/modstk.h src/modules/modstk.c include/modules/modblob.h src/modules/modblob.c include/modules/modint.h src/modules/modint.c include/modules/modfloat.h src/modules/modfloat.c include/modules/modmath.h src/modules/modmath.c include/modules/modlist.h src/modules/modlist.c include/modules/modmap.h src/modules/modmap.c include/modules/modstr.h src/modules/modstr.c include/Machine.h src/Machine.c
	gcc $(CFLAGS) -c src/primitives.c -o lib/liblocalcompilation/obj/primitives.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/modules/modsys.o : include/modules/modsys.h src/modules/modsys.c include/fails.h src/fails.c include/modules/modsys0.h src/modules/modsys0.c include/modules/modsys1.h src/modules/modsys1.c include/modules/modsys2.h src/modules/modsys2.c include/Token.h src/Token.c include/primitives.h src/primitives.c
	gcc $(CFLAGS) -c src/modules/modsys.c -o lib/liblocalcompilation/obj/modules/modsys.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/fails.o : include/fails.h src/fails.c include/Machine.h src/Machine.c
	gcc $(CFLAGS) -c src/fails.c -o lib/liblocalcompilation/obj/fails.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/Machine.o : include/Machine.h src/Machine.c include/Reader.h src/Reader.c include/primitives.h src/primitives.c include/fails.h src/fails.c include/Heap.h src/Heap.c
	gcc $(CFLAGS) -c src/Machine.c -o lib/liblocalcompilation/obj/Machine.o \
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

lib/liblocalcompilation/obj/Token.o : include/Token.h src/Token.c include/Symbol.h src/Symbol.c
	gcc $(CFLAGS) -c src/Token.c -o lib/liblocalcompilation/obj/Token.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/Symbol.o : include/Symbol.h src/Symbol.c
	gcc $(CFLAGS) -c src/Symbol.c -o lib/liblocalcompilation/obj/Symbol.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/Heap.o : include/Heap.h src/Heap.c include/Token.h src/Token.c
	gcc $(CFLAGS) -c src/Heap.c -o lib/liblocalcompilation/obj/Heap.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/modules/modsys0.o : include/modules/modsys0.h src/modules/modsys0.c include/fails.h src/fails.c include/Machine.h src/Machine.c
	gcc $(CFLAGS) -c src/modules/modsys0.c -o lib/liblocalcompilation/obj/modules/modsys0.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/modules/modsys1.o : include/modules/modsys1.h src/modules/modsys1.c include/fails.h src/fails.c include/Machine.h src/Machine.c
	gcc $(CFLAGS) -c src/modules/modsys1.c -o lib/liblocalcompilation/obj/modules/modsys1.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/modules/modsys2.o : include/modules/modsys2.h src/modules/modsys2.c include/fails.h src/fails.c include/Machine.h src/Machine.c
	gcc $(CFLAGS) -c src/modules/modsys2.c -o lib/liblocalcompilation/obj/modules/modsys2.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/modules/modstk.o : include/modules/modstk.h src/modules/modstk.c include/fails.h src/fails.c include/Machine.h src/Machine.c include/primitives.h src/primitives.c
	gcc $(CFLAGS) -c src/modules/modstk.c -o lib/liblocalcompilation/obj/modules/modstk.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/modules/modblob.o : include/modules/modblob.h src/modules/modblob.c include/fails.h src/fails.c include/Machine.h src/Machine.c include/primitives.h src/primitives.c
	gcc $(CFLAGS) -c src/modules/modblob.c -o lib/liblocalcompilation/obj/modules/modblob.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/modules/modint.o : include/modules/modint.h src/modules/modint.c include/fails.h src/fails.c include/Machine.h src/Machine.c include/primitives.h src/primitives.c
	gcc $(CFLAGS) -c src/modules/modint.c -o lib/liblocalcompilation/obj/modules/modint.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/modules/modfloat.o : include/modules/modfloat.h src/modules/modfloat.c include/fails.h src/fails.c include/Machine.h src/Machine.c include/primitives.h src/primitives.c
	gcc $(CFLAGS) -c src/modules/modfloat.c -o lib/liblocalcompilation/obj/modules/modfloat.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/modules/modmath.o : include/modules/modmath.h src/modules/modmath.c include/fails.h src/fails.c include/Machine.h src/Machine.c include/primitives.h src/primitives.c
	gcc $(CFLAGS) -c src/modules/modmath.c -o lib/liblocalcompilation/obj/modules/modmath.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/modules/modlist.o : include/modules/modlist.h src/modules/modlist.c include/fails.h src/fails.c include/Machine.h src/Machine.c include/primitives.h src/primitives.c
	gcc $(CFLAGS) -c src/modules/modlist.c -o lib/liblocalcompilation/obj/modules/modlist.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/modules/modmap.o : include/modules/modmap.h src/modules/modmap.c include/fails.h src/fails.c include/Machine.h src/Machine.c include/primitives.h src/primitives.c
	gcc $(CFLAGS) -c src/modules/modmap.c -o lib/liblocalcompilation/obj/modules/modmap.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/modules/modstr.o : include/modules/modstr.h src/modules/modstr.c include/fails.h src/fails.c include/Machine.h src/Machine.c include/primitives.h src/primitives.c
	gcc $(CFLAGS) -c src/modules/modstr.c -o lib/liblocalcompilation/obj/modules/modstr.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

