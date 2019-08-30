// Copyright 27-Aug-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "Machine.h"
#include "primitives.h"
#include "fails.h"

struct machine_Machine {
  // List<Map<Token>>
  List *pheaps;
  // Arr<Token>
  Arr *stack;
  // Map<Token>
  Map *heap;
  Token *prg;
  int ix;
};

List *machine_pheaps (Machine *this) {
  return this->pheaps;
}

Arr *machine_stack (Machine *this) {
  return this->stack;
}

Map *machine_heap (Machine *this) {
  return this->heap;
}

Token *machine_prg (Machine *this) {
  return this->prg;
}

static void mrun (Machine *this) {
  char *fnc = token_string(machine_pop_exc(this, token_STRING));
  char *module = token_string(machine_pop_exc(this, token_STRING));
  primitives_Fn fn = opt_nget(primitives_get(module, fnc));
  if (fn) fn(this);
  else machine_fail(this, str_f("Symbol '%s.%s' not found", module, fnc));
}

static void run (Machine *this) {
  Token *prg = machine_pop_exc(this, token_LIST);
  // List<Map<Token>>
  List *pheaps = this->pheaps;
  pheaps = list_cons(pheaps, this->heap);
  machine_process(pheaps, prg);
}

static void import (Machine *this) {
  char *prefix = "";

  char *source = token_string(machine_pop_opt(this, token_STRING));
  if (!source) {
    // Arr<Token>
    Arr *a = token_list(machine_pop_opt(this, token_LIST));
    if (!a)
      fails_types(this, 2, (enum token_Type[]){token_STRING, token_LIST});
    if (arr_size(a) != 2)
      fails_size_list(this, a, 2);
    if (token_type(arr_get(a, 0)) != token_STRING)
      fails_type_list(this, a, 0, token_STRING);
    if (token_type(arr_get(a, 1)) != token_SYMBOL)
      fails_type_list(this, a, 1, token_SYMBOL);

    source = token_string(arr_get(a, 0));
    prefix = token_symbol(arr_get(a, 1));
  }

  // Map<Token>
  Map *fns;

  fns = opt_nget(primitives_module(source));
  if (!fns)
    machine_fail(this, str_f("Memory module '%s' not found", source));

  EACH(fns, Kv, kv) {
    machine_create_var(this, str_cat(prefix, kv_key(kv), NULL), kv_value(kv));
  }_EACH
}

void machine_fail (Machine *this, char *msg) {
  void stack (void) {
    // Arr<Token>
    Arr *st = this->stack;
    int c = 0;
    while (arr_size(st))
      puts(str_f("    (%d): %s", c++, token_to_str(arr_pop(st))));
  }

  puts(str_f("runtime error:%d: %s", this->ix + 1, msg));
  puts("  Stack:");
  stack();

  // Arr<Token>
  Arr *tks = token_list(this->prg);

  void pre (void) {
    int i = this->ix - 1;
    int lim = i - 5;
    lim = lim < 0 ? -1 : lim;
    while (i > lim) {
      puts(str_f("    (%d) %s", i + 1, token_to_str(arr_get(tks, i))));
      --i;
    };
  }
  void tk (void) {
    if (this->ix < arr_size(tks))
      puts(str_f(
        "    --> (%d) %s", this->ix + 1, token_to_str(arr_get(tks, this->ix))
      ));
  }
  void post (void) {
    int i = this->ix + 1;
    int lim = i + 5;
    lim = lim > arr_size(tks) ? arr_size(tks) : lim;
    while (i < lim) {
      puts(str_f("  (%d) %s", i + 1, token_to_str(arr_get(tks, i))));
      ++i;
    }
  }

  puts("  Program:");
  pre();
  tk();
  post();

  exit(0);
}

void machine_push (Machine *this, Token *tk) {
  arr_push(this->stack, tk);
}

Token *machine_pop (Machine *this) {
  Arr *stack = this->stack;
  if (arr_size(stack))
    return arr_pop(stack);
  machine_fail(this, "Stack is empty");
  return NULL; // Unreachable
}

Token *machine_pop_exc (Machine *this, enum token_Type type) {
  Arr *stack = this->stack;
  if (arr_size(stack) && token_type(arr_peek(stack)) == type)
    return arr_pop(stack);
  if (arr_size(stack))
    fails_type(this, type);
  machine_fail(this, "Stack is empty");
  return NULL; // Unreachable
}

Token *machine_pop_opt (Machine *this, enum token_Type type) {
  Arr *stack = this->stack;
  if (arr_size(stack) && token_type(arr_peek(stack)) == type)
    return arr_pop(stack);
  if (arr_size(stack))
    return NULL;
  machine_fail(this, "Stack is empty");
  return NULL; // Unreachable
}

void machine_create_var (Machine *this, char *id, Token *value) {
  if (map_has_key(this->heap, id)) {
    machine_fail(this, str_f(
      "Variable '%s' can not be created, because it is duplicated", id
    ));
  }
  map_put(this->heap, id, value);
}

void machine_set_var (Machine *this, char *id, Token *value) {
  if (map_has_key(this->heap, id)) {
    map_put(this->heap, id, value);
    return;
  }
  machine_fail(this, str_f(
    "Variable '%s' can not be set, because it has not been created", id
  ));
}

void machine_del_var (Machine *this, char *id) {
  map_remove(this->heap, id);
}

Map *machine_process (List *pheaps, Token *prg) {
  Machine *m = MALLOC(Machine);
  m->pheaps = pheaps;
  m->stack = arr_new();
  m->heap = map_new();
  m->prg = prg;
  m->ix = 0;

  EACH_IX(token_list(prg), Token, tk, ix) {
    m->ix = ix;
    if (token_type(tk) == token_SYMBOL) {
      char *symbol = token_symbol(tk);

      if (str_eq(symbol, "run")) run(m);
      else if (str_eq(symbol, "mrun")) mrun(m);
      else if (str_eq(symbol, "import")) import(m);
      else machine_fail(m, str_f("Symbol '%s' not found", symbol));
    } else {
      arr_push(m->stack, tk);
    }
  }_EACH

  return m->heap;
}

