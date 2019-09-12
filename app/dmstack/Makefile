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

lib/liblocalcompilation/liblocalcompilation.a : lib/liblocalcompilation/obj/dmstack.o lib/liblocalcompilation/obj/primitives.o lib/liblocalcompilation/obj/primitives/modsys.o lib/liblocalcompilation/obj/fails.o lib/liblocalcompilation/obj/Machine.o lib/liblocalcompilation/obj/Reader.o lib/liblocalcompilation/obj/DEFS.o lib/liblocalcompilation/obj/tkreader.o lib/liblocalcompilation/obj/Token.o lib/liblocalcompilation/obj/Symbol.o lib/liblocalcompilation/obj/modules.o lib/liblocalcompilation/obj/modules/modit.o lib/liblocalcompilation/obj/Heap.o lib/liblocalcompilation/obj/primitives/modsys0.o lib/liblocalcompilation/obj/primitives/modsys1.o lib/liblocalcompilation/obj/primitives/modsys2.o lib/liblocalcompilation/obj/primitives/modstk.o lib/liblocalcompilation/obj/primitives/modblob.o lib/liblocalcompilation/obj/primitives/modint.o lib/liblocalcompilation/obj/primitives/modfloat.o lib/liblocalcompilation/obj/primitives/modmath.o lib/liblocalcompilation/obj/primitives/modlist.o lib/liblocalcompilation/obj/primitives/modmap.o lib/liblocalcompilation/obj/primitives/modstr.o
	ar rcs lib/liblocalcompilation/liblocalcompilation.a lib/liblocalcompilation/obj/dmstack.o lib/liblocalcompilation/obj/primitives.o lib/liblocalcompilation/obj/primitives/modsys.o lib/liblocalcompilation/obj/fails.o lib/liblocalcompilation/obj/Machine.o lib/liblocalcompilation/obj/Reader.o lib/liblocalcompilation/obj/DEFS.o lib/liblocalcompilation/obj/tkreader.o lib/liblocalcompilation/obj/Token.o lib/liblocalcompilation/obj/Symbol.o lib/liblocalcompilation/obj/modules.o lib/liblocalcompilation/obj/modules/modit.o lib/liblocalcompilation/obj/Heap.o lib/liblocalcompilation/obj/primitives/modsys0.o lib/liblocalcompilation/obj/primitives/modsys1.o lib/liblocalcompilation/obj/primitives/modsys2.o lib/liblocalcompilation/obj/primitives/modstk.o lib/liblocalcompilation/obj/primitives/modblob.o lib/liblocalcompilation/obj/primitives/modint.o lib/liblocalcompilation/obj/primitives/modfloat.o lib/liblocalcompilation/obj/primitives/modmath.o lib/liblocalcompilation/obj/primitives/modlist.o lib/liblocalcompilation/obj/primitives/modmap.o lib/liblocalcompilation/obj/primitives/modstr.o

lib/liblocalcompilation/obj/dmstack.o : include/dmstack.h src/dmstack.c include/primitives.h src/primitives.c include/modules.h src/modules.c include/Reader.h src/Reader.c include/Machine.h src/Machine.c include/fails.h src/fails.c include/DEFS.h src/DEFS.c
	gcc $(CFLAGS) -c src/dmstack.c -o lib/liblocalcompilation/obj/dmstack.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/primitives.o : include/primitives.h src/primitives.c include/primitives/modsys.h src/primitives/modsys.c include/primitives/modstk.h src/primitives/modstk.c include/primitives/modblob.h src/primitives/modblob.c include/primitives/modint.h src/primitives/modint.c include/primitives/modfloat.h src/primitives/modfloat.c include/primitives/modmath.h src/primitives/modmath.c include/primitives/modlist.h src/primitives/modlist.c include/primitives/modmap.h src/primitives/modmap.c include/primitives/modstr.h src/primitives/modstr.c include/Machine.h src/Machine.c
	gcc $(CFLAGS) -c src/primitives.c -o lib/liblocalcompilation/obj/primitives.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/primitives/modsys.o : include/primitives/modsys.h src/primitives/modsys.c include/fails.h src/fails.c include/primitives/modsys0.h src/primitives/modsys0.c include/primitives/modsys1.h src/primitives/modsys1.c include/primitives/modsys2.h src/primitives/modsys2.c include/Token.h src/Token.c include/primitives.h src/primitives.c
	gcc $(CFLAGS) -c src/primitives/modsys.c -o lib/liblocalcompilation/obj/primitives/modsys.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/fails.o : include/fails.h src/fails.c include/Machine.h src/Machine.c
	gcc $(CFLAGS) -c src/fails.c -o lib/liblocalcompilation/obj/fails.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/Machine.o : include/Machine.h src/Machine.c include/Reader.h src/Reader.c include/primitives.h src/primitives.c include/modules.h src/modules.c include/fails.h src/fails.c include/Heap.h src/Heap.c
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

