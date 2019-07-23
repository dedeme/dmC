LIB_PATH_dmc = lib/libdmc
LIB_INCLUDE_dmc = $(LIB_PATH_dmc)/include

CFLAGS = -Wall -rdynamic
bin/sciweb : src/sciweb.c include/sciweb.h lib/liblocalcompilation/liblocalcompilation.a
	gcc $(CFLAGS) src/sciweb.c -o bin/sciweb \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc) \
		-Llib/liblocalcompilation -llocalcompilation \
		-L$(LIB_PATH_dmc) -ldmc \
		-lgc -lpthread -lm

lib/liblocalcompilation/liblocalcompilation.a : lib/liblocalcompilation/obj/sciweb.o lib/liblocalcompilation/obj/help.o lib/liblocalcompilation/obj/book.o lib/liblocalcompilation/obj/tpls/book_index.o lib/liblocalcompilation/obj/tpls/book_geany.o lib/liblocalcompilation/obj/tpls/book_js.o lib/liblocalcompilation/obj/update.o lib/liblocalcompilation/obj/Conf.o lib/liblocalcompilation/obj/reader.o lib/liblocalcompilation/obj/tpls/unit.o lib/liblocalcompilation/obj/tpls/exercise.o
	ar rcs lib/liblocalcompilation/liblocalcompilation.a lib/liblocalcompilation/obj/sciweb.o lib/liblocalcompilation/obj/help.o lib/liblocalcompilation/obj/book.o lib/liblocalcompilation/obj/tpls/book_index.o lib/liblocalcompilation/obj/tpls/book_geany.o lib/liblocalcompilation/obj/tpls/book_js.o lib/liblocalcompilation/obj/update.o lib/liblocalcompilation/obj/Conf.o lib/liblocalcompilation/obj/reader.o lib/liblocalcompilation/obj/tpls/unit.o lib/liblocalcompilation/obj/tpls/exercise.o

lib/liblocalcompilation/obj/sciweb.o : include/sciweb.h src/sciweb.c include/help.h src/help.c include/book.h src/book.c include/update.h src/update.c
	gcc $(CFLAGS) -c src/sciweb.c -o lib/liblocalcompilation/obj/sciweb.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/help.o : include/help.h src/help.c
	gcc $(CFLAGS) -c src/help.c -o lib/liblocalcompilation/obj/help.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/book.o : include/book.h src/book.c include/tpls/book_index.h src/tpls/book_index.c include/tpls/book_geany.h src/tpls/book_geany.c include/tpls/book_js.h src/tpls/book_js.c
	gcc $(CFLAGS) -c src/book.c -o lib/liblocalcompilation/obj/book.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/tpls/book_index.o : include/tpls/book_index.h src/tpls/book_index.c
	gcc $(CFLAGS) -c src/tpls/book_index.c -o lib/liblocalcompilation/obj/tpls/book_index.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/tpls/book_geany.o : include/tpls/book_geany.h src/tpls/book_geany.c
	gcc $(CFLAGS) -c src/tpls/book_geany.c -o lib/liblocalcompilation/obj/tpls/book_geany.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/tpls/book_js.o : include/tpls/book_js.h src/tpls/book_js.c
	gcc $(CFLAGS) -c src/tpls/book_js.c -o lib/liblocalcompilation/obj/tpls/book_js.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/update.o : include/update.h src/update.c include/Conf.h src/Conf.c include/reader.h src/reader.c include/tpls/unit.h src/tpls/unit.c include/tpls/exercise.h src/tpls/exercise.c
	gcc $(CFLAGS) -c src/update.c -o lib/liblocalcompilation/obj/update.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/Conf.o : include/Conf.h src/Conf.c
	gcc $(CFLAGS) -c src/Conf.c -o lib/liblocalcompilation/obj/Conf.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/reader.o : include/reader.h src/reader.c include/Conf.h src/Conf.c
	gcc $(CFLAGS) -c src/reader.c -o lib/liblocalcompilation/obj/reader.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/tpls/unit.o : include/tpls/unit.h src/tpls/unit.c
	gcc $(CFLAGS) -c src/tpls/unit.c -o lib/liblocalcompilation/obj/tpls/unit.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

lib/liblocalcompilation/obj/tpls/exercise.o : include/tpls/exercise.h src/tpls/exercise.c
	gcc $(CFLAGS) -c src/tpls/exercise.c -o lib/liblocalcompilation/obj/tpls/exercise.o \
		-Iinclude \
		-I$(LIB_INCLUDE_dmc)

