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

static int elements (Arr *a) {
  int r = arr_size(a);
  EACH(a, Exp, e) {
    if (exp_is_array(e)) r += elements(exp_get_array(e)) - 1;
  }_EACH
  return r;
}

#define tarr_FROM(A, V) \
  int size = elements(a); \
  int bf_size = size < 15 ? 15 : size; \
  A *this = MALLOC(A); \
  this->begin = ATOMIC(sizeof(V) * bf_size); \
  this->end = this->begin + size; \
  this->bf = this->begin + bf_size;

// a is Arr<Exp(int)>
static int64_t *addi (int64_t *p, Arr *a) {
  EACH(a, Exp, e) {
    if (exp_is_int(e)) *p++ = exp_get_int(e);
    else if (exp_is_array(e)) p = addi(p, exp_get_array(e));
    else
      EXC_KUT(str_f(
        "Expected array of type 'I' has values whose type is not 'int': %s",
        exp_to_js(e)
      ));
  }_EACH
  return p;
}

// a is Arr<Exp(int)>
Arri *tarr_fromi (Arr *a) {
  tarr_FROM(Arri, int64_t)
  addi(this->begin, a);
  return this;
}

// a is Arr<Exp(float)>
static double *addf (double *p, Arr *a) {
  EACH(a, Exp, e) {
    if (exp_is_float(e)) *p++ = exp_get_float(e);
    else if (exp_is_array(e)) p = addf(p, exp_get_array(e));
    else
      EXC_KUT(str_f(
        "Expected array of type 'F' has values whose type is not 'float': %s",
        exp_to_js(e)
      ));
  }_EACH
  return p;
}

// a is Arr<Exp(float)>
Arrf *tarr_fromf (Arr *a) {
  tarr_FROM(Arrf, double)
  addf(this->begin, a);
  return this;
}

// a is Arr<Exp(string)>
static char **adds (char **p, Arr *a) {
  EACH(a, Exp, e) {
    if (exp_is_string(e)) *p++ = exp_get_string(e);
    else if (exp_is_array(e)) p = adds(p, exp_get_array(e));
    else
      EXC_KUT(str_f(
        "Expected array of type 'S' has values whose type is not 'string': %s",
        exp_to_js(e)
      ));
  }_EACH
  return p;
}

// a is Arr<Exp(string)>
Arrs *tarr_froms (Arr *a) {
  tarr_FROM(Arrs, char*)
  adds(this->begin, a);
  return this;
}

#undef tarr_FROM

Arrs *tarr_fromd (Map *m) {
  // <Kv<Exp>>
  Arr *a = (Arr *)m;
  int size = arr_size(a) * 2;
  int bf_size = size < 15 ? 15 : size;
  Arrs *this = MALLOC(Arrs);
  this->begin = ATOMIC(sizeof(char *) * bf_size);
  this->end = this->begin + size;
  this->bf = this->begin + bf_size;
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