lib/liblocalcompilation/obj/modules.o : include/modules.h src/modules.c include/modules/modit.h src/modules/modit.c
	gcc $(CFLAGS) -c src/modules.c -o lib/liblocalcompilation/obj/modules.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/modules/modit.o : include/modules/modit.h src/modules/modit.c include/Reader.h src/Reader.c
	gcc $(CFLAGS) -c src/modules/modit.c -o lib/liblocalcompilation/obj/modules/modit.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/Heap.o : include/Heap.h src/Heap.c include/Token.h src/Token.c
	gcc $(CFLAGS) -c src/Heap.c -o lib/liblocalcompilation/obj/Heap.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/primitives/modsys0.o : include/primitives/modsys0.h src/primitives/modsys0.c include/fails.h src/fails.c include/Machine.h src/Machine.c
	gcc $(CFLAGS) -c src/primitives/modsys0.c -o lib/liblocalcompilation/obj/primitives/modsys0.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/primitives/modsys1.o : include/primitives/modsys1.h src/primitives/modsys1.c include/fails.h src/fails.c include/Machine.h src/Machine.c
	gcc $(CFLAGS) -c src/primitives/modsys1.c -o lib/liblocalcompilation/obj/primitives/modsys1.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/primitives/modsys2.o : include/primitives/modsys2.h src/primitives/modsys2.c include/fails.h src/fails.c include/Machine.h src/Machine.c
	gcc $(CFLAGS) -c src/primitives/modsys2.c -o lib/liblocalcompilation/obj/primitives/modsys2.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/primitives/modstk.o : include/primitives/modstk.h src/primitives/modstk.c include/fails.h src/fails.c include/Machine.h src/Machine.c include/primitives.h src/primitives.c
	gcc $(CFLAGS) -c src/primitives/modstk.c -o lib/liblocalcompilation/obj/primitives/modstk.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/primitives/modblob.o : include/primitives/modblob.h src/primitives/modblob.c include/fails.h src/fails.c include/Machine.h src/Machine.c include/primitives.h src/primitives.c
	gcc $(CFLAGS) -c src/primitives/modblob.c -o lib/liblocalcompilation/obj/primitives/modblob.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/primitives/modint.o : include/primitives/modint.h src/primitives/modint.c include/fails.h src/fails.c include/Machine.h src/Machine.c include/primitives.h src/primitives.c
	gcc $(CFLAGS) -c src/primitives/modint.c -o lib/liblocalcompilation/obj/primitives/modint.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/primitives/modfloat.o : include/primitives/modfloat.h src/primitives/modfloat.c include/fails.h src/fails.c include/Machine.h src/Machine.c include/primitives.h src/primitives.c
	gcc $(CFLAGS) -c src/primitives/modfloat.c -o lib/liblocalcompilation/obj/primitives/modfloat.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/primitives/modmath.o : include/primitives/modmath.h src/primitives/modmath.c include/fails.h src/fails.c include/Machine.h src/Machine.c include/primitives.h src/primitives.c
	gcc $(CFLAGS) -c src/primitives/modmath.c -o lib/liblocalcompilation/obj/primitives/modmath.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/primitives/modlist.o : include/primitives/modlist.h src/primitives/modlist.c include/fails.h src/fails.c include/Machine.h src/Machine.c include/primitives.h src/primitives.c
	gcc $(CFLAGS) -c src/primitives/modlist.c -o lib/liblocalcompilation/obj/primitives/modlist.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/primitives/modmap.o : include/primitives/modmap.h src/primitives/modmap.c include/fails.h src/fails.c include/Machine.h src/Machine.c include/primitives.h src/primitives.c
	gcc $(CFLAGS) -c src/primitives/modmap.c -o lib/liblocalcompilation/obj/primitives/modmap.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/primitives/modstr.o : include/primitives/modstr.h src/primitives/modstr.c include/fails.h src/fails.c include/Machine.h src/Machine.c include/primitives.h src/primitives.c
	gcc $(CFLAGS) -c src/primitives/modstr.c -o lib/liblocalcompilation/obj/primitives/modstr.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)
