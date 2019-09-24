// Copyright 04-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "primitives/modlist.h"
#include "fails.h"
#include "Machine.h"
#include "DEFS.h"

// Returns Arr<Token>
static Arr *getlist (Machine *m) {
  return token_list(machine_pop_exc(m, token_LIST));
}

// Returns Arr<Token>
static Arr *peeklist (Machine *m) {
  return token_list(machine_peek_exc(m, token_LIST));
}

// a is Arr<Tokeb>
static void pushlist (Machine *m, Arr *a) {
  machine_push(m, token_new_list(0, a));
}

static int getint (Machine *m) {
  return token_int(machine_pop_exc(m, token_INT));
}

static void new (Machine *m) {
  pushlist(m, arr_new());
}

static void unary (Machine *m) {
  // Arr<Token>
  Arr *a = arr_new();
  arr_push(a, machine_pop(m));
  pushlist(m, a);
}

static void push (Machine *m) {
  Token *tk = machine_pop(m);
  // Arr<Token>
  Arr *a = getlist(m);
  arr_push(a, tk);
}

static void pushplus (Machine *m) {
  Token *tk = machine_pop(m);
  // Arr<Token>
  Arr *a = peeklist(m);
  arr_push(a, tk);
}

static void pop (Machine *m) {
  // Arr<Token>
  Arr *a = getlist(m);
  if (!arr_size(a)) machine_fail(m, "List is empty");
  machine_push(m, arr_pop(a));
}

static void peek (Machine *m) {
  // Arr<Token>
  Arr *a = getlist(m);
  if (!arr_size(a)) machine_fail(m, "List is empty");
  machine_push(m, arr_peek(a));
}

static void push0 (Machine *m) {
  Token *tk = machine_pop(m);
  // Arr<Token>
  Arr *a = getlist(m);
  arr_insert(a, 0, tk);
}

static void push0plus (Machine *m) {
  Token *tk = machine_pop(m);
  // Arr<Token>
  Arr *a = peeklist(m);
  arr_insert(a, 0, tk);
}

static void pop0 (Machine *m) {
  // Arr<Token>
  Arr *a = getlist(m);
  if (!arr_size(a)) machine_fail(m, "List is empty");
  Token *tk = arr_get(a, 0);
  arr_remove(a, 0);
  machine_push(m, tk);
}

static void peek0 (Machine *m) {
  // Arr<Token>
  Arr *a = getlist(m);
  if (!arr_size(a)) machine_fail(m, "List is empty");
  machine_push(m, arr_get(a, 0));
}

static void insert (Machine *m) {
  Token *tk = machine_pop(m);
  int ix = getint(m);
  // Arr<Token>
  Arr *a = getlist(m);
  fails_check_range(m, 0, arr_size(a), ix);
  arr_insert(a, ix, tk);
}

static void insertList (Machine *m) {
  // Arr<Token>
  Arr *add = getlist(m);
  int ix = getint(m);
  // Arr<Token>
  Arr *a = getlist(m);
  fails_check_range(m, 0, arr_size(a), ix);
  arr_insert_arr(a, ix, add);
}

static void lremove (Machine *m) {
  int ix = getint(m);
  // Arr<Token>
  Arr *a = getlist(m);
  fails_check_range(m, 0, arr_size(a) - 1, ix);
  arr_remove(a, ix);
}

static void removeRange (Machine *m) {
  int end = getint(m);
  int begin = getint(m);
  // Arr<Token>
  Arr *a = getlist(m);
  fails_check_range(m, 0, arr_size(a), begin);
  fails_check_range(m, begin, arr_size(a), end);
  arr_remove_range(a, begin, end);
}

static void clear (Machine *m) {
  arr_clear(getlist(m));
}

static void reverse (Machine *m) {
  // Arr<Token>
  Arr *a = getlist(m);
  arr_reverse(a);
}

static void shuffle (Machine *m) {
  // Arr<Token>
  Arr *a = getlist(m);
  arr_shuffle(a);
}

static void sort (Machine *m) {
  // Arr<Token>
  Token *prg = machine_pop_exc(m, token_LIST);
  int fn (Token *e1, Token *e2) {
    machine_push(m, e1);
    machine_push(m, e2);
    machine_process("", machine_pmachines(m), prg);
    return getint(m);
  }
  // Arr<Token>
  Arr *a = getlist(m);
  arr_sort(a, (FCMP)fn);
}

