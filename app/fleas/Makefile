PRG = fleas

C_FILES = $(wildcard src/*.c) \
          $(wildcard src/order/*.c) \
          $(wildcard src/tests/*.c) \
          $(wildcard src/families/*.c)
H_FILES = $(wildcard include/*.h) \
          $(wildcard include/order/*.h) \
          $(wildcard include/tests/*.h) \
          $(wildcard include/families/*.h)

CFLAGS = -Wall -rdynamic
FILES = $(C_FILES) $(H_FILES)
bin/$(PRG): $(FILES) lib/libdmc/libdmc.a lib/libmarket/libmarket.a
	gcc $(CFLAGS) $(C_FILES) -o bin/$(PRG) \
	  -Iinclude \
	  -Ilib/libdmc/include -Llib/libdmc -ldmc \
	  -Ilib/libmarket/include -Llib/libmarket -lmarket \
		-lgc -lm
