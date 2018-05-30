LIB = dmc

CFLAGS = -Wall -rdynamic
OBJECTS = $(subst src/dmc,obj,$(patsubst %.c,%.o,$(wildcard src/dmc/*.c)))

lib/lib$(LIB).a : $(OBJECTS)
	ar rcs lib/lib$(LIB).a $(OBJECTS)

obj/%.o : src/dmc/%.c include/dmc/%.h
	gcc $(CFLAGS) -c $< -o $@ -Iinclude \
	  -lgc

