// Copyright 21Aug-2024 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "DEFS.h"
#include "typed/tarr.h"

struct tarr_Arri {
  int64_t *begin;
  int64_t *end;
  int64_t *bf;
};
struct tarr_Arrf {
  double *begin;
  double *end;
  double *bf;
};
struct tarr_Arrs {
  char **begin;
  char **end;
  char **bf;
};

#define tarr_FROM(A, V) \
  int size = arr_size(a); \
  A *this = MALLOC(A); \
  this->begin = ATOMIC(sizeof(V) * size); \
  this->end = this->begin + size; \
  this->bf = this->end;

// a is Arr<Exp(int)>
Arri *tarr_fromi (Arr *a) {
  tarr_FROM(Arri, int64_t)
  int64_t *p = this->begin;
  EACH(a, Exp, e) {
    if (!exp_is_int(e))
      EXC_KUT(str_f(
        "Expected array of type 'I' has values whose type is not 'int': %s",
        exp_to_js(e)
      ));
    *p++ = exp_get_int(e);
  }_EACH
  return this;
}

Arrf *tarr_fromf (Arr *a) {
  tarr_FROM(Arrf, double)
  double *p = this->begin;
  EACH(a, Exp, e) {
    if (!exp_is_float(e))
      EXC_KUT(str_f(
        "Expected array of type 'F' has values whose type is not 'float': %s",
        exp_to_js(e)
      ));
    *p++ = exp_get_float(e);
  }_EACH
  return this;
}

Arrs *tarr_froms (Arr *a) {
  tarr_FROM(Arrs, char*)
  char **p = this->begin;
  EACH(a, Exp, e) {
    if (!exp_is_string(e))
      EXC_KUT(str_f(
        "Expected array of type 'S' has values whose type is not 'string': %s",
        exp_to_js(e)
      ));
    *p++ = exp_get_string(e);
  }_EACH
  return this;
}

#undef tarr_FROM

Arrs *tarr_fromd (Map *m) {
  // <Kv<Exp>>
  Arr *a = (Arr *)m;
  int size = arr_size(a) * 2;
  Arrs *this = MALLOC(Arrs);
  this->begin = ATOMIC(sizeof(char *) * size);
  this->end = this->begin + size;
  this->bf = this->end;
  char **p = this->begin;
  EACH(a, Kv, kv) {
    *p++ = kv_key(kv);
    Exp *e = kv_value(kv);
    if (!exp_is_string(e))
      EXC_KUT(str_f(
        "Expected dictionary has values whose type is not 'string': %s",
        exp_to_js(e)
      ));
    *p++ = exp_get_string(e);
  }_EACH
  return this;
}

// <Exp>
Arr *tarr_to_arri (Arri *this) {
  Arr *a = arr_new();
  int64_t *p = this->begin;
  int64_t *end = this->end;
  while (p < end) arr_push(a, exp_int(*p++));
  return a;
}

// <Exp>
Arr *tarr_to_arrf (Arrf *this) {
  // <Exp>
  Arr *a = arr_new();
  double *p = this->begin;
  double *end = this->end;
  while (p < end) arr_push(a, exp_float(*p++));
  return a;
}

// <Exp>
Arr *tarr_to_arrs (Arrs *this) {
  // <Exp>
  Arr *a = arr_new();
  char **p = this->begin;
  char **end = this->end;
  while (p < end) {
    char *s = *p++;
    arr_push(a, exp_string(str_new(s)));
  }
  return a;
}

// <Exp>
Map *tarr_to_arrd (Arrs *this) {
  // <Kv>
  Arr *a = arr_new();
  char **p = this->begin;
  char **end = this->end;
  while (p < end) {
    char *k = *p++;
    char *v = *p++;
    arr_push(a, kv_new(str_new(k), exp_string(str_new(v))));
  }
  return (Map *)a;
}

