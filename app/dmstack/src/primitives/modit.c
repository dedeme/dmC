// Copyright 26-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "primitives/modit.h"
#include "dmc/rnd.h"
#include "Machine.h"
#include "fails.h"
#include "primitives/modlist.h"

static It *getIt (Machine *m) {
  return fails_read_pointer(m, "= Iterator", machine_pop(m));
}

static void setIt (Machine *m, It *it) {
  machine_push(m, token_from_pointer("= Iterator", it));
}

static void asList (Machine *m, char *fn) {
  Token *prg = machine_pop(m);
  It *it = getIt(m);
  machine_push(m, token_new_list(0, it_to(it)));
  machine_push(m, prg);
  ((primitives_Fn)opt_get(map_get(modlist_mk(), fn)))(m);
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
  ((primitives_Fn)opt_get(map_get(modlist_mk(), "eq?")))(m);
}

static void neq (Machine *m) {
  Token *prg = machine_pop(m);
  It *it2 = getIt(m);
  It *it1 = getIt(m);
  machine_push(m, token_new_list(0, it_to(it1)));
  machine_push(m, token_new_list(0, it_to(it2)));
  machine_push(m, prg);
  ((primitives_Fn)opt_get(map_get(modlist_mk(), "neq?")))(m);
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
  ((primitives_Fn)opt_get(map_get(modlist_mk(), "reduce")))(m);
}

static void to (Machine *m) {
  It *it = getIt(m);
  machine_push(m, token_new_list(0, it_to(it)));
}

static void shuffle (Machine *m) {
  It *it = getIt(m);
  Token *tk = token_new_list(0, it_to(it));
  machine_push(m, tk);
  ((primitives_Fn)opt_get(map_get(modlist_mk(), "shuffle")))(m);
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
  ((primitives_Fn)opt_get(map_get(modlist_mk(), "sort")))(m);
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

// Resturns Map<primitives_Fn>
Map *modit_mk (void) {
  // Map<primitives_Fn>
  Map *r = map_new();

  map_put(r, "new", new);
  map_put(r, "empty", empty);
  map_put(r, "unary", unary);
  map_put(r, "from", from);
  map_put(r, "range", range);
  map_put(r, "range0", range0);
  map_put(r, "has?", has);
  map_put(r, "peek", peek);
  map_put(r, "next", next);

  map_put(r, "+", plus);
  map_put(r, "drop", drop);
  map_put(r, "dropf", dropf);
  map_put(r, "filter", filter);
  map_put(r, "map", map);
  map_put(r, "push", push);
  map_put(r, "push0", push0);
  map_put(r, "take", take);
  map_put(r, "takef", takef);
  map_put(r, "zip", zip);
  map_put(r, "zip3", zip3);

  map_put(r, "all?", all);
  map_put(r, "any?", any);
  map_put(r, "count", count);
  map_put(r, "duplicates", duplicates);
  map_put(r, "each", each);
  map_put(r, "eachIx", eachix);
  map_put(r, "eq?", eq);
  map_put(r, "neq?", neq);
  map_put(r, "==", equals);
  map_put(r, "!=", notequals);
  map_put(r, "find", find);
  map_put(r, "index", iindex);
  map_put(r, "indexf", indexf);
  map_put(r, "lastIndex", lastindex);
  map_put(r, "lastIndexf", lastindexf);
  map_put(r, "reduce", reduce);
  map_put(r, "to", to);

  map_put(r, "shuffle", shuffle);
  map_put(r, "reverse", reverse);
  map_put(r, "sort", sort);

  map_put(r, "box", box);

  return r;
}



