LIB_PATH_dmc = lib/libdmc
LIB_INCLUDE_dmc = $(LIB_PATH_dmc)/include

CFLAGS = -Wall -rdynamic
bin/makemake : src/makemake.c include/makemake.h lib/liblocalcompilation/liblocalcompilation.a
	gcc $(CFLAGS) src/makemake.c -o bin/makemake \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc) \
		-Llib/liblocalcompilation -llocalcompilation \
		-L$(LIB_PATH_dmc) -ldmc \
		-lgc -lpthread -lm

lib/liblocalcompilation/liblocalcompilation.a : lib/liblocalcompilation/obj/makemake.o lib/liblocalcompilation/obj/DEFS.o lib/liblocalcompilation/obj/reader.o lib/liblocalcompilation/obj/writer.o
	ar rcs lib/liblocalcompilation/liblocalcompilation.a lib/liblocalcompilation/obj/makemake.o lib/liblocalcompilation/obj/DEFS.o lib/liblocalcompilation/obj/reader.o lib/liblocalcompilation/obj/writer.o

lib/liblocalcompilation/obj/makemake.o : include/makemake.h src/makemake.c include/DEFS.h src/DEFS.c include/reader.h src/reader.c include/writer.h src/writer.c
	gcc $(CFLAGS) -c src/makemake.c -o lib/liblocalcompilation/obj/makemake.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/DEFS.o : include/DEFS.h src/DEFS.c
	gcc $(CFLAGS) -c src/DEFS.c -o lib/liblocalcompilation/obj/DEFS.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/reader.o : include/reader.h src/reader.c include/DEFS.h src/DEFS.c
	gcc $(CFLAGS) -c src/reader.c -o lib/liblocalcompilation/obj/reader.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/writer.o : include/writer.h src/writer.c include/DEFS.h src/DEFS.c
	gcc $(CFLAGS) -c src/writer.c -o lib/liblocalcompilation/obj/writer.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

