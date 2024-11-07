#include <stdio.h>
extern int b;
int btake (int n);
void bmain ();
extern int a;
Tvalue take (int n);

// ---

int b;

int btake (int n) {
  return n + 34;
}

void bmain (void) {
  int in () {
    int(*x)(int n) = btake;
    return x(1211);
  }
b = in(5);
}
