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

lib/liblocalcompilation/liblocalcompilation.a : lib/liblocalcompilation/obj/dmstack.o lib/liblocalcompilation/obj/args.o lib/liblocalcompilation/obj/DEFS.o lib/liblocalcompilation/obj/imports.o lib/liblocalcompilation/obj/primitives.o lib/liblocalcompilation/obj/primitives/modglobal.o lib/liblocalcompilation/obj/fails.o lib/liblocalcompilation/obj/Machine.o lib/liblocalcompilation/obj/Reader.o lib/liblocalcompilation/obj/tkreader.o lib/liblocalcompilation/obj/Token.o lib/liblocalcompilation/obj/Symbol.o lib/liblocalcompilation/obj/Heap.o lib/liblocalcompilation/obj/primitives/modglobal0.o lib/liblocalcompilation/obj/primitives/modglobal1.o lib/liblocalcompilation/obj/primitives/modglobal2.o lib/liblocalcompilation/obj/primitives/modstk.o lib/liblocalcompilation/obj/primitives/modblob.o lib/liblocalcompilation/obj/primitives/modint.o lib/liblocalcompilation/obj/primitives/modfloat.o lib/liblocalcompilation/obj/primitives/modmath.o lib/liblocalcompilation/obj/primitives/modlist.o lib/liblocalcompilation/obj/primitives/modjs.o lib/liblocalcompilation/obj/primitives/modwrap.o lib/liblocalcompilation/obj/primitives/modmap.o lib/liblocalcompilation/obj/primitives/modobj.o lib/liblocalcompilation/obj/primitives/modstr.o lib/liblocalcompilation/obj/primitives/modsys.o lib/liblocalcompilation/obj/primitives/modb64.o lib/liblocalcompilation/obj/primitives/modcryp.o lib/liblocalcompilation/obj/primitives/modtime.o lib/liblocalcompilation/obj/primitives/modclock.o lib/liblocalcompilation/obj/primitives/modpath.o lib/liblocalcompilation/obj/primitives/modit.o lib/liblocalcompilation/obj/primitives/modfile.o lib/liblocalcompilation/obj/primitives/modiserver.o lib/liblocalcompilation/obj/Lib.o
	ar rcs lib/liblocalcompilation/liblocalcompilation.a lib/liblocalcompilation/obj/dmstack.o lib/liblocalcompilation/obj/args.o lib/liblocalcompilation/obj/DEFS.o lib/liblocalcompilation/obj/imports.o lib/liblocalcompilation/obj/primitives.o lib/liblocalcompilation/obj/primitives/modglobal.o lib/liblocalcompilation/obj/fails.o lib/liblocalcompilation/obj/Machine.o lib/liblocalcompilation/obj/Reader.o lib/liblocalcompilation/obj/tkreader.o lib/liblocalcompilation/obj/Token.o lib/liblocalcompilation/obj/Symbol.o lib/liblocalcompilation/obj/Heap.o lib/liblocalcompilation/obj/primitives/modglobal0.o lib/liblocalcompilation/obj/primitives/modglobal1.o lib/liblocalcompilation/obj/primitives/modglobal2.o lib/liblocalcompilation/obj/primitives/modstk.o lib/liblocalcompilation/obj/primitives/modblob.o lib/liblocalcompilation/obj/primitives/modint.o lib/liblocalcompilation/obj/primitives/modfloat.o lib/liblocalcompilation/obj/primitives/modmath.o lib/liblocalcompilation/obj/primitives/modlist.o lib/liblocalcompilation/obj/primitives/modjs.o lib/liblocalcompilation/obj/primitives/modwrap.o lib/liblocalcompilation/obj/primitives/modmap.o lib/liblocalcompilation/obj/primitives/modobj.o lib/liblocalcompilation/obj/primitives/modstr.o lib/liblocalcompilation/obj/primitives/modsys.o lib/liblocalcompilation/obj/primitives/modb64.o lib/liblocalcompilation/obj/primitives/modcryp.o lib/liblocalcompilation/obj/primitives/modtime.o lib/liblocalcompilation/obj/primitives/modclock.o lib/liblocalcompilation/obj/primitives/modpath.o lib/liblocalcompilation/obj/primitives/modit.o lib/liblocalcompilation/obj/primitives/modfile.o lib/liblocalcompilation/obj/primitives/modiserver.o lib/liblocalcompilation/obj/Lib.o

