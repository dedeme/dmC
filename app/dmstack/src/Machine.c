// Copyright 27-Aug-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "Machine.h"
#include "Reader.h"
#include "primitives.h"
#include "fails.h"
#include "Heap.h"

struct machine_Machine {
  char *source;
  // Arr<char>
  Arr *imports;
  // List<Machine>
  List *pmachines;
  // Arr<Token>
  Arr *stack;
  Heap *heap;
  Token *prg;
  int ix;
};

char *machine_source (Machine *this) {
  return this->source;
}

// Arr<char>
Arr *machine_imports (Machine *this) {
  return this->imports;
}

// List<Machine>
List *machine_pmachines (Machine *this) {
  return this->pmachines;
}

// Arr<Token>
Arr *machine_stack (Machine *this) {
  return this->stack;
}

Heap *machine_heap (Machine *this) {
  return this->heap;
}

Token *machine_prg (Machine *this) {
  return this->prg;
}

Machine *machine_new_root (void) {
  Machine *this = MALLOC(Machine);
  this->source = "";
  this->imports = arr_new();
  this->pmachines = list_new();
  this->stack = arr_new();
  this->heap = heap_new();
  this->prg = token_new_list(arr_new());
  this->ix = 0;

  void load (char *module) {
    arr_push(this->imports, module);
    EACH(opt_get(primitives_module(module)), Kv, kv) {
      machine_create_var(this, symbol_new(kv_key(kv)), kv_value(kv));
    }_EACH
  }
  load("sys");
  load("str");
  load("lst");
  load("stk");
  load("float");
  load("int");

  return this;
}

// static ---------------------------------------------------------------------

static void run (Machine *this) {
  Token *prg = machine_pop_exc(this, token_LIST);
  machine_process("", list_cons(this->pmachines, this), prg);
}

static void mrun (Machine *this) {
  char *fnc = token_string(machine_pop_exc(this, token_STRING));
  char *module = token_string(machine_pop_exc(this, token_STRING));
  primitives_Fn fn = opt_nget(primitives_get(module, fnc));
  if (fn) fn(this);
  else machine_fail(this, str_f("Symbol '%s.%s' not found", module, fnc));
}

static void xrun (Machine *this) {
  char *command = token_string(machine_pop_exc(this, token_STRING));
  char *r = opt_nget(sys_cmd(command));
  if (r)
    machine_push(
      this, token_new_list(it_to(it_unary(token_new_string(r))))
    );
  else
    machine_push(this, token_new_list(arr_new()));
}

// tp is Tp<List<Machine>, Token>
static void threadfn (Tp *tp) {
  machine_isolate_process("", tp_e1(tp), tp_e2(tp));
}

static void thread (Machine *this) {
  Token *prg = machine_pop_exc(this, token_LIST);
  async_thread_detached(
    (FPROC)threadfn, tp_new(list_cons(this->pmachines, this), prg)
  );
}

static void jthread (Machine *this) {
  Token *prg = machine_pop_exc(this, token_LIST);
  char *th = (char *)async_thread2(
    (FPROC)threadfn, tp_new(list_cons(this->pmachines, this), prg)
  );
  // Arr<Token>
  Arr *a = arr_new();
  arr_push(a, token_new_symbol(symbol_new("thread_to_join")));
  arr_push(a, token_new_string(th));
  arr_push(this->stack, token_new_list(a));
}

static void join (Machine *this) {
  Token *thread = machine_pop_exc(this, token_LIST);
  // Arr<Token>
  Arr *a = token_list(thread);
  if (
    arr_size(a) != 2 ||
    (token_type(arr_get(a, 0)) != token_SYMBOL &&
      !str_eq(symbol_name(token_symbol(arr_get(a, 0))), "thread_to_join")
    )
  )
    machine_fail(this, "Expected a thread value");
  async_join((pthread_t *)token_string(arr_get(a, 1)));
}

