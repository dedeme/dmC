// Copyright 26-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "primitives/modit.h"
#include "dmc/rnd.h"
#include "tk.h"
#include "fails.h"
#include "primitives/modlist.h"

typedef struct {
  Machine *m;
  Token *prg;
  Token *tk;
} modIt_stkItElement;

typedef struct {
  Machine *m;
  Token *prg;
  It *it;
} modIt_toStkIt_O;

static Opt *toStkIt_next (modIt_toStkIt_O *o) {
  if (it_has_next(o->it)) {
    modIt_stkItElement *e = MALLOC(modIt_stkItElement);
    e->m = o->m;
    e->prg = o->prg;
    e->tk = it_next(o->it);
    return opt_new(e);
  }
  return opt_empty();
}

// It<modIt_stkItElement>
static It *toStkIt (Machine *m, Token *prg, It *it) {
  modIt_toStkIt_O *o = MALLOC(modIt_toStkIt_O);
  o->m = m;
  o->prg = prg;
  o->it = it;
  return it_new(o, (it_Next)toStkIt_next);
}

static Token *modIt_stkItElement_to_token (modIt_stkItElement *e) {
  return e->tk;
}

// It<modIt_stkItElement>
static void setStkIt (Machine *m, It *it) {
  machine_push(m, token_from_pointer(
    symbol_ITERATOR_,
    it_map(it, (FCOPY)modIt_stkItElement_to_token)
  ));
}

static void tofproc (void *value) {
  modIt_stkItElement *e = value;
  Machine *m = e->m;
  machine_push(m, e->tk);
  machine_process("", machine_pmachines(m), e->prg);
}

static void tofprocix (void *value, size_t ix) {
  modIt_stkItElement *e = value;
  Machine *m = e->m;
  machine_push(m, e->tk);
  machine_push(m, token_new_int(ix));
  machine_process("", machine_pmachines(m), e->prg);
}

static int tofpred (void *value) {
  modIt_stkItElement *e = value;
  Machine *m = e->m;
  machine_push(m, e->tk);
  machine_process("", machine_pmachines(m), e->prg);
  return tk_pop_int(m);
}

static int tofeq (void *v1, void *v2) {
  modIt_stkItElement *e1 = v1;
  modIt_stkItElement *e2 = v2;
  Machine *m = e1->m;
  machine_push(m, e1->tk);
  machine_push(m, e2->tk);
  machine_process("", machine_pmachines(m), e1->prg);
  return tk_pop_int(m);
}

static void *tofcopy (void *value) {
  modIt_stkItElement *e = value;
  Machine *m = e->m;
  Token *prg = e->prg;
  machine_push(m, e->tk);
  machine_process("", machine_pmachines(m), prg);
  modIt_stkItElement *r = MALLOC(modIt_stkItElement);
  r->m = m;
  r->prg = prg;
  r->tk = machine_pop(m);
  return r;
}

// -----------------------------------------------------------------------------

static It *getIt (Machine *m) {
  return tk_pop_native(m, symbol_ITERATOR_);
}

static void setIt (Machine *m, It *it) {
  machine_push(m, token_from_pointer(symbol_ITERATOR_, it));
}

// m_o_fn is Tp3<Machine, Token, Token>. Returns i Opt<Token>.
static Opt *newf (Tp3 *m_o_fn) {
  Machine *m = tp3_e1(m_o_fn);
  machine_push(m, tp3_e2(m_o_fn));
  machine_process("", machine_pmachines(m), tp3_e3(m_o_fn));
  // Arr<Token>
  Arr *a = tk_pop_list(m);
  int sz = arr_size(a);
  if (sz > 1) fails_list_size(m, a, 1);
  return sz
    ? opt_new(*arr_start(a))
    : opt_empty()
  ;
}

static void new (Machine *m) {
  Token *prg = machine_pop_exc(m, token_LIST);
  Token *o = machine_pop(m);
  It *it = it_new(tp3_new(m, o, prg), (Opt *(*)(void *))newf);
  setIt(m, it);
}

static void empty (Machine *m) {
  setIt(m, it_empty());
}

static void unary (Machine *m) {
  Token *o = machine_pop(m);
  setIt(m, it_unary(o));
}

static void from (Machine *m) {
  // Arr<Token>
  Arr *ls =tk_pop_list(m);
  setIt(m, it_from(ls));
}

Token *rangef (int *n) { return token_new_int(*n); }

static void range (Machine *m) {
  int end = tk_pop_int(m);
  int begin = tk_pop_int(m);
  setIt(m, it_map(it_range(begin, end), (FCOPY)rangef));
}

static void range0 (Machine *m) {
  int end = tk_pop_int(m);
  setIt(m, it_map(it_range0(end), (FCOPY)rangef));
}

static void has (Machine *m) {
  It *it = getIt(m);
  machine_push(m, token_new_int(it_has_next(it)));
}

static void peek (Machine *m) {
  It *it = getIt(m);
  machine_push(m, it_peek(it));
}

static void next (Machine *m) {
  It *it = getIt(m);
  machine_push(m, it_next(it));
}

