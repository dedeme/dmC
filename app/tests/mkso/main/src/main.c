// Copyright 12-Jul-2024 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

typedef union tval_Tval {
  int64_t i;
  double f;
  void *o;
} Tval;

typedef Tval (*F11) (Tval);
typedef void (*F10) (Tval);

int
main(int argc, char **argv)
{
  test();

  void *handle;
  char *error;

  handle = dlopen("/dm/dmC/app/tests/mkso/lib/bin/mksolib.so", RTLD_LAZY);
  if (!handle) {
      fprintf(stderr, "%s\n", dlerror());
      exit(EXIT_FAILURE);
  }

  dlerror();    /* Clear any existing error */

  /* Writing: cosine = (double (*)(double)) dlsym(handle, "cos");
     would seem more natural, but the C99 standard leaves
     casting from "void *" to a function pointer undefined.
     The assignment used below is the POSIX.1-2003 (Technical
     Corrigendum 1) workaround; see the Rationale for the
     POSIX specification of dlsym(). */


  void (*setSum) (Tval);
  *(void **) (&setSum) = dlsym(handle, "setSummand");
  Tval (*add) (Tval);
  *(void **) (&add) = dlsym(handle, "add");

  if ((error = dlerror()) != NULL)  {
      fprintf(stderr, "-->%s\n", error);
      exit(EXIT_FAILURE);
  }

  Tval v;
  v.i = 5;
  setSum(v);

  v.i = 7;
  printf("5 + 7 = %ld\n", add(v).i);

  exit(EXIT_SUCCESS);
}
