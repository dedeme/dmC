SO = libkgtk
LIB = kut

C_FILES = $(wildcard src/*.c) $(wildcard src/*/*.c) $(wildcard src/*/*/*.c)
H_FILES = $(wildcard include/*.h) $(wildcard include/*/*.h) $(wildcard include/*/*/*.h)

LIB_PATH = lib/lib$(LIB)
LIB_INCLUDE = lib/lib$(LIB)/include

OBJECTS = $(subst src,obj,$(patsubst %.c,%.o,$(wildcard src/*.c))) \
  $(subst src,obj,$(patsubst %.c,%.o,$(wildcard src/*/*.c))) \
  $(subst src,obj,$(patsubst %.c,%.o,$(wildcard src/*/*/*.c)))

CFLAGS = -Wall -rdynamic -z execstack
FILES = $(C_FILES) $(H_FILES)
obj/%.o : src/%.c include/%.h
	gcc $(CFLAGS) `pkg-config --cflags gtk4` -fPIC -c $< -o $@ -Iinclude \
	-Ilib/libkut/include `pkg-config --libs gtk4`

bin/$(SO).so : $(OBJECTS)
	gcc `pkg-config --cflags gtk4` \
	-z execstack -shared -o bin/$(SO).so $(OBJECTS) \
	`pkg-config --libs gtk4`