static void plus (Machine *m) {
  It *it2 = getIt(m);
  It *it1 = getIt(m);
  setIt(m, it_cat(it1, it2));
}

static void drop (Machine *m) {
  int n = tk_pop_int(m);
  It *it = getIt(m);
  setIt(m, it_drop(it, n));
}

static void dropf (Machine *m) {
  Token *prg = machine_pop_exc(m, token_LIST);
  // It<Token>
  It *it = getIt(m);
  setStkIt(m, it_dropf(toStkIt(m, prg, it), tofpred));
}

static void filter (Machine *m) {
  Token *prg = machine_pop_exc(m, token_LIST);

  // It<Token>
  It *it = getIt(m);
  setStkIt(m, it_filter(toStkIt(m, prg, it), tofpred));
}

static void map (Machine *m) {
  Token *prg = machine_pop_exc(m, token_LIST);

  // It<Token>
  It *it = getIt(m);
  setStkIt(m, it_map(toStkIt(m, prg, it), tofcopy));
}

static void zip (Machine *m) {
  It *it2 = getIt(m);
  It *it1 = getIt(m);
  // t is Tp<Token, Token>
  Token *fn (Tp *t) {
    return token_new_list(arr_new_from(tp_e1(t), tp_e2(t), NULL));
  }
  setIt(m, it_from(it_to(it_map(it_zip(it1, it2), (FCOPY) fn))));
}

static void zip3 (Machine *m) {
  It *it3 = getIt(m);
  It *it2 = getIt(m);
  It *it1 = getIt(m);
  // t is Tp3<Token, Token, Token>
  Token *fn (Tp3 *t) {
    return token_new_list(
      arr_new_from(tp3_e1(t), tp3_e2(t), tp3_e3(t), NULL)
    );
  }
  setIt(m, it_from(it_to(it_map(it_zip3(it1, it2, it3), (FCOPY) fn))));
}

static void push (Machine *m) {
  Token *tk = machine_pop(m);
  It *it = getIt(m);
  setIt(m, it_add(it, tk));
}

static void push0 (Machine *m) {
  Token *tk = machine_pop(m);
  It *it = getIt(m);
  setIt(m, it_add0(it, tk));
}

static void take (Machine *m) {
  int n = tk_pop_int(m);
  It *it = getIt(m);
  setIt(m, it_take(it, n));
}

static void takef (Machine *m) {
  // Arr<Token>
  Token *prg = machine_pop_exc(m, token_LIST);

  // It<Token>
  It *it = getIt(m);
  setStkIt(m, it_takef(toStkIt(m, prg, it), tofpred));
}

static void all (Machine *m) {
  Token *prg = machine_pop_exc(m, token_LIST);

  Token *prg2 = token_new_list(arr_new_from(
    prg,
    token_new_symbol(symbol_RUN),
    token_new_symbol(symbol_new("!")),
    NULL
  ));
  // It<Token>
  It *it = getIt(m);
  machine_push(m, token_new_int(
    !it_contains(toStkIt(m, prg2, it), tofpred)
  ));
}

static void any (Machine *m) {
  Token *prg = machine_pop_exc(m, token_LIST);

  // It<Token>
  It *it = getIt(m);
  machine_push(m, token_new_int(it_contains(toStkIt(m, prg, it), tofpred)));
}

static void count (Machine *m) {
  It *it = getIt(m);
  machine_push(m, token_new_int(it_count(it)));
}

static void duplicates (Machine *m) {
  Token *prg = machine_pop_exc(m, token_LIST);

  // It<Token>
  It *it = getIt(m);
  // Tp<Arr<modIt_stkItElement>>
  Tp *dr = it_duplicates(toStkIt(m, prg, it), tofeq);
  // Arr<Token>
  Arr *l1 = arr_map(tp_e1(dr), (FCOPY)modIt_stkItElement_to_token);
  // Arr<Token>
  Arr *l2 = arr_map(tp_e2(dr), (FCOPY)modIt_stkItElement_to_token);
  // Arr<Token>
  Arr *r = arr_new();
  arr_push(r, token_new_list(l1));
  arr_push(r, token_new_list(l2));
  machine_push(m, token_new_list(r));
}

static void each (Machine *m) {
  Token *prg = machine_pop_exc(m, token_LIST);

  // It<Token>
  It *it = getIt(m);
  it_each(toStkIt(m, prg, it), tofproc);
}

static void eachix (Machine *m) {
  Token *prg = machine_pop_exc(m, token_LIST);

  // It<Token>
  It *it = getIt(m);
  it_each_ix(toStkIt(m, prg, it), tofprocix);
}

static void eq (Machine *m) {
  Token *prg = machine_pop_exc(m, token_LIST);

  // It<Token>
  It *it2 = getIt(m);
  // It<Token>
  It *it1 = getIt(m);
  // Tp<Arr<modIt_stkItElement>>
  machine_push(m, token_new_int(
    it_eq(toStkIt(m, prg, it1), toStkIt(m, prg, it2), tofeq)
  ));
}