lib/liblocalcompilation/obj/dmstack.o : include/dmstack.h src/dmstack.c include/args.h src/args.c include/imports.h src/imports.c include/primitives.h src/primitives.c include/Reader.h src/Reader.c include/Machine.h src/Machine.c include/fails.h src/fails.c include/DEFS.h src/DEFS.c
	gcc $(CFLAGS) -c src/dmstack.c -o lib/liblocalcompilation/obj/dmstack.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/args.o : include/args.h src/args.c include/DEFS.h src/DEFS.c
	gcc $(CFLAGS) -c src/args.c -o lib/liblocalcompilation/obj/args.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/DEFS.o : include/DEFS.h src/DEFS.c
	gcc $(CFLAGS) -c src/DEFS.c -o lib/liblocalcompilation/obj/DEFS.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/imports.o : include/imports.h src/imports.c include/primitives.h src/primitives.c include/Machine.h src/Machine.c include/Lib.h src/Lib.c
	gcc $(CFLAGS) -c src/imports.c -o lib/liblocalcompilation/obj/imports.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/primitives.o : include/primitives.h src/primitives.c include/primitives/modglobal.h src/primitives/modglobal.c include/primitives/modstk.h src/primitives/modstk.c include/primitives/modblob.h src/primitives/modblob.c include/primitives/modint.h src/primitives/modint.c include/primitives/modfloat.h src/primitives/modfloat.c include/primitives/modmath.h src/primitives/modmath.c include/primitives/modlist.h src/primitives/modlist.c include/primitives/modwrap.h src/primitives/modwrap.c include/primitives/modmap.h src/primitives/modmap.c include/primitives/modobj.h src/primitives/modobj.c include/primitives/modstr.h src/primitives/modstr.c include/primitives/modsys.h src/primitives/modsys.c include/primitives/modjs.h src/primitives/modjs.c include/primitives/modb64.h src/primitives/modb64.c include/primitives/modcryp.h src/primitives/modcryp.c include/primitives/modtime.h src/primitives/modtime.c include/primitives/modclock.h src/primitives/modclock.c include/primitives/modpath.h src/primitives/modpath.c include/primitives/modit.h src/primitives/modit.c include/primitives/modfile.h src/primitives/modfile.c include/primitives/modiserver.h src/primitives/modiserver.c include/Machine.h src/Machine.c
	gcc $(CFLAGS) -c src/primitives.c -o lib/liblocalcompilation/obj/primitives.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/primitives/modglobal.o : include/primitives/modglobal.h src/primitives/modglobal.c include/fails.h src/fails.c include/primitives/modglobal0.h src/primitives/modglobal0.c include/primitives/modglobal1.h src/primitives/modglobal1.c include/primitives/modglobal2.h src/primitives/modglobal2.c include/Token.h src/Token.c include/primitives.h src/primitives.c
	gcc $(CFLAGS) -c src/primitives/modglobal.c -o lib/liblocalcompilation/obj/primitives/modglobal.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/fails.o : include/fails.h src/fails.c include/Machine.h src/Machine.c
	gcc $(CFLAGS) -c src/fails.c -o lib/liblocalcompilation/obj/fails.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/Machine.o : include/Machine.h src/Machine.c include/Reader.h src/Reader.c include/imports.h src/imports.c include/primitives.h src/primitives.c include/fails.h src/fails.c include/Heap.h src/Heap.c include/DEFS.h src/DEFS.c
	gcc $(CFLAGS) -c src/Machine.c -o lib/liblocalcompilation/obj/Machine.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/Reader.o : include/Reader.h src/Reader.c include/DEFS.h src/DEFS.c include/tkreader.h src/tkreader.c include/imports.h src/imports.c include/Token.h src/Token.c
	gcc $(CFLAGS) -c src/Reader.c -o lib/liblocalcompilation/obj/Reader.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/tkreader.o : include/tkreader.h src/tkreader.c include/Reader.h src/Reader.c
	gcc $(CFLAGS) -c src/tkreader.c -o lib/liblocalcompilation/obj/tkreader.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/Token.o : include/Token.h src/Token.c include/DEFS.h src/DEFS.c include/Symbol.h src/Symbol.c
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

lib/liblocalcompilation/obj/primitives/modglobal0.o : include/primitives/modglobal0.h src/primitives/modglobal0.c include/fails.h src/fails.c include/Machine.h src/Machine.c
	gcc $(CFLAGS) -c src/primitives/modglobal0.c -o lib/liblocalcompilation/obj/primitives/modglobal0.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/primitives/modglobal1.o : include/primitives/modglobal1.h src/primitives/modglobal1.c include/fails.h src/fails.c include/Machine.h src/Machine.c
	gcc $(CFLAGS) -c src/primitives/modglobal1.c -o lib/liblocalcompilation/obj/primitives/modglobal1.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/primitives/modglobal2.o : include/primitives/modglobal2.h src/primitives/modglobal2.c include/fails.h src/fails.c include/Machine.h src/Machine.c
	gcc $(CFLAGS) -c src/primitives/modglobal2.c -o lib/liblocalcompilation/obj/primitives/modglobal2.o \
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

