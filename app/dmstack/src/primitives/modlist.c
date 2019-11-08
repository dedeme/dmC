// Copyright 04-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "primitives/modlist.h"
#include "fails.h"
#include "tk.h"
#include "primitives/modjs.h"
#include "DEFS.h"

// Returns Arr<Token>
static Arr *getlist (Machine *m) {
  return tk_pop_list(m);
}

// Returns Arr<Token>
static Arr *peeklist (Machine *m) {
  return tk_peek_list(m);
}

// a is Arr<Tokeb>
static void pushlist (Machine *m, Arr *a) {
  machine_push(m, token_new_list(a));
}

static int getint (Machine *m) {
  return tk_pop_int(m);
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

static void make (Machine *m) {
  Int sz = tk_pop_int(m);
  Token *tk = machine_pop(m);
  if (token_type(tk) == token_LIST || token_type(tk) == token_NATIVE)
    fails_types_in(m, 4, (enum token_Type[]) {
        token_INT, token_FLOAT, token_STRING, token_SYMBOL
      }, tk);
  Token **tks = GC_MALLOC(sizeof(Token *) * sz);
  Token **p = tks;
  Token **pend = p + sz;
  while (p < pend) *p++ = tk;
  pushlist(m, arr_new_c(sz, (void **)tks));
}

static void fill (Machine *m) {
  Token *tk = machine_pop(m);
  if (token_type(tk) == token_LIST || token_type(tk) == token_NATIVE)
    fails_types_in(m, 4, (enum token_Type[]) {
        token_INT, token_FLOAT, token_STRING, token_SYMBOL
      }, tk);
  // Arr<Token>
  Arr *a = tk_pop_list(m);
  Int sz = arr_size(a);;
  Token **p = (Token **)arr_start(a);
  Token **pend = p + sz;
  while (p < pend) *p++ = tk;
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

static void insertlist (Machine *m) {
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

static void removerange (Machine *m) {
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
  arr_push(r, token_new_list(tp_e1(dr)));
  arr_push(r, token_new_list(tp_e2(dr)));
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
  machine_push(m, token_new_int(arr_all(a, (FPRED)fn)));
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
  machine_push(m, token_new_int(arr_any(a, (FPRED)fn)));
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
    machine_push(m, token_new_int(c++));
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
  machine_push(m, token_new_int(it_eq(it_from(a1), it_from(a2), (FCMP)fn)));
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
  machine_push(m, token_new_int(!it_eq(it_from(a1), it_from(a2), (FCMP)fn)));
}

static void lindex (Machine *m) {
  Token *tk = machine_pop(m);
  int fn (Token *t) { return token_eq(t, tk); }
  // Arr<Token>
  Arr *a = getlist(m);
  machine_push(m, token_new_int(arr_index(a, (FPRED)fn)));
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
  machine_push(m, token_new_int(arr_index(a, (FPRED)fn)));
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

static void lastindex (Machine *m) {
  Token *tk = machine_pop(m);
  int fn (Token *t) { return token_eq(t, tk); }
  // Arr<Token>
  Arr *a = getlist(m);
  machine_push(m, token_new_int(arr_last_index(a, (FPRED)fn)));
}

static void lastindexf (Machine *m) {
  // Arr<Token>
  Token *prg = machine_pop_exc(m, token_LIST);
  int fn (Token *e) {
    machine_push(m, e);
    machine_process("", machine_pmachines(m), prg);
    return getint(m);
  }
  // Arr<Token>
  Arr *a = getlist(m);
  machine_push(m, token_new_int(arr_last_index(a, (FPRED)fn)));
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

static void flat (Machine *m) {
  // Arr<Token>
  Arr *a = getlist(m);
  // Arr<Token>
  Arr *r = arr_new();
  void add (Arr *ls) {
    EACH(ls, Token, tk) {
      if (token_type(tk) == token_LIST) add(token_list(tk));
      else arr_push(r, tk);
    } _EACH
  }
  add(a);
  pushlist(m, r);
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
    arr_push(r, token_new_list(ae));
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
    arr_push(r, token_new_list(ae));
  }_EACH
  pushlist(m, r);
}

static void unzip (Machine *m) {
  // Arr<Tp<Token>>
  Arr *a = arr_new();
  EACH(getlist(m), Token, tk) {
    // Arr<Token>
    Arr *ae = token_list(tk);
    if (arr_size(ae) != 2) fails_list_size(m, ae, 2);
    arr_push(a, tp_new(arr_get(ae, 0), arr_get(ae, 1)));
  }_EACH
  // Tp3<Arr<Token>, Arr<Token>>>
  Tp *t = arr_unzip(a);
  // Arr<Token>
  Arr *r = arr_new();
  arr_push(r, token_new_list(tp_e1(t)));
  arr_push(r, token_new_list(tp_e2(t)));
  pushlist(m, r);
}

static void unzip3 (Machine *m) {
  // Arr<Tp<Token>>
  Arr *a = arr_new();
  EACH(getlist(m), Token, tk) {
    // Arr<Token>
    Arr *ae = token_list(tk);
    if (arr_size(ae) != 3) fails_list_size(m, ae, 3);
    arr_push(a, tp3_new(arr_get(ae, 0), arr_get(ae, 1), arr_get(ae, 2)));
  }_EACH
  // Tp3<Arr<Token>, Arr<Token>, Arr<Token>>
  Tp3 *t = arr_unzip3(a);
  // Arr<Token>
  Arr *r = arr_new();
  arr_push(r, token_new_list(tp3_e1(t)));
  arr_push(r, token_new_list(tp3_e2(t)));
  arr_push(r, token_new_list(tp3_e3(t)));
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

  // Arr<Token>
  Arr *a = tk_pop_list(m);

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
  machine_push(m, token_new_list(ra));
}

static void sub (Machine *m) {
  Int end = tk_pop_int(m);
  Int begin = tk_pop_int(m);
  subaux(m, begin, end, 0);
}

static void left (Machine *m) {
  Int cut = tk_pop_int(m);
  subaux(m, 0, cut, 0);
}

static void right (Machine *m) {
  Int cut = tk_pop_int(m);
  subaux(m, cut, 0, 1);
}

static void copy (Machine *m) {
  // Arr<Token>
  Arr *a = tk_pop_list(m);

  Arr *ra;
  int df = arr_size(a);
  if (df) {
    int len = df * sizeof(Token *);
    Token **tks = GC_MALLOC(len);
    memcpy(tks, (Token **)(arr_start(a)), len);
    ra = arr_new_c(df, (void **)tks);
  } else {
    ra = arr_new();
  }

  machine_push(m, token_new_list(ra));
}

static void fromjs (Machine *m) {
  modjs_to_list(m);
}

static void tojs (Machine *m) {
  modjs_from_list(m);
}

Pmodule *modlist_mk (void) {
  Pmodule *r = pmodule_new();
  void add (char *name, pmodule_Fn fn) {
    pmodule_add(r, symbol_new(name), fn);
  }

  add("new", new); // [] - LIST
  add("unary", unary); // [] - LIST
  add("make", make); // <INT, *> - <LIST>

  add("fill", fill); // <LIST, *> - <>
  add("push", push); // <LIST, *> - <>
  add("push+", pushplus); // <LIST, *> - <LIST>
  add("pop", pop); // LIST - *
  add("peek", peek); // LIST - *
  add("push0", push0); // [LIST, *] - []
  add("push0+", push0plus); // [LIST, *] - LIST
  add("pop0", pop0); // LIST - *
  add("peek0", peek0); // LIST - *
  add("insert", insert);
  add("insertList", insertlist);
  add("remove", lremove);
  add("removeRange", removerange);
  add("clear", clear);
  add("reverse", reverse);
  add("shuffle", shuffle);
  add("sort", sort);

  add("duplicates", duplicates);
  add("all?", all);
  add("any?", any);
  add("each", each);
  add("eachIx", eachix);
  add("eq?", eq);
  add("neq?", neq);
  add("index", lindex);
  add("indexf", indexf);
  add("find", find);
  add("lastIndex", lastindex);
  add("lastIndexf", lastindexf);
  add("reduce", reduce);

  add("flat", flat);
  add("drop", drop);
  add("dropf", dropf);
  add("filter", filter);
  add("map", map);
  add("take", take);
  add("takef", takef);
  add("zip", zip);
  add("zip3", zip3);
  add("unzip", unzip);
  add("unzip3", unzip3);

  add("sub", sub);
  add("left", left);
  add("right", right);
  add("copy", copy);

  add("fromJs", fromjs);
  add("toJs", tojs);

  return r;
}


