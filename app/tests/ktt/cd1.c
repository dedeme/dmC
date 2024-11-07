#include <stdio.h>
#include <stdint.h>
typedef union Tvalue {
  int b;
  int64_t i;
} Tvalue;
extern Tvalue b;
int btake (Tvalue n);
void bmain ();
extern Tvalue a;
Tvalue take (Tvalue n);

// ----

Tvalue a;

Tvalue take (Tvalue n) {
  return (Tvalue)(n.i + ((Tvalue)(int64_t)4).i);
}

void main (void) {
  bmain();
  a = take((Tvalue)(((Tvalue)(int64_t)5).i + take(b).i));
  printf("%ld\n", a.i);
}
