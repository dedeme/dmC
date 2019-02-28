// Copyright 23-Feb-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "Flea.h"

/* .
-Flea: serial
  date: char *
  cycle: char *
  idn: char *
  gen: Gen *
*/
/*--*/

struct flea_Flea {
  char *date;
  char *cycle;
  char *idn;
  Gen *gen;
};

Flea *_flea_new(
  char *date,
  char *cycle,
  char *idn,
  Gen *gen
) {
  Flea *this = malloc(sizeof(Flea));
  this->date = date;
  this->cycle = cycle;
  this->idn = idn;
  this->gen = gen;
  return this;
}

void flea_free(Flea *this) {
  free(this->date);
  free(this->cycle);
  free(this->idn);
  gen_free(this->gen);
  free(this);
};

char *flea_date(Flea *this) {
  return this->date;
}

char *flea_cycle(Flea *this) {
  return this->cycle;
}

char *flea_idn(Flea *this) {
  return this->idn;
}

Gen *flea_gen(Flea *this) {
  return this->gen;
}

Js *flea_to_js_new(Flea *this) {
  // Arr[Js]
  Arr *a = arr_new(free);
  arr_push(a, js_ws_new(this->date));
  arr_push(a, js_ws_new(this->cycle));
  arr_push(a, js_ws_new(this->idn));
  arr_push(a, gen_to_js_new(this->gen));
  Js *r = js_wa_new(a);
  arr_free(a);
  return r;
}

Flea *flea_from_js_new(Js *js) {
  // Arr[Js]
  Arr *a = js_ra_new(js);
  int i = 0;
  char *date = js_rs_new(arr_get(a, i++));
  char *cycle = js_rs_new(arr_get(a, i++));
  char *idn = js_rs_new(arr_get(a, i++));
  Gen *gen = gen_from_js_new(arr_get(a, i++));
  arr_free(a);
  return _flea_new(date, cycle, idn, gen);
}

/*--*/

static Flea *__flea_new(
  char *date,
  char *cycle,
  char *idn,
  Gen *gen
) {
  return _flea_new(str_new(date),str_new(cycle), str_new(idn), gen);
}

Flea *flea_new(
  char *date,
  char *cycle,
  char *idn,
  int n
){
  return __flea_new(date, cycle, idn, gen_new(n));
}

Flea *flea_mutate_new(
  Flea *this,
  char *date,
  char *cycle,
  char *idn
) {
  return __flea_new(date, cycle, idn, gen_mutate_new(this->gen));
}

Flea *flea_copy_new(Flea *f) {
  return __flea_new(
    f->date, f->cycle, f->idn, gen_copy_new(f->gen)
  );
}