static void sif (Machine *this) {
  // List<Machine>
  List *pmachines = list_cons(this->pmachines, this);

  Token *tk = machine_pop_opt(this, token_LIST);
  if (tk) {
    Token *prg = machine_pop_exc(this, token_LIST);
    machine_process("", pmachines, tk);
    int op = token_int(machine_pop_exc(this, token_INT));
    if (op) machine_process("", pmachines, prg);
    return;
  }
  tk = machine_pop_opt(this, token_SYMBOL);
  if (tk) {
    if (str_eq(symbol_name(token_symbol(tk)), "else")) {
      for (;;) {
        Token *cond = machine_pop_exc(this, token_LIST);
        Token *prg = machine_pop_exc(this, token_LIST);
        machine_process("", pmachines, cond);
        int op = token_int(machine_pop_exc(this, token_INT));
        if (op) {
          for (;;) { // Consume else's
            tk = machine_pop_opt(this, token_LIST);
            if (tk) break;
            tk = machine_pop_exc(this, token_SYMBOL);
            if (tk) {
              if (str_eq(symbol_name(token_symbol(tk)), "else")) {
                machine_pop_exc(this, token_LIST);
                machine_pop_exc(this, token_LIST);
                continue;
              }
              machine_fail(this, str_f(
                "Stack pop: Expected 'else', found '%s'", token_symbol(tk)
              ));
            }
            machine_fail(this, str_f(
              "Stack pop: Expected 'else' or token of type 'List', "
              "found type '%s'",
              token_type_to_str(token_type(arr_peek(machine_stack(this))))
            ));
          }
          machine_process("", pmachines, prg);
          break;
        } else {
          tk = machine_pop_opt(this, token_LIST);
          if (tk) {
            machine_process("", pmachines, tk);
            break;
          }
          tk = machine_pop_exc(this, token_SYMBOL);
          if (tk) {
            if (str_eq(symbol_name(token_symbol(tk)), "else"))
              continue;
            machine_fail(this, str_f(
              "Stack pop: Expected 'else', found '%s'", token_symbol(tk)
            ));
          }
          machine_fail(this, str_f(
            "Stack pop: Expected 'else' or token of type 'List', "
            "found type '%s'",
            token_type_to_str(token_type(arr_peek(machine_stack(this))))
          ));
        }
      }
      return;
    }
    machine_fail(this, str_f(
      "Stack pop: Expected 'else', found '%s'", token_symbol(tk)
    ));
  }
  machine_fail(this, str_f(
    "Stack pop: Expected 'else' or token of type 'List', found type '%s'",
    token_type_to_str(token_type(arr_peek(machine_stack(this))))
  ));
}

static void loop (Machine *this) {
  Token *prg = machine_pop_exc(this, token_LIST);
  // List<Machine>
  List *pmachines = list_cons(this->pmachines, this);

  for (;;) {
    machine_process("", pmachines, prg);
    if (arr_size(this->stack)) {
      Token *tk = machine_pop_opt(this, token_SYMBOL);
      if (tk) {
        if (str_eq(symbol_name(token_symbol(tk)), "break")) break;
        machine_push(this, tk);
      }
    }
  }
}

static void swhile (Machine *this) {
  Token *cond = machine_pop_exc(this, token_LIST);
  Token *prg = machine_pop_exc(this, token_LIST);
  // List<Machine>
  List *pmachines = list_cons(this->pmachines, this);

  for (;;) {
    machine_process("", pmachines, cond);
    if (token_int(machine_pop_exc(this, token_INT))) {
      machine_process("", pmachines, prg);
      if (arr_size(this->stack)) {
        Token *tk = machine_pop_opt(this, token_SYMBOL);
        if (tk) {
          if (str_eq(symbol_name(token_symbol(tk)), "break")) break;
          machine_push(this, tk);
        }
      }
    } else {
      break;
    }
  }
}

static void sfor (Machine *this) {
  Token *cond = machine_pop_exc(this, token_LIST);
  Token *prg = machine_pop_exc(this, token_LIST);

  // Arr<Token>
  Arr *a = token_list(cond);
  int size = arr_size(a);
  if (size < 2)
    machine_fail(this,
      "Expected al least one symbol and one integer in for loop");
  if (size > 4)
    machine_fail(this,
      "Expected as much one symbol and three integers in for loop");
  if (token_type(arr_get(a, 0)) != token_SYMBOL)
    machine_fail(this, "Expected a symbol as first element in for loop");
  Symbol *var = token_symbol(arr_get(a, 0));
  if (token_type(arr_get(a, 1)) != token_LIST)
    machine_fail(this, "Expected a list as first value in for loop");

  // List<Machine>
  List *pmachines = list_cons(this->pmachines, this);

  machine_process("", pmachines, arr_get(a, 1));
  int begin = token_int(machine_pop_exc(this, token_INT));
  int end = begin;
  if (size > 2) {
    if (token_type(arr_get(a, 2)) != token_LIST)
      machine_fail(this, "Expected a list as second value in for loop");
    machine_process("", pmachines, arr_get(a, 2));
    end = token_int(machine_pop_exc(this, token_INT));
  } else {
    begin = 0;
  }
  int step = 1;
  if (size == 4) {
    if (token_type(arr_get(a, 3)) != token_LIST)
      machine_fail(this, "Expected a list as third value in for loop");
    machine_process("", pmachines, arr_get(a, 3));
    step = token_int(machine_pop_exc(this, token_INT));
    if (step == 0)
      machine_fail(this, "No valid '0' value as step in for loop");
  }

  machine_create_var(this, var, token_new_int(begin));

  for (;;) {
    if (step > 0 && begin >= end) break;
    else if (step < 0 && begin <= end) break;

    machine_process("", pmachines, prg);
    if (arr_size(this->stack)) {
      Token *tk = machine_pop_opt(this, token_SYMBOL);
      if (tk) {
        if (str_eq(symbol_name(token_symbol(tk)), "break")) break;
        machine_push(this, tk);
      }
    }
    Token *begintk = machine_get_var(this, var);
    if (token_type(begintk) != token_INT)
      machine_fail(this, str_f(
        "Loop variable '%s' changed to type '%s'",
        var, token_type_to_str(token_type(begintk))
      ));
    begin = token_int(begintk) + step;
    machine_set_var(this, var, token_new_int(begin));
  }

  heap_remove(this->heap, var);
}