static void neq (Machine *m) {
  Token *prg = machine_pop_exc(m, token_LIST);

  // It<Token>
  It *it2 = getIt(m);
  // It<Token>
  It *it1 = getIt(m);
  // Tp<Arr<modIt_stkItElement>>
  machine_push(m, token_new_int(
    !it_eq(toStkIt(m, prg, it1), toStkIt(m, prg, it2), tofeq)
  ));
}

static void equals (Machine *m) {
  machine_push(m, token_new_list(
    arr_new_from(token_new_symbol(symbol_new("==")), NULL)
  ));
  eq(m);
}

static void notequals (Machine *m) {
  machine_push(m, token_new_list(
    arr_new_from(token_new_symbol(symbol_new("==")), NULL)
  ));
  neq(m);
}

static void find (Machine *m) {
  Token *prg = machine_pop_exc(m, token_LIST);

  // It<Token>
  It *it = getIt(m);
  modIt_stkItElement *e = opt_nget(it_find(toStkIt(m, prg, it), tofpred));

  // Arr<Token>
  Arr *r = arr_new();
  if (e) arr_push(r, e->tk);
  machine_push(m, token_new_list(r));
}

static void indexf (Machine *m) {
  Token *prg = machine_pop_exc(m, token_LIST);

  // It<Token>
  It *it = getIt(m);
  machine_push(m, token_new_int(it_index(toStkIt(m, prg, it), tofpred)));
}

static void iindex (Machine *m) {
  Token *tk = machine_pop(m);
  machine_push(m, token_new_list(
    arr_new_from(tk, token_new_symbol(symbol_new("==")), NULL)
  ));
  indexf(m);
}

static void lastindexf (Machine *m) {
  Token *prg = machine_pop_exc(m, token_LIST);

  // It<Token>
  It *it = getIt(m);
  machine_push(m, token_new_int(
    it_last_index(toStkIt(m, prg, it), tofpred)
  ));
}

static void lastindex (Machine *m) {
  Token *tk = machine_pop(m);
  machine_push(m, token_new_list(
    arr_new_from(tk, token_new_symbol(symbol_new("==")), NULL)
  ));
  lastindexf(m);
}

static void reduce (Machine *m) {
  Token *prg = machine_pop(m);
  Token *seed = machine_pop(m);
  It *it = getIt(m);
  machine_push(m, token_new_list(it_to(it)));
  machine_push(m, seed);
  machine_push(m, prg);
  ((pmodule_Fn)opt_get(pmodule_get(modlist_mk(), symbol_new("reduce"))))(m);
}

static void to (Machine *m) {
  It *it = getIt(m);
  machine_push(m, token_new_list(it_to(it)));
}

static void shuffle (Machine *m) {
  It *it = getIt(m);
  Token *tk = token_new_list(it_to(it));
  machine_push(m, tk);
  ((pmodule_Fn)opt_get(pmodule_get(modlist_mk(), symbol_new("shuffle"))))(m);
  // Arr<Token>
  Arr *ls =token_list(tk);
  setIt(m, it_from(ls));
}

static void reverse (Machine *m) {
  It *it = getIt(m);
  setIt(m, it_reverse(it));
}

static void sort (Machine *m) {
  Token *prg = machine_pop(m);
  It *it = getIt(m);
  Token *tk = token_new_list(it_to(it));
  machine_push(m, tk);
  machine_push(m, prg);
  ((pmodule_Fn)opt_get(pmodule_get(modlist_mk(), symbol_new("sort"))))(m);
  // Arr<Token>
  Arr *ls =token_list(tk);
  setIt(m, it_from(ls));
}

static void box (Machine *m) {
  // Arr<Token>
  Arr *a = tk_pop_list(m);
  if (!arr_size(a)) fails_list_size(m, a, 1);
  // Opt<Token>
  Opt *fn (Token *tk) { return opt_new(tk); }
  Box *bx = rnd_box_new(arr_map(a, (FCOPY)fn));
  It *it = it_new(bx, (Opt *(*)(void *))rnd_box_next);
  setIt(m, it);
}

Pmodule *modit_mk (void) {
  Pmodule *r = pmodule_new();
  void add (char *name, pmodule_Fn fn) {
    pmodule_add(r, symbol_new(name), fn);
  }

  add("new", new);
  add("empty", empty);
  add("unary", unary);
  add("from", from);
  add("range", range);
  add("range0", range0);
  add("has?", has);
  add("peek", peek);
  add("next", next);

  add("+", plus);
  add("drop", drop);
  add("dropf", dropf);
  add("filter", filter);
  add("map", map);
  add("push", push);
  add("push0", push0);
  add("take", take);
  add("takef", takef);
  add("zip", zip);
  add("zip3", zip3);

  add("all?", all);
  add("any?", any);
  add("count", count);
  add("duplicates", duplicates);
  add("each", each);
  add("eachIx", eachix);
  add("eq?", eq);
  add("neq?", neq);
  add("==", equals);
  add("!=", notequals);
  add("find", find);
  add("index", iindex);
  add("indexf", indexf);
  add("lastIndex", lastindex);
  add("lastIndexf", lastindexf);
  add("reduce", reduce);
  add("to", to);

  add("shuffle", shuffle);
  add("reverse", reverse);
  add("sort", sort);

  add("box", box);

  return r;
}



