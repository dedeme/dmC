// Copyright 04-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "modules/modlist.h"
#include "fails.h"
#include "Machine.h"

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
  machine_push(m, token_new_list(a));
}

static int getint (Machine *m) {
  return token_int(machine_pop_exc(m, token_INT));
}

static void push (Machine *m) {
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
  Arr *a = peeklist(m);
  fails_check_range(m, 0, arr_size(a), ix);
  arr_insert(a, ix, tk);
}

static void insertList (Machine *m) {
  // Arr<Token>
  Arr *add = getlist(m);
  int ix = getint(m);
  // Arr<Token>
  Arr *a = peeklist(m);
  fails_check_range(m, 0, arr_size(a), ix);
  arr_insert_arr(a, ix, add);
}

static void lremove (Machine *m) {
  int ix = getint(m);
  // Arr<Token>
  Arr *a = peeklist(m);
  fails_check_range(m, 0, arr_size(a) - 1, ix);
  arr_remove(a, ix);
}

static void removeRange (Machine *m) {
  int end = getint(m);
  int begin = getint(m);
  // Arr<Token>
  Arr *a = peeklist(m);
  fails_check_range(m, 0, arr_size(a), begin);
  fails_check_range(m, begin, arr_size(a), end);
  arr_remove_range(a, begin, end);
}

static void reverse (Machine *m) {
  // Arr<Token>
  Arr *a = peeklist(m);
  arr_reverse(a);
}

static void shuffle (Machine *m) {
  // Arr<Token>
  Arr *a = peeklist(m);
  arr_shuffle(a);
}

static void sort (Machine *m) {
  // List<Machine>
  List *pmachines = list_cons(machine_pmachines(m), m);

  // Arr<Token>
  Token *prg = machine_pop_exc(m, token_LIST);
  int fn (Token *e1, Token *e2) {
    machine_push(m, e1);
    machine_push(m, e2);
    machine_process("", pmachines, prg);
    return getint(m);
  }
  // Arr<Token>
  Arr *a = peeklist(m);
  arr_sort(a, (FCMP)fn);
}

static void lindex (Machine *m) {
  Token *tk = machine_pop(m);
  int fn (Token *t) { return token_eq(t, tk); }
  // Arr<Token>
  Arr *a = getlist(m);
  machine_push(m, token_new_int(arr_index(a, (FPRED)fn)));
}

static void indexf (Machine *m) {
  // List<Machine>
  List *pmachines = list_cons(machine_pmachines(m), m);

  // Arr<Token>
  Token *prg = machine_pop_exc(m, token_LIST);
  int fn (Token *e) {
    machine_push(m, e);
    machine_process("", pmachines, prg);
    return getint(m);
  }
  // Arr<Token>
  Arr *a = getlist(m);
  machine_push(m, token_new_int(arr_index(a, (FPRED)fn)));
}

static void find (Machine *m) {
  // List<Machine>
  List *pmachines = list_cons(machine_pmachines(m), m);

  // Arr<Token>
  Token *prg = machine_pop_exc(m, token_LIST);
  int fn (Token *e) {
    machine_push(m, e);
    machine_process("", pmachines, prg);
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

}

static void clear (Machine *m) {
  getlist(m);
  pushlist(m, arr_new());
}

static void drop (Machine *m) {
  int n = getint(m);
  // Arr<Token>
  Arr *a = getlist(m);
  pushlist(m, arr_drop(a, n));
}

static void dropf (Machine *m) {
  // List<Machine>
  List *pmachines = list_cons(machine_pmachines(m), m);

  // Arr<Token>
  Token *prg = machine_pop_exc(m, token_LIST);
  int fn (Token *e) {
    machine_push(m, e);
    machine_process("", pmachines, prg);
    return getint(m);
  }
  // Arr<Token>
  Arr *a = getlist(m);
  pushlist(m, arr_dropf(a, (FPRED)fn));
}

static void filter (Machine *m) {
  // List<Machine>
  List *pmachines = list_cons(machine_pmachines(m), m);

  // Arr<Token>
  Token *prg = machine_pop_exc(m, token_LIST);
  int fn (Token *e) {
    machine_push(m, e);
    machine_process("", pmachines, prg);
    return getint(m);
  }
  // Arr<Token>
  Arr *a = getlist(m);
  pushlist(m, arr_filter_to(a, (FPRED)fn));
}

static void map (Machine *m) {
  // List<Machine>
  List *pmachines = list_cons(machine_pmachines(m), m);

  // Arr<Token>
  Token *prg = machine_pop_exc(m, token_LIST);
  Token *fn (Token *e) {
    machine_push(m, e);
    machine_process("", pmachines, prg);
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
  // List<Machine>
  List *pmachines = list_cons(machine_pmachines(m), m);

  // Arr<Token>
  Token *prg = machine_pop_exc(m, token_LIST);
  int fn (Token *e) {
    machine_push(m, e);
    machine_process("", pmachines, prg);
    return getint(m);
  }
  // Arr<Token>
  Arr *a = getlist(m);
  pushlist(m, arr_takef(a, (FPRED)fn));
}

// Resturns Map<primitives_Fn>
Map *modlist_mk (void) {
  // Map<primitives_Fn>
  Map *r = map_new();

  map_put(r, "lst.push", push);
  map_put(r, "lst.pop", pop);
  map_put(r, "lst.peek", peek);
  map_put(r, "lst.push0", push0);
  map_put(r, "lst.pop0", pop0);
  map_put(r, "lst.peek0", peek0);
  map_put(r, "lst.insert", insert);
  map_put(r, "lst.insertList", insertList);
  map_put(r, "lst.remove", lremove);
  map_put(r, "lst.removeRange", removeRange);
  map_put(r, "lst.reverse", reverse);
  map_put(r, "lst.shuffle", shuffle);
  map_put(r, "lst.sort", sort);

  map_put(r, "lst.index", lindex);
  map_put(r, "lst.indexf", indexf);
  map_put(r, "lst.find", find);
  map_put(r, "lst.reduce", reduce);

  map_put(r, "lst.clear", clear);
  map_put(r, "lst.drop", drop);
  map_put(r, "lst.dropf", dropf);
  map_put(r, "lst.filter", filter);
  map_put(r, "lst.map", map);
  map_put(r, "lst.take", take);
  map_put(r, "lst.takef", takef);

  return r;
}


