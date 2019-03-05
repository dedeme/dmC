// Copyright 23-Feb-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "util.h"
#include "DEFS.h"

double util_param(double mx, double mn, double value) {
  return mn + value * (mx - mn);
}

static char *repeat_new(char ch, int n) {
  char *r = calloc(n + 1, 1);
  char *p = r;
  REPEAT(n)
    *p++ = ch;
  _REPEAT
  return r;
}

void util_print_model_name(Model *model) {
  char *name = model_name(model);
  char *decoration = repeat_new('_', strlen(name));
  printf("%s\n%s\n\n", decoration, name);
  free(decoration);
}
