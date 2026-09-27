SO = libmkt-202602
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
obj/%.o : src/%.c include/*.h
	if [ ! -e obj ];then mkdir obj;fi
	if [ ! -e obj/model ];then mkdir obj/model;fi
	gcc $(CFLAGS) -fPIC -c $< -o $@ -Iinclude \
	-Ilib/libkut/include

bin/$(SO).so : $(OBJECTS)
	gcc -z execstack -shared -o bin/$(SO).so $(OBJECTS)
