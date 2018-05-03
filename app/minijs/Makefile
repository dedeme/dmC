PRG = minijs

C_FILES = $(wildcard src/*.c) \
          $(wildcard src/ast/*.c) \
          $(wildcard src/builtin/*.c) \
          $(wildcard src/lexer/*.c) \
          $(wildcard src/parser/*.c) \
          $(wildcard src/js/*.c)
H_FILES = $(wildcard include/*.h) \
          $(wildcard include/ast/*.h) \
          $(wildcard include/builtin/*.h) \
          $(wildcard include/lexer/*.h) \
          $(wildcard include/parser/*.h) \
          $(wildcard include/js/*.h)

CFLAGS = -Wall -rdynamic
FILES = $(C_FILES) $(H_FILES)
bin/$(PRG): $(FILES) lib/libdmc/libdmc.a
	gcc $(CFLAGS) $(C_FILES) -o bin/$(PRG) \
	  -Iinclude \
	  -Ilib/libdmc/include -Llib/libdmc -ldmc \
	  -lgc