static void import (Machine *this) {
  char *source = "";

  Token *tk = machine_pop_exc(this, token_STRING);
  source = token_string(tk);

  EACHL(list_cons(this->pmachines, this), Machine, m) {
    EACH(m->imports, char, im) {
      if (str_eq(im, source)) return;
    }_EACH
  }_EACH

  if (
    *source == '/' ||
    (*source == '.' && source[1] == '/') ||
    (*source == '.' && source[1] == '.' && source[2] == '/')
  )  {
    char *f = str_cat(source, ".dms", NULL);
    if (!file_exists(f))
      machine_fail(this, str_f("Import file '%s' not found.", f));
    Reader *r = reader_new(f, file_read(f), 1, 0);
    Machine *m = machine_process(
      path_cat(path_parent(this->source), source, NULL),
      list_cons(this->pmachines, this), reader_process(r)
    );

    EACH(heap_entries(m->heap), HeapEntry, e) {
      machine_create_var(this, heapEntry_symbol(e), heapEntry_token(e));
    }_EACH
  } else {
    // Map<Token>
    Map *fns;
    fns = opt_nget(primitives_module(source));
    if (!fns)
      machine_fail(this, str_f("Memory module '%s' not found", source));

    EACH(fns, Kv, kv) {
      machine_create_var(this, symbol_new(kv_key(kv)), kv_value(kv));
    }_EACH
  }

  arr_push(this->imports, source);
}

static void sexit (Machine *this) {
  void stack (void) {
    // Arr<Token>
    Arr *st = this->stack;
    if (!arr_size(st)) {
      puts("    [EMPTY]");
      return;
    }
    printf("    ");
    while (arr_size(st))
      printf("%s ", token_to_str(arr_pop(st)));
    puts("");
  }

  puts("  Stack:");
  stack();

  int c = 1;

  EACHL(
    list_tail(list_reverse(list_cons(this->pmachines, this))),
    Machine, m
  ) {
    // Arr<Token>
    Arr *tks = token_list(m->prg);

    void pre (void) {
      printf("  | ");
      int lim = m->ix;
      int i = lim - 5;
      i = i < 0 ? 0 : i;
      while (i < lim)
        printf("%s ", token_to_str(arr_get(tks, i++)));
      puts("");
    }
    void tk (void) {
      if (m->ix < arr_size(tks))
        puts(str_f("  | %s", token_to_str(arr_get(tks, m->ix))));
    }
    void post (void) {
      printf("  | ");
      int i = m->ix + 1;
      int lim = i + 5;
      lim = lim > arr_size(tks) ? arr_size(tks) : lim;
      while (i < lim)
        printf("%s ", token_to_str(arr_get(tks, i++)));
      puts("");
    }

    printf("  Program %d:\n", c++);
    pre();
    tk();
    post();
  }_EACH

  exit(0);
}

static void sassert (Machine *this) {
  Token *tk = machine_pop_exc(this, token_STRING);
  char *msg = token_string(tk);
  tk = machine_pop_exc(this, token_INT);
  if (token_int(tk)) return;
  if (msg)
    puts(str_f("%s: Assert error", msg));
  sexit(this);
}

void machine_fail (Machine *this, char *msg) {
  puts(str_f("runtime error:%d: %s", list_count(this->pmachines) + 1, msg));
  sexit(this);
}

void machine_push (Machine *this, Token *tk) {
  arr_push(this->stack, tk);
}

Token *machine_peek (Machine *this) {
  Arr *stack = this->stack;
  if (arr_size(stack))
    return arr_peek(stack);
  machine_fail(this, "Stack is empty");
  return NULL; // Unreachable
}

Token *machine_peek_exc (Machine *this, enum token_Type type) {
  Arr *stack = this->stack;
  if (arr_size(stack) && token_type(arr_peek(stack)) == type)
    return arr_peek(stack);
  if (arr_size(stack))
    fails_type(this, type);
  machine_fail(this, "Stack is empty");
  return NULL; // Unreachable
}

