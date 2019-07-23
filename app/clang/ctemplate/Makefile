LIB_PATH_dmc = lib/libdmc
LIB_INCLUDE_dmc = $(LIB_PATH_dmc)/include

CFLAGS = -Wall -rdynamic
bin/ctemplate : src/ctemplate.c include/ctemplate.h lib/liblocalcompilation/liblocalcompilation.a
	gcc $(CFLAGS) src/ctemplate.c -o bin/ctemplate \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc) \
		-Llib/liblocalcompilation -llocalcompilation \
		-L$(LIB_PATH_dmc) -ldmc \
		-lgc -lpthread -lm

lib/liblocalcompilation/liblocalcompilation.a : lib/liblocalcompilation/obj/ctemplate.o lib/liblocalcompilation/obj/reader.o lib/liblocalcompilation/obj/Tpl.o lib/liblocalcompilation/obj/DEFS.o lib/liblocalcompilation/obj/Log.o lib/liblocalcompilation/obj/writer.o
	ar rcs lib/liblocalcompilation/liblocalcompilation.a lib/liblocalcompilation/obj/ctemplate.o lib/liblocalcompilation/obj/reader.o lib/liblocalcompilation/obj/Tpl.o lib/liblocalcompilation/obj/DEFS.o lib/liblocalcompilation/obj/Log.o lib/liblocalcompilation/obj/writer.o

lib/liblocalcompilation/obj/ctemplate.o : include/ctemplate.h src/ctemplate.c include/reader.h src/reader.c include/writer.h src/writer.c
	gcc $(CFLAGS) -c src/ctemplate.c -o lib/liblocalcompilation/obj/ctemplate.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/reader.o : include/reader.h src/reader.c include/Tpl.h src/Tpl.c include/Log.h src/Log.c
	gcc $(CFLAGS) -c src/reader.c -o lib/liblocalcompilation/obj/reader.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/Tpl.o : include/Tpl.h src/Tpl.c include/DEFS.h src/DEFS.c include/Log.h src/Log.c
	gcc $(CFLAGS) -c src/Tpl.c -o lib/liblocalcompilation/obj/Tpl.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/DEFS.o : include/DEFS.h src/DEFS.c
	gcc $(CFLAGS) -c src/DEFS.c -o lib/liblocalcompilation/obj/DEFS.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/Log.o : include/Log.h src/Log.c
	gcc $(CFLAGS) -c src/Log.c -o lib/liblocalcompilation/obj/Log.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/writer.o : include/writer.h src/writer.c include/Log.h src/Log.c include/Tpl.h src/Tpl.c
	gcc $(CFLAGS) -c src/writer.c -o lib/liblocalcompilation/obj/writer.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

