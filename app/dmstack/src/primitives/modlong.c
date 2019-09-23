// Copyright 20-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "primitives/modlong.h"
#include "Machine.h"
#include "fails.h"


static union {
  long l;
  int is[2];
} lg;

long modlong_to_long (Machine *m, Token *tk) {
  if (token_type(tk) != token_LIST) fails_type_in(m, token_LIST, tk);
  // Arr<Token>
  Arr *a = token_list(tk);
  if (arr_size(a) != 2) fails_size_list(m, a, 2);
  Token *i1 = *arr_start(a);
  Token *i2 = *(arr_start(a) + 1);
  if (token_type(i1) != token_INT) fails_type_in(m, token_INT, i1);
  if (token_type(i2) != token_INT) fails_type_in(m, token_INT, i2);
  int n1 = token_int(i1);
  int n2 = token_int(i2);
  #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    lg.is[0] = n2; lg.is[1] = n1;
  #else
    lg.is[1] = n2; lg.is[0] = n1;
  #endif
  return lg.l;
}

Token *modlong_from_long (long n) {
  lg.l = n;
  #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    int i0 = lg.is[1]; int i1 = lg.is[0];
  #else
    int i0 = lg.is[0]; int i1 = lg.is[1];
  #endif
  return token_new_list(0, arr_new_from(
    token_new_int(0, i0), token_new_int(0, i1), NULL
  ));
}

void *modlong_to_pointer (Machine *m, char *type, Token *tk) {
  if (token_type(tk) != token_LIST) fails_type_in(m, token_LIST, tk);
  // Arr<Token>
  Arr *a = token_list(tk);
  if (arr_size(a) != 2) fails_size_list(m, a, 2);
  Token *sym = *arr_start(a);
  Token *n = *(arr_start(a) + 1);
  if (token_type(sym) != token_SYMBOL) fails_type_in(m, token_SYMBOL, sym);
  char *id = symbol_id(token_symbol(sym));
  if (!str_eq(id, type))
    machine_fail(m, str_f(
      "Expected pointer of type '%s', found '%s'", type, id
    ));
  return (void *)modlong_to_long(m, n);
}

Token *modlong_from_pointer (char *type, void *p) {
  return token_new_list(0, arr_new_from(
    token_new_symbol(0, symbol_new(type)),
    modlong_from_long((long) p),
    NULL
  ));
}

static void new (Machine *m) {
  int i2 = token_int(machine_pop_exc(m, token_INT));
  int i1 = token_int(machine_pop_exc(m, token_INT));
  machine_push(m, token_new_list(0, arr_new_from(
    token_new_int(0, i1), token_new_int(0, i2), NULL
  )));
}

static void toint (Machine *m) {
  long n = modlong_to_long(m, machine_pop(m));
  machine_push(m, token_new_int(0, (int)n));
}

static void fromint (Machine *m) {
  int n = token_int(machine_pop_exc(m, token_INT));
  machine_push(m, modlong_from_long(n));
}

static void add (Machine *m) {
  long n2 = modlong_to_long(m, machine_pop(m));
  long n1 = modlong_to_long(m, machine_pop(m));
  machine_push(m, modlong_from_long(n1 + n2));
}

static void sub (Machine *m) {
  long n2 = modlong_to_long(m, machine_pop(m));
  long n1 = modlong_to_long(m, machine_pop(m));
  machine_push(m, modlong_from_long(n1 - n2));
}

static void mul (Machine *m) {
  long n2 = modlong_to_long(m, machine_pop(m));
  long n1 = modlong_to_long(m, machine_pop(m));
  machine_push(m, modlong_from_long(n1 * n2));
}

static void sdiv (Machine *m) {
  long n2 = modlong_to_long(m, machine_pop(m));
  long n1 = modlong_to_long(m, machine_pop(m));
  machine_push(m, modlong_from_long(n1 / n2));
}

static void mod (Machine *m) {
  long n2 = modlong_to_long(m, machine_pop(m));
  long n1 = modlong_to_long(m, machine_pop(m));
  machine_push(m, modlong_from_long(n1 % n2));
}

static void greater (Machine *m) {
  long n2 = modlong_to_long(m, machine_pop(m));
  long n1 = modlong_to_long(m, machine_pop(m));
  machine_push(m, token_new_int(0, n1 > n2));
}

static void greater_eq (Machine *m) {
  long n2 = modlong_to_long(m, machine_pop(m));
  long n1 = modlong_to_long(m, machine_pop(m));
  machine_push(m, token_new_int(0, n1 >= n2));
}

static void less (Machine *m) {
  long n2 = modlong_to_long(m, machine_pop(m));
  long n1 = modlong_to_long(m, machine_pop(m));
  machine_push(m, token_new_int(0, n1 < n2));
}

static void less_eq (Machine *m) {
  long n2 = modlong_to_long(m, machine_pop(m));
  long n1 = modlong_to_long(m, machine_pop(m));
  machine_push(m, token_new_int(0, n1 <= n2));
}

static void add2 (Machine *m) {
  long n2 = token_int(machine_pop_exc(m, token_INT));
  long n1 = modlong_to_long(m, machine_pop(m));
  machine_push(m, modlong_from_long(n1 + n2));
}

static void sub2 (Machine *m) {
  long n2 = token_int(machine_pop_exc(m, token_INT));
  long n1 = modlong_to_long(m, machine_pop(m));
  machine_push(m, modlong_from_long(n1 - n2));
}

// Resturns Map<primitives_Fn>
Map *modlong_mk (void) {
  // Map<primitives_Fn>
  Map *r = map_new();

  map_put(r, "new", new); // [INT - INT] - LIST
  map_put(r, "toInt", toint); // LIST - INT
  map_put(r, "fromInt", fromint); // INT - LIST

  map_put(r, "+", add); // [LIST - LIST] - LIST
  map_put(r, "-", sub); // [LIST - LIST] - LIST
  map_put(r, "*", mul); // [LIST - LIST] - LIST
  map_put(r, "/", sdiv); // [LIST - LIST] - LIST
  map_put(r, "%", mod); // [LIST - LIST] - LIST

  map_put(r, ">", greater); // [LIST - LIST] - INT
  map_put(r, ">=", greater_eq); // [LIST - LIST] - INT
  map_put(r, "<", less); // [LIST - LIST] - INT
  map_put(r, "<=", less_eq); // [LIST - LIST] - INT

  map_put(r, "+Int", add2); // [LIST - INT] - LIST
  map_put(r, "-Int", sub2); // [LIST - INT] - LIST

  return r;
}