static void duplicates (Machine *m) {
  // Arr<Token>
  Token *prg = machine_pop_exc(m, token_LIST);
  int fn (Token *e1, Token *e2) {
    machine_push(m, e1);
    machine_push(m, e2);
    machine_process("", machine_pmachines(m), prg);
    return getint(m);
  }
  // Arr<Token>
  Arr *a = getlist(m);
  // Tp<Arr<Token>>
  Tp *dr = arr_duplicates(a, (FCMP) fn);
  // Arr<Token>
  Arr *r = arr_new();
  arr_push(r, token_new_list(0, tp_e1(dr)));
  arr_push(r, token_new_list(0, tp_e2(dr)));
  pushlist(m, r);
}

static void all (Machine *m) {

  // Arr<Token>
  Token *prg = machine_pop_exc(m, token_LIST);
  int fn (Token *e) {
    machine_push(m, e);
    machine_process("", machine_pmachines(m), prg);
    return getint(m);
  }
  // Arr<Token>
  Arr *a = getlist(m);
  machine_push(m, token_new_int(0, arr_all(a, (FPRED)fn)));
}

static void any (Machine *m) {
  // Arr<Token>
  Token *prg = machine_pop_exc(m, token_LIST);
  int fn (Token *e) {
    machine_push(m, e);
    machine_process("", machine_pmachines(m), prg);
    return getint(m);
  }
  // Arr<Token>
  Arr *a = getlist(m);
  machine_push(m, token_new_int(0, arr_any(a, (FPRED)fn)));
}

static void each (Machine *m) {
  // Arr<Token>
  Token *prg = machine_pop_exc(m, token_LIST);
  void fn (Token *e) {
    machine_push(m, e);
    machine_process("", machine_pmachines(m), prg);
  }
  // Arr<Token>
  Arr *a = getlist(m);
  it_each(it_from(a), (FPROC)fn);
}

static void eachix (Machine *m) {
  // Arr<Token>
  Token *prg = machine_pop_exc(m, token_LIST);
  int c = 0;
  void fn (Token *e) {
    machine_push(m, e);
    machine_push(m, token_new_int(0, c++));
    machine_process("", machine_pmachines(m), prg);
  }
  // Arr<Token>
  Arr *a = getlist(m);
  it_each(it_from(a), (FPROC)fn);
}

static void eq (Machine *m) {
  // Arr<Token>
  Token *prg = machine_pop_exc(m, token_LIST);
  int fn (Token *e1, Token *e2) {
    machine_push(m, e1);
    machine_push(m, e2);
    machine_process("", machine_pmachines(m), prg);
    return getint(m);
  }
  // Arr<Token>
  Arr *a1 = getlist(m);
  // Arr<Token>
  Arr *a2 = getlist(m);
  machine_push(m, token_new_int(0, it_eq(it_from(a1), it_from(a2), (FCMP)fn)));
}

static void neq (Machine *m) {
  // Arr<Token>
  Token *prg = machine_pop_exc(m, token_LIST);
  int fn (Token *e1, Token *e2) {
    machine_push(m, e1);
    machine_push(m, e2);
    machine_process("", machine_pmachines(m), prg);
    return getint(m);
  }
  // Arr<Token>
  Arr *a1 = getlist(m);
  // Arr<Token>
  Arr *a2 = getlist(m);
  machine_push(m, token_new_int(0, !it_eq(it_from(a1), it_from(a2), (FCMP)fn)));
}

static void lindex (Machine *m) {
  Token *tk = machine_pop(m);
  int fn (Token *t) { return token_eq(t, tk); }
  // Arr<Token>
  Arr *a = getlist(m);
  machine_push(m, token_new_int(0, arr_index(a, (FPRED)fn)));
}

static void indexf (Machine *m) {
  // Arr<Token>
  Token *prg = machine_pop_exc(m, token_LIST);
  int fn (Token *e) {
    machine_push(m, e);
    machine_process("", machine_pmachines(m), prg);
    return getint(m);
  }
  // Arr<Token>
  Arr *a = getlist(m);
  machine_push(m, token_new_int(0, arr_index(a, (FPRED)fn)));
}

static void find (Machine *m) {
  // Arr<Token>
  Token *prg = machine_pop_exc(m, token_LIST);
  int fn (Token *e) {
    machine_push(m, e);
    machine_process("", machine_pmachines(m), prg);
    return getint(m);
  }
  // Arr<Token>
  Arr *a = getlist(m);
  Token *r0 = opt_nget(it_find(it_from(a), (FPRED)fn));
  // Arr<Token>
  Arr *r = arr_new();
  if (r0) arr_push(r, r0);
  pushlist(m, r);
}