Token *machine_peek_opt (Machine *this, enum token_Type type) {
  Arr *stack = this->stack;
  if (arr_size(stack) && token_type(arr_peek(stack)) == type)
    return arr_peek(stack);
  if (arr_size(stack))
    return NULL;
  machine_fail(this, "Stack is empty");
  return NULL; // Unreachable
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

Token *machine_get_var (Machine *this, Symbol *id) {
  EACHL(list_cons(this->pmachines, this), Machine, m) {
    Token *r = heap_get(m->heap, id);
    if (r) return r;
  }_EACH
  machine_fail(this, str_f("Variable '%s' not found", symbol_name(id)));
  return NULL; // Unreachable
}

void machine_create_var (Machine *this, Symbol *id, Token *value) {
  Token *r = heap_get(this->heap, id);
  if (r)
    machine_fail(this, str_f(
      "Duplicate variable '%s' can not be created", symbol_name(id)
    ));
  heap_add(this->heap, id, value);
}

void machine_set_var (Machine *this, Symbol *id, Token *value) {
  EACHL(list_cons(this->pmachines, this), Machine, m) {
    if (heap_set(m->heap, id, value)) return;
  }_EACH
  machine_fail(this, str_f(
    "Not created variable '%s' can not be set", symbol_name(id)
  ));
}

static Machine *cprocess (Machine *m) {
  fails_register_machine(m);

  Symbol *sym = NULL;
  EACH_IX(token_list(m->prg), Token, tk, ix) {
    m->ix = ix;

    if (sym) {
      if (token_type(tk) == token_SYMBOL) {
        char *symbol = symbol_name(token_symbol(tk));
        if (str_eq(symbol, "=")) {
          machine_create_var(m, sym, machine_pop(m));
          sym = NULL;
          continue;
        }
        if (str_eq(symbol, "#")) {
          machine_set_var(m, sym, machine_pop(m));
          sym = NULL;
          continue;
        }
        if (str_eq(symbol, "&")) {
          arr_push(m->stack, machine_get_var(m, sym));
          sym = NULL;
          continue;
        }
      }

      Token *t = machine_get_var(m, sym);
      arr_push(m->stack, t);
      char sname = *symbol_name(sym);
      sym = NULL;
      if (
        token_type(t) == token_LIST &&
        (sname > 'Z' || sname < 'A')
      ) run(m);
    }

    if (token_type(tk) == token_SYMBOL) {
      Symbol *symbol = token_symbol(tk);
      char *name = symbol_name(symbol);

      if (str_eq(name, "nop")) continue;
      else if (str_eq(name, "run")) run(m);
      else if (str_eq(name, "mrun")) mrun(m);
      else if (str_eq(name, "xrun")) xrun(m);
      else if (str_eq(name, "thread")) thread(m);
      else if (str_eq(name, "jthread")) jthread(m);
      else if (str_eq(name, "join")) join(m);
      else if (str_eq(name, "else")) arr_push(m->stack, tk);
      else if (str_eq(name, "if")) sif(m);
      else if (
        str_eq(name, "return") ||
        str_eq(name, "break")
      ) {
        if ("break") arr_push(m->stack, token_new_symbol(symbol_new("break")));
        return m;
      }
      else if (str_eq(name, "loop")) loop(m);
      else if (str_eq(name, "while")) swhile(m);
      else if (str_eq(name, "for")) sfor(m);
      else if (str_eq(name, "import")) import(m);
      else if (str_eq(name, "assert")) sassert(m);
      else sym = symbol;
    } else {
      arr_push(m->stack, tk);
    }
  }_EACH

  if (sym) {
    Token *t = machine_get_var(m, sym);
    arr_push(m->stack, t);
    if (token_type(t) == token_LIST) run(m);
  }

  fails_unregister_machine();
  return m;
}

// pmachines is List<Machine>
Machine *machine_process (char *source, List *pmachines, Token *prg) {
  Machine *m = MALLOC(Machine);
  m->source = source;
  m->imports = arr_new();
  m->pmachines = pmachines;
  m->stack = list_empty(pmachines)
    ? arr_new()
    : ((Machine *)list_head(pmachines))->stack
  ;
  m->heap = heap_new();
  m->prg = prg;
  m->ix = 0;

  return cprocess(m);
}

// pmachines is List<Machine>
Machine *machine_isolate_process (char *source, List *pmachines, Token *prg) {
  Machine *m = MALLOC(Machine);
  m->source = source;
  m->imports = arr_new();
  m->pmachines = pmachines;
  m->stack = arr_new();
  m->heap = heap_new();
  m->prg = prg;
  m->ix = 0;

  return cprocess(m);
}


