LIB_PATH_dmc = lib/libdmc
LIB_INCLUDE_dmc = $(LIB_PATH_dmc)/include

CFLAGS = -Wall -rdynamic
bin/nickleague : src/nickleague.c include/nickleague.h lib/liblocalcompilation/liblocalcompilation.a
	gcc $(CFLAGS) src/nickleague.c -o bin/nickleague \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc) \
		-Llib/liblocalcompilation -llocalcompilation \
		-L$(LIB_PATH_dmc) -ldmc \
		-lgc -lpthread -lm

lib/liblocalcompilation/liblocalcompilation.a : lib/liblocalcompilation/obj/nickleague.o lib/liblocalcompilation/obj/log.o lib/liblocalcompilation/obj/io.o lib/liblocalcompilation/obj/DEFS.o lib/liblocalcompilation/obj/League.o lib/liblocalcompilation/obj/match.o
	ar rcs lib/liblocalcompilation/liblocalcompilation.a lib/liblocalcompilation/obj/nickleague.o lib/liblocalcompilation/obj/log.o lib/liblocalcompilation/obj/io.o lib/liblocalcompilation/obj/DEFS.o lib/liblocalcompilation/obj/League.o lib/liblocalcompilation/obj/match.o

lib/liblocalcompilation/obj/nickleague.o : include/nickleague.h src/nickleague.c include/log.h src/log.c include/io.h src/io.c include/League.h src/League.c
	gcc $(CFLAGS) -c src/nickleague.c -o lib/liblocalcompilation/obj/nickleague.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/log.o : include/log.h src/log.c
	gcc $(CFLAGS) -c src/log.c -o lib/liblocalcompilation/obj/log.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/io.o : include/io.h src/io.c include/DEFS.h src/DEFS.c
	gcc $(CFLAGS) -c src/io.c -o lib/liblocalcompilation/obj/io.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/DEFS.o : include/DEFS.h src/DEFS.c
	gcc $(CFLAGS) -c src/DEFS.c -o lib/liblocalcompilation/obj/DEFS.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/League.o : include/League.h src/League.c include/match.h src/match.c
	gcc $(CFLAGS) -c src/League.c -o lib/liblocalcompilation/obj/League.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/match.o : include/match.h src/match.c
	gcc $(CFLAGS) -c src/match.c -o lib/liblocalcompilation/obj/match.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