static void reduce (Machine *m) {
  // Arr<Token>
  Token *prg = machine_pop_exc(m, token_LIST);
  Token *seed = machine_pop(m);
  Token *fn (Token *seed, Token *e) {
    machine_push(m, seed);
    machine_push(m, e);
    machine_process("", machine_pmachines(m), prg);
    return machine_pop(m);
  }
  // Arr<Token>
  Arr *a = getlist(m);
  EACH(a, Token, e) {
    seed = fn(seed, e);
  }_EACH
  machine_push(m, seed);
}

static void drop (Machine *m) {
  int n = getint(m);
  // Arr<Token>
  Arr *a = getlist(m);
  pushlist(m, arr_drop(a, n));
}

static void dropf (Machine *m) {
  // Arr<Token>
  Token *prg = machine_pop_exc(m, token_LIST);
  int fn (Token *e) {
    machine_push(m, e);
    machine_process("", machine_pmachines(m), prg);
    return getint(m);
  }
  // Arr<Token>
  Arr *a = getlist(m);
  pushlist(m, arr_dropf(a, (FPRED)fn));
}

static void filter (Machine *m) {
  // Arr<Token>
  Token *prg = machine_pop_exc(m, token_LIST);
  int fn (Token *e) {
    machine_push(m, e);
    machine_process("", machine_pmachines(m), prg);
    return getint(m);
  }
  // Arr<Token>
  Arr *a = getlist(m);
  pushlist(m, arr_filter_to(a, (FPRED)fn));
}

static void map (Machine *m) {
  // Arr<Token>
  Token *prg = machine_pop_exc(m, token_LIST);
  Token *fn (Token *e) {
    machine_push(m, e);
    machine_process("", machine_pmachines(m), prg);
    return machine_pop(m);
  }
  // Arr<Token>
  Arr *a = getlist(m);
  pushlist(m, arr_map(a, (FCOPY)fn));
}

static void take (Machine *m) {
  int n = getint(m);
  // Arr<Token>
  Arr *a = getlist(m);
  pushlist(m, arr_take(a, n));
}

static void takef (Machine *m) {
  // Arr<Token>
  Token *prg = machine_pop_exc(m, token_LIST);
  int fn (Token *e) {
    machine_push(m, e);
    machine_process("", machine_pmachines(m), prg);
    return getint(m);
  }
  // Arr<Token>
  Arr *a = getlist(m);
  pushlist(m, arr_takef(a, (FPRED)fn));
}

static void zip (Machine *m) {
  // Arr<Token>
  Arr *a2 = getlist(m);
  // Arr<Token>
  Arr *a1 = getlist(m);
  // Arr<Tp<Token>>
  Arr *r0 = arr_zip(a1, a2);
  // Arr<Token>
  Arr *r = arr_new();
  EACH(r0, Tp, t) {
    // Arr<Token>
    Arr *ae = arr_new();
    arr_push(ae, tp_e1(t));
    arr_push(ae, tp_e2(t));
    arr_push(r, token_new_list(0, ae));
  }_EACH
  pushlist(m, r);
}

static void zip3 (Machine *m) {
  // Arr<Token>
  Arr *a3 = getlist(m);
  // Arr<Token>
  Arr *a2 = getlist(m);
  // Arr<Token>
  Arr *a1 = getlist(m);
  // Arr<Tp<Token>>
  Arr *r0 = arr_zip3(a1, a2, a3);
  // Arr<Token>
  Arr *r = arr_new();
  EACH(r0, Tp3, t) {
    // Arr<Token>
    Arr *ae = arr_new();
    arr_push(ae, tp3_e1(t));
    arr_push(ae, tp3_e2(t));
    arr_push(ae, tp3_e3(t));
    arr_push(r, token_new_list(0, ae));
  }_EACH
  pushlist(m, r);
}

static void unzip (Machine *m) {
  // Arr<Tp<Token>>
  Arr *a = arr_new();
  EACH(getlist(m), Token, tk) {
    // Arr<Token>
    Arr *ae = token_list(tk);
    if (arr_size(ae) != 2) fails_size_list(m, ae, 2);
    arr_push(a, tp_new(arr_get(ae, 0), arr_get(ae, 1)));
  }_EACH
  // Tp3<Arr<Token>, Arr<Token>>>
  Tp *t = arr_unzip(a);
  // Arr<Token>
  Arr *r = arr_new();
  arr_push(r, token_new_list(0, tp_e1(t)));
  arr_push(r, token_new_list(0, tp_e2(t)));
  pushlist(m, r);
}

