// Copyright 26-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "primitives/modit.h"
#include "dmc/rnd.h"
#include "Machine.h"
#include "fails.h"
#include "primitives/modlist.h"

static It *getIt (Machine *m) {
  return fails_read_pointer(m, symbol_ITERATOR_, machine_pop(m));
}

static void setIt (Machine *m, It *it) {
  machine_push(m, token_from_pointer(symbol_ITERATOR_, it));
}

static void asList (Machine *m, char *fn) {
  Token *prg = machine_pop(m);
  It *it = getIt(m);
  machine_push(m, token_new_list(0, it_to(it)));
  machine_push(m, prg);
  ((pmodule_Fn)opt_get(pmodule_get(modlist_mk(), symbol_new(fn))))(m);
}

// m_o_fn is Tp3<Machine, Token, Token>. Returns i Opt<Token>.
static Opt *newf (Tp3 *m_o_fn) {
  Machine *m = tp3_e1(m_o_fn);
  machine_push(m, tp3_e2(m_o_fn));
  machine_process("", machine_pmachines(m), tp3_e3(m_o_fn));
  // Arr<Token>
  Arr *a = token_list(machine_pop_exc(m, token_LIST));
  int sz = arr_size(a);
  if (sz > 1) fails_size_list(m, a, 1);
  return sz
    ? opt_new(*arr_start(a))
    : opt_empty()
  ;
}

static void new (Machine *m) {
  Token *prg =machine_pop_exc(m, token_LIST);
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
  Arr *ls =token_list(machine_pop_exc(m, token_LIST));
  setIt(m, it_from(ls));
}

Token *rangef (int *n) { return token_new_int(0, *n); }

static void range (Machine *m) {
  int end = token_int(machine_pop_exc(m, token_INT));
  int begin = token_int(machine_pop_exc(m, token_INT));
  setIt(m, it_map(it_range(begin, end), (FCOPY)rangef));
}

static void range0 (Machine *m) {
  int end = token_int(machine_pop_exc(m, token_INT));
  setIt(m, it_map(it_range0(end), (FCOPY)rangef));
}

static void has (Machine *m) {
  It *it = getIt(m);
  machine_push(m, token_new_int(0, it_has_next(it)));
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
  int n = token_int(machine_pop_exc(m, token_INT));
  It *it = getIt(m);
  setIt(m, it_drop(it, n));
}

static void dropf (Machine *m) {
  asList(m, "dropf");
  // Arr<Token>
  Arr *ls =token_list(machine_pop(m));
  setIt(m, it_from(ls));
}

static void filter (Machine *m) {
  asList(m, "filter");
  // Arr<Token>
  Arr *ls =token_list(machine_pop(m));
  setIt(m, it_from(ls));
}

static void map (Machine *m) {
  asList(m, "map");
  // Arr<Token>
  Arr *ls =token_list(machine_pop(m));
  setIt(m, it_from(ls));
}

static void zip (Machine *m) {
  It *it2 = getIt(m);
  It *it1 = getIt(m);
  // t is Tp<Token, Token>
  Token *fn (Tp *t) {
    return token_new_list(0, arr_new_from(tp_e1(t), tp_e2(t), NULL));
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
      0, arr_new_from(tp3_e1(t), tp3_e2(t), tp3_e3(t), NULL)
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
  int n = token_int(machine_pop_exc(m, token_INT));
  It *it = getIt(m);
  setIt(m, it_take(it, n));
}

static void takef (Machine *m) {
  asList(m, "takef");
  // Arr<Token>
  Arr *ls =token_list(machine_pop(m));
  setIt(m, it_from(ls));
}

static void all (Machine *m) {
  asList(m, "all?");
}

static void any (Machine *m) {
  asList(m, "any?");
}

static void count (Machine *m) {
  It *it = getIt(m);
  machine_push(m, token_new_int(0, it_count(it)));
}

static void duplicates (Machine *m) {
  asList(m, "duplicates");
}

static void each (Machine *m) {
  asList(m, "each");
}

static void eachix (Machine *m) {
  asList(m, "eachIx");
}

static void eq (Machine *m) {
  Token *prg = machine_pop(m);
  It *it2 = getIt(m);
  It *it1 = getIt(m);
  machine_push(m, token_new_list(0, it_to(it1)));
  machine_push(m, token_new_list(0, it_to(it2)));
  machine_push(m, prg);
  ((pmodule_Fn)opt_get(pmodule_get(modlist_mk(), symbol_new("eq?"))))(m);
}

static void neq (Machine *m) {
  Token *prg = machine_pop(m);
  It *it2 = getIt(m);
  It *it1 = getIt(m);
  machine_push(m, token_new_list(0, it_to(it1)));
  machine_push(m, token_new_list(0, it_to(it2)));
  machine_push(m, prg);
  ((pmodule_Fn)opt_get(pmodule_get(modlist_mk(), symbol_new("neq?"))))(m);
}

static void equals (Machine *m) {
  machine_push(m, token_new_list(
    0, arr_new_from(token_new_symbol(0, symbol_new("==")), NULL)
  ));
  eq(m);
}

static void notequals (Machine *m) {
  machine_push(m, token_new_list(
    0, arr_new_from(token_new_symbol(0, symbol_new("==")), NULL)
  ));
  neq(m);
}

static void find (Machine *m) {
  asList(m, "find");
}

static void iindex (Machine *m) {
  Token *tk = machine_pop(m);
  machine_push(m, token_new_list(
    0, arr_new_from(tk, token_new_symbol(0, symbol_new("==")), NULL)
  ));
  asList(m, "indexf");
}

static void indexf (Machine *m) {
  asList(m, "indexf");
}

static void lastindex (Machine *m) {
  Token *tk = machine_pop(m);
  machine_push(m, token_new_list(
    0, arr_new_from(tk, token_new_symbol(0, symbol_new("==")), NULL)
  ));
  asList(m, "lastIndexf");
}

static void lastindexf (Machine *m) {
  asList(m, "lastIndexf");
}

static void reduce (Machine *m) {
  Token *prg = machine_pop(m);
  Token *seed = machine_pop(m);
  It *it = getIt(m);
  machine_push(m, token_new_list(0, it_to(it)));
  machine_push(m, seed);
  machine_push(m, prg);
  ((pmodule_Fn)opt_get(pmodule_get(modlist_mk(), symbol_new("reduce"))))(m);
}

static void to (Machine *m) {
  It *it = getIt(m);
  machine_push(m, token_new_list(0, it_to(it)));
}

static void shuffle (Machine *m) {
  It *it = getIt(m);
  Token *tk = token_new_list(0, it_to(it));
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
  Token *tk = token_new_list(0, it_to(it));
  machine_push(m, tk);
  machine_push(m, prg);
  ((pmodule_Fn)opt_get(pmodule_get(modlist_mk(), symbol_new("sort"))))(m);
  // Arr<Token>
  Arr *ls =token_list(tk);
  setIt(m, it_from(ls));
}

static void box (Machine *m) {
  // Arr<Token>
  Arr *a = token_list(machine_pop_exc(m, token_LIST));
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



