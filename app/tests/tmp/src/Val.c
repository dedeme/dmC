#include <stdio.h>
#include "Val.h"
#include "kut/str.h"

Val fn() {
  Val r = (Val)str_new("abc");
  return r;
}

void pr (Val s_p, Val i_p) {
  char *s = s_p.s;
  int i = i_p.i;
  printf("%s: %i\n", s, i);
}

void val (void) {
  Val x = (Val)12;
  Val y = (Val)3;
  Val sum = (Val)(x.i + y.i);
  printf("%d\n", sum.i);
  puts(fn().s);
  pr((Val)"Value", (Val)32);
}