lib/liblocalcompilation/obj/primitives/modlist.o : include/primitives/modlist.h src/primitives/modlist.c include/fails.h src/fails.c include/Machine.h src/Machine.c include/primitives/modjs.h src/primitives/modjs.c include/DEFS.h src/DEFS.c include/primitives.h src/primitives.c
	gcc $(CFLAGS) -c src/primitives/modlist.c -o lib/liblocalcompilation/obj/primitives/modlist.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/primitives/modjs.o : include/primitives/modjs.h src/primitives/modjs.c include/fails.h src/fails.c include/Machine.h src/Machine.c include/primitives.h src/primitives.c
	gcc $(CFLAGS) -c src/primitives/modjs.c -o lib/liblocalcompilation/obj/primitives/modjs.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/primitives/modwrap.o : include/primitives/modwrap.h src/primitives/modwrap.c include/Machine.h src/Machine.c include/fails.h src/fails.c include/primitives.h src/primitives.c
	gcc $(CFLAGS) -c src/primitives/modwrap.c -o lib/liblocalcompilation/obj/primitives/modwrap.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/primitives/modmap.o : include/primitives/modmap.h src/primitives/modmap.c include/fails.h src/fails.c include/Machine.h src/Machine.c include/primitives/modjs.h src/primitives/modjs.c include/primitives.h src/primitives.c
	gcc $(CFLAGS) -c src/primitives/modmap.c -o lib/liblocalcompilation/obj/primitives/modmap.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/primitives/modobj.o : include/primitives/modobj.h src/primitives/modobj.c include/fails.h src/fails.c include/Machine.h src/Machine.c include/primitives.h src/primitives.c
	gcc $(CFLAGS) -c src/primitives/modobj.c -o lib/liblocalcompilation/obj/primitives/modobj.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/primitives/modstr.o : include/primitives/modstr.h src/primitives/modstr.c include/fails.h src/fails.c include/Machine.h src/Machine.c include/primitives.h src/primitives.c
	gcc $(CFLAGS) -c src/primitives/modstr.c -o lib/liblocalcompilation/obj/primitives/modstr.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/primitives/modsys.o : include/primitives/modsys.h src/primitives/modsys.c include/Machine.h src/Machine.c include/args.h src/args.c include/primitives.h src/primitives.c
	gcc $(CFLAGS) -c src/primitives/modsys.c -o lib/liblocalcompilation/obj/primitives/modsys.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/primitives/modb64.o : include/primitives/modb64.h src/primitives/modb64.c include/Machine.h src/Machine.c include/primitives.h src/primitives.c
	gcc $(CFLAGS) -c src/primitives/modb64.c -o lib/liblocalcompilation/obj/primitives/modb64.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/primitives/modcryp.o : include/primitives/modcryp.h src/primitives/modcryp.c include/Machine.h src/Machine.c include/primitives.h src/primitives.c
	gcc $(CFLAGS) -c src/primitives/modcryp.c -o lib/liblocalcompilation/obj/primitives/modcryp.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/primitives/modtime.o : include/primitives/modtime.h src/primitives/modtime.c include/Machine.h src/Machine.c include/fails.h src/fails.c include/primitives.h src/primitives.c
	gcc $(CFLAGS) -c src/primitives/modtime.c -o lib/liblocalcompilation/obj/primitives/modtime.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/primitives/modclock.o : include/primitives/modclock.h src/primitives/modclock.c include/fails.h src/fails.c include/Machine.h src/Machine.c include/primitives.h src/primitives.c
	gcc $(CFLAGS) -c src/primitives/modclock.c -o lib/liblocalcompilation/obj/primitives/modclock.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/primitives/modpath.o : include/primitives/modpath.h src/primitives/modpath.c include/Machine.h src/Machine.c include/primitives.h src/primitives.c
	gcc $(CFLAGS) -c src/primitives/modpath.c -o lib/liblocalcompilation/obj/primitives/modpath.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/primitives/modit.o : include/primitives/modit.h src/primitives/modit.c include/Machine.h src/Machine.c include/fails.h src/fails.c include/primitives/modlist.h src/primitives/modlist.c include/primitives.h src/primitives.c
	gcc $(CFLAGS) -c src/primitives/modit.c -o lib/liblocalcompilation/obj/primitives/modit.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/primitives/modfile.o : include/primitives/modfile.h src/primitives/modfile.c include/Machine.h src/Machine.c include/fails.h src/fails.c include/primitives.h src/primitives.c
	gcc $(CFLAGS) -c src/primitives/modfile.c -o lib/liblocalcompilation/obj/primitives/modfile.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/primitives/modiserver.o : include/primitives/modiserver.h src/primitives/modiserver.c include/fails.h src/fails.c include/Machine.h src/Machine.c include/primitives.h src/primitives.c
	gcc $(CFLAGS) -c src/primitives/modiserver.c -o lib/liblocalcompilation/obj/primitives/modiserver.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/Lib.o : include/Lib.h src/Lib.c include/Heap.h src/Heap.c
	gcc $(CFLAGS) -c src/Lib.c -o lib/liblocalcompilation/obj/Lib.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