static void unzip3 (Machine *m) {
  // Arr<Tp<Token>>
  Arr *a = arr_new();
  EACH(getlist(m), Token, tk) {
    // Arr<Token>
    Arr *ae = token_list(tk);
    if (arr_size(ae) != 3) fails_size_list(m, ae, 3);
    arr_push(a, tp3_new(arr_get(ae, 0), arr_get(ae, 1), arr_get(ae, 2)));
  }_EACH
  // Tp3<Arr<Token>, Arr<Token>, Arr<Token>>
  Tp3 *t = arr_unzip3(a);
  // Arr<Token>
  Arr *r = arr_new();
  arr_push(r, token_new_list(0, tp3_e1(t)));
  arr_push(r, token_new_list(0, tp3_e2(t)));
  arr_push(r, token_new_list(0, tp3_e3(t)));
  pushlist(m, r);
}

static void subaux (Machine *m, int begin, int end, int is_right) {
  void bounds (int size) {
    if (is_right) end = size;
    if (begin < 0) begin = size + begin;
    if (begin < 0 || begin > size) fails_range(m, 0, size, begin);
    if (end < 0) end = size + end;
    if (end < 0 || end > size) fails_range(m, 0, size, end);
  }

  Token *tk = machine_pop_exc(m, token_LIST);
  // Arr<Token>
  Arr *a = token_list(tk);

  bounds(arr_size(a));
  int df = end - begin;

  Arr *ra;
  if (df > 0) {
    int len = df * sizeof(Token *);
    Token **tks = GC_MALLOC(len);
    memcpy(tks, (Token **)(arr_start(a) + begin), len);
    ra = arr_new_c(df, (void **)tks);
  } else {
    ra = arr_new();
  }
  machine_push(m, token_new_list(0, ra));
}

static void sub (Machine *m) {
  Token *tk3 = machine_pop_opt(m, token_INT);
  int end = token_int(tk3);
  Token *tk2 = machine_pop_opt(m, token_INT);
  int begin = token_int(tk2);
  subaux(m, begin, end, 0);
}

static void left (Machine *m) {
  Token *tk2 = machine_pop_opt(m, token_INT);
  int cut = token_int(tk2);
  subaux(m, 0, cut, 0);
}

static void right (Machine *m) {
  Token *tk2 = machine_pop_opt(m, token_INT);
  int cut = token_int(tk2);
  subaux(m, cut, 0, 1);
}

// Resturns Map<primitives_Fn>
Map *modlist_mk (void) {
  // Map<primitives_Fn>
  Map *r = map_new();

  map_put(r, "new", new); // [] - LIST
  map_put(r, "unary", unary); // [] - LIST

  map_put(r, "push", push); // [LIST, *] - LIST
  map_put(r, "push+", pushplus); // [LIST, *] - []
  map_put(r, "pop", pop); // LIST - *
  map_put(r, "peek", peek); // LIST - *
  map_put(r, "push0", push0); // [LIST, *] - []
  map_put(r, "push0+", push0plus); // [LIST, *] - LIST
  map_put(r, "pop0", pop0); // LIST - *
  map_put(r, "peek0", peek0); // LIST - *
  map_put(r, "insert", insert);
  map_put(r, "insertList", insertList);
  map_put(r, "remove", lremove);
  map_put(r, "removeRange", removeRange);
  map_put(r, "reverse", reverse);
  map_put(r, "shuffle", shuffle);
  map_put(r, "sort", sort);

  map_put(r, "duplicates", duplicates);
  map_put(r, "all?", all);
  map_put(r, "any?", any);
  map_put(r, "each", each);
  map_put(r, "eachIx", eachix);
  map_put(r, "eq?", eq);
  map_put(r, "neq?", neq);
  map_put(r, "index", lindex);
  map_put(r, "indexf", indexf);
  map_put(r, "find", find);
  map_put(r, "reduce", reduce);

  map_put(r, "clear", clear);
  map_put(r, "drop", drop);
  map_put(r, "dropf", dropf);
  map_put(r, "filter", filter);
  map_put(r, "map", map);
  map_put(r, "take", take);
  map_put(r, "takef", takef);
  map_put(r, "zip", zip);
  map_put(r, "zip3", zip3);
  map_put(r, "unzip", unzip);
  map_put(r, "unzip3", unzip3);

  map_put(r, "sub", sub);
  map_put(r, "left", left);
  map_put(r, "right", right);


  return r;
}


