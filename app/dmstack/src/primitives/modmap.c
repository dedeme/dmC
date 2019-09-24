// Copyright 04-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "primitives/modmap.h"
#include "fails.h"
#include "Machine.h"

// Tp<Token>
static char *kvk (Machine *m, Token *tk) {
  if (token_type(tk) != token_LIST) fails_type_in(m, token_LIST, tk);
  // Arr<Token>
  Arr *a = token_list(tk);
  if (arr_size(a) != 2) fails_size_list(m, a, 2);
  Token *tkstr = *arr_start(a);
  if (token_type(tkstr) != token_STRING) fails_type_in(m, token_STRING, tkstr);
  return token_string(tkstr);
}

static void new (Machine *m) {
  machine_push(m, token_new_list(0, arr_new()));
}

static void get (Machine *m) {
  char *key = token_string(machine_pop_exc(m, token_STRING));
  // Arr<Token>
  Arr *a = token_list(machine_pop_exc(m, token_LIST));
  int fn (Token *tk) { return str_eq(kvk(m, tk), key); }
  Token *tk = opt_nget(it_find(it_from(a), (FPRED)fn));
  if (tk) {
    machine_push(m, token_new_list(0, arr_new_from(
      arr_get(token_list(tk), 1), NULL)
    ));
  } else {
    machine_push(m, token_new_list(0, arr_new()));
  }
}

static void haskey (Machine *m) {
  char *key = token_string(machine_pop_exc(m, token_STRING));
  // Arr<Token>
  Arr *a = token_list(machine_pop_exc(m, token_LIST));
  int fn (Token *tk) { return str_eq(kvk(m, tk), key); }
  machine_push(m, token_new_int(0, arr_index(a, (FPRED)fn) != -1));
}

static void putboth (Machine *m, int isplus) {
  Token *tk = machine_pop(m);
  char *key = token_string(machine_pop_exc(m, token_STRING));
  // Arr<Token>
  Arr *a = isplus
    ? token_list(machine_peek_exc(m, token_LIST))
    : token_list(machine_pop_exc(m, token_LIST))
  ;
  int fn (Token *tk) { return str_eq(kvk(m, tk), key); }
  Token *t = opt_nget(it_find(it_from(a), (FPRED)fn));
  if (t)
    arr_set(token_list(t), 1, tk);
  else
    arr_push(a, token_new_list(0, arr_new_from(
      token_new_string(0, key), tk, NULL
    )));
}

static void put (Machine *m) {
  putboth(m, 0);
}

static void putplus (Machine *m) {
  putboth(m, 1);
}

static void mremove (Machine *m) {
  char *key = token_string(machine_pop_exc(m, token_STRING));
  // Arr<Token>
  Arr *a = token_list(machine_pop_exc(m, token_LIST));
  int fn (Token *tk) { return !str_eq(kvk(m, tk), key); }
  arr_filter_in(a, (FPRED)fn);
}

static void keys (Machine *m) {
  // Arr<Token>
  Arr *r = arr_new();
  EACH(token_list(machine_pop_exc(m, token_LIST)), Token, tk) {
    arr_push(r, token_new_string(0, kvk(m, tk)));
  }_EACH
  machine_push(m, token_new_list(0, r));
}

static void values (Machine *m) {
  // Arr<Token>
  Arr *r = arr_new();
  EACH(token_list(machine_pop_exc(m, token_LIST)), Token, tk) {
    if (token_type(tk) != token_LIST) fails_type_in(m, token_LIST, tk);
    // Arr<Token>
    Arr *a = token_list(tk);
    if (arr_size(a) != 2) fails_size_list(m, a, 2);
    arr_push(r, arr_get(a, 1));
  }_EACH
  machine_push(m, token_new_list(0, r));
}

static void sortboth (Machine *m, int (*greater)(Token *e1, Token *e2)) {
  // Arr<Token>
  Arr *a = token_list(machine_pop_exc(m, token_LIST));
  arr_sort(a, (FCMP)greater);
}

static void sort (Machine *m) {
  int greater (Token *e1, Token *e2) {
    return strcmp(kvk(m, e1), kvk(m, e2)) > 0;
  }
  sortboth(m, greater);
}

static void sortlocale (Machine *m) {
  int greater (Token *e1, Token *e2) {
    return strcoll(kvk(m, e1), kvk(m, e2)) > 0;
  }
  sortboth(m, greater);
}

// Resturns Map<primitives_Fn>
Map *modmap_mk (void) {
  // Map<primitives_Fn>
  Map *r = map_new();

  map_put(r, "new", new); // [] - MAP
  map_put(r, "get", get); // [MAP, STRING] - LIST
                          // ([map, key] - [unary list] or [empty list])
  map_put(r, "hasKey", haskey); // [MAP, STRING] - INT
  map_put(r, "put", put); // [MAP - STRING - *] - []
  map_put(r, "put+", putplus); // [MAP - STRING - *] - MAP
  map_put(r, "remove", mremove); // [MAP, STRING] - []
  map_put(r, "keys", keys); // MAP - LIST
  map_put(r, "values", values); // MAP - LIST
  map_put(r, "sort", sort); // MAP - []
  map_put(r, "sortLocale", sortlocale); // MAP - []

  return r;
}


