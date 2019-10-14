// Copyright 27-Aug-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "Machine.h"
#include "Reader.h"
#include "imports.h"
#include "primitives.h"
#include "fails.h"
#include "Heap.h"
#include "types.h"
#include "DEFS.h"

struct machine_Machine {
  char *source;
  // List<Machine>
  List *pmachines;
  // Arr<Token>
  Arr *stack;
  Heap *heap;
  Token *prg;
  int ix;
};

static char *machine_source (Machine *this) {
  if (*this->source) return this->source;
  // List<Machine>
  List *pms = this->pmachines;
  while (!*((Machine *)list_head(pms))->source) pms = list_tail(pms);
  return ((Machine *)list_head(pms))->source;
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

// static ---------------------------------------------------------------------

static void eval (Machine *this) {
  char *p = token_string(machine_pop_exc(this, token_STRING));
  Reader *r = reader_new("[EVAL]", p);
  machine_process("", this->pmachines, reader_process(r));
}

static void run (Machine *this) {
  Token *prg = machine_pop_exc(this, token_LIST);
  machine_process("", this->pmachines, prg);
}

static void runmod (Machine *this, char *module) {
  Token *prg = machine_pop_exc(this, token_LIST);
  Token *runner = token_new_list(0, arr_new_from(
    prg, token_new_symbol(0, symbol_new("run")), NULL
  ));
  machine_process(str_cat(module, ".dms", NULL), this->pmachines, runner);
}

static void mrun (Machine *this) {
  // Arr<Token>
  Arr *a = token_list(machine_pop_exc(this, token_LIST));
  Token *module = *arr_start(a);
  if (token_type(module) != token_SYMBOL)
    fails_type_in(this, token_SYMBOL, module);
  Symbol modules = token_symbol(module);
  Token *fnc = *(arr_start(a) + 1);
  if (token_type(fnc) != token_SYMBOL)
    fails_type_in(this, token_SYMBOL, fnc);
  Symbol fncs = token_symbol(fnc);

  pmodule_Fn fn = opt_nget(primitives_get(modules, fncs));
  if (fn) fn(this);
  else machine_fail(this, str_f(
    "Symbol '%s.%s' not found", symbol_to_str(modules), symbol_to_str(fncs)
  ));
}

static void data (Machine *this) {
  Token *prg = machine_pop_exc(this, token_LIST);
  Machine *m = machine_isolate_process("", this->pmachines, prg);
  machine_push(this, token_new_list(0, m->stack));
}

static pthread_mutex_t mutex;
static void sync (Machine *this) {
  Token *prg = machine_pop_exc(this, token_LIST);
  pthread_mutex_lock(&mutex);
  machine_process("", this->pmachines, prg);
  pthread_mutex_unlock(&mutex);
}

static void elif (Machine *this) {
  Token *prg_not = machine_pop_exc(this, token_LIST);
  Token *prg_yes = machine_pop_exc(this, token_LIST);
  if (token_int(machine_pop_exc(this, token_INT)))
      machine_process("", this->pmachines, prg_yes);
  else
      machine_process("", this->pmachines, prg_not);
}

static void sif (Machine *this) {
  Token *prg = machine_pop_opt(this, token_LIST);
  if (prg) {
    Token *tk = machine_pop_opt(this, token_INT);
    if (tk) {
      if (token_int(tk)) machine_process("", this->pmachines, prg);
      return;
    }
  } else {
    Token *tk = machine_peek_opt(this, token_SYMBOL);
    if (!tk || token_symbol(tk) != symbol_ELSE)
      machine_fail(this, "Expected List or 'else'");
  }

  // Arr<Token>
  Arr *a = arr_new();
  Token *tk = machine_peek_opt(this, token_SYMBOL);
  while (tk && token_symbol(tk) == symbol_ELSE) {
    machine_pop(this);
    arr_push(a, machine_pop_exc(this, token_LIST));
    arr_push(a, machine_pop_exc(this, token_LIST));
    tk = arr_size(this->stack) ? machine_peek_opt(this, token_SYMBOL) : NULL;
  }

  if (arr_size(a)) {
    int rprg = 1;
    while (arr_size(a) > 0) {
      machine_process("", this->pmachines, arr_pop(a));
      if (token_int(machine_pop_exc(this, token_INT))) {
        machine_process("", this->pmachines, arr_pop(a));
        rprg = 0;
        break;
      }
      arr_pop(a);
    }
    if (rprg && prg) machine_process("", this->pmachines, prg);
  } else {
    if (!prg)
      machine_fail(this, "Expected List");
  }
}

static void loop (Machine *this) {
  Token *prg = machine_pop_exc(this, token_LIST);

  for (;;) {
    machine_process("", this->pmachines, prg);
    if (arr_size(this->stack)) {
      Token *tk = machine_peek_opt(this, token_SYMBOL);
      if (tk && token_symbol(tk) == symbol_BREAK) {
        machine_pop(this);
        break;
      }
    }
  }
}

static void swhile (Machine *this) {
  Token *prg = machine_pop_exc(this, token_LIST);
  Token *cond = machine_pop_exc(this, token_LIST);

  for (;;) {
    machine_process("", this->pmachines, cond);
    if (token_int(machine_pop_exc(this, token_INT))) {
      machine_process("", this->pmachines, prg);
      if (arr_size(this->stack)) {
        Token *tk = machine_peek_opt(this, token_SYMBOL);
        if (tk && token_symbol(tk) == symbol_BREAK) {
          machine_pop(this);
          break;
        }
      }
    } else {
      break;
    }
  }
}

static void sfor (Machine *this) {
  Token *prg = machine_pop_exc(this, token_LIST);
  Token *cond = machine_pop_opt(this, token_INT);
  if (cond) {
    for (Int i = 0; i < token_int(cond); ++i) {
      machine_push(this, token_new_int(0, i));
      machine_process("", this->pmachines, prg);
      if (arr_size(this->stack)) {
        Token *tk = machine_peek_opt(this, token_SYMBOL);
        if (tk && token_symbol(tk) == symbol_BREAK) {
          machine_pop(this);
          break;
        }
      }
    }
    return;
  }

  cond = machine_pop_opt(this, token_LIST);

  if (!cond) fails_types(this, 2, (enum token_Type[]){token_INT, token_LIST});

  Machine *m2 = machine_isolate_process("", this->pmachines, cond);

  // Arr<Token>
  Arr *a = m2->stack;
  int size = arr_size(a);
  if (size < 1)
    machine_fail(this,
      "Expected al least one one value in 'for'");
  if (size > 4)
    machine_fail(this,
      "Expected as much three values in 'for'");

  Token *tk = arr_get(a, 0);
  if (token_type(tk) != token_INT)
    machine_fail(this, "Expected an Int as first value in 'for'");

  int begin = token_int(tk);
  int end = begin;
  if (size > 1) {
    Token *tk = arr_get(a, 1);
    if (token_type(tk) != token_INT)
      machine_fail(this, "Expected an Int as second value in 'for'");
    end = token_int(tk);
  } else {
    begin = 0;
  }
  int step = 1;
  if (size == 3) {
    Token *tk = arr_get(a, 2);
    if (token_type(tk) != token_INT)
      machine_fail(this, "Expected an Int as third value in 'for'");
    step = token_int(tk);
    if (step == 0)
      machine_fail(this, "No valid '0' value as step in 'for'");
  }

  for (;;) {
    if (step > 0 && begin >= end) break;
    else if (step < 0 && begin <= end) break;
    machine_push(this, token_new_int(0, begin));
    machine_process("", this->pmachines, prg);
    if (arr_size(this->stack)) {
      Token *tk = machine_peek_opt(this, token_SYMBOL);
      if (tk && token_symbol(tk) == symbol_BREAK) {
        machine_pop(this);
        break;
      }
    }
    begin += step;
  }
}

static void import (Machine *this) {
  Token *tk = machine_pop(this);
  // Kv<SymbolKv>
  Kv *errorsymkv = imports_read_symbol(tk);
  if (*kv_key(errorsymkv)) machine_fail(this, kv_key(errorsymkv));
  Symbol source = symbolKv_value(kv_value(errorsymkv));

  char *sid = symbol_to_str(source);
  char *f = str_cat(sid, ".dms", NULL);
  if (*sid != '/')
    f = path_cat(path_parent(machine_source(this)), f, NULL);

  char *fc = opt_nget(path_canonical(f));
  if (!fc)
    machine_fail(this, str_f("Import file '%s' not found.", f));

  char *fid = str_left(fc, -4);
  Symbol ssource = symbol_new(fid);

  if (!imports_is_on_way(ssource) && !imports_get(ssource)) {
    imports_put_on_way(ssource);

    Reader *r = reader_new(fid, file_read(fc));
    Machine *m = machine_isolate_process(
      fc, this->pmachines, reader_process(r)
    );

    imports_add(ssource, m->heap);

    imports_quit_on_way(ssource);
  }
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

  EACHL(this->pmachines, Machine, m) {
    Token *tk = arr_get(token_list(m->prg), m->ix);
    printf(
      "%s:%d:%s\n",
      machine_source(m), token_line(tk),
      token_to_str(tk)
    );
  }_EACH

  exit(0);
}

static void sassert (Machine *this) {
  if (token_int(machine_pop_exc(this, token_INT))) return;
  puts(str_f("Assert error:%d:", list_count(this->pmachines) + 1));
  sexit(this);
}

static void expect (Machine *this) {
  Token *expected = machine_pop(this);
  Token *actual = machine_pop(this);
  if (token_eq(expected, actual)) return;
  puts(str_f(
    "Expected error: %d:\n  Expected: %s\n  Actual  : %s",
    list_count(this->pmachines) + 1,
    token_to_str(expected), token_to_str(actual)
  ));
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
  if (arr_size(stack)){
    Token *tk = arr_peek(stack);
    return token_type(tk) == type ? tk : NULL;
  }
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

static void cprocess (Machine *m) {
  fails_register_machine(m);

  Symbol module = -1;
  Heap *moduleh = NULL;
  Symbol sym = -1;

  EACH_IX(token_list(m->prg), Token, tk, ix) {
    m->ix = ix;

    if (sym != -1) {
      Token *t;
      if (module != -1) {
        t = heap_get(moduleh, sym);
        if (t) {
          if (token_type(tk) == token_SYMBOL) {
            Symbol symbol = token_symbol(tk);
            if (symbol == symbol_EQUALS || symbol == symbol_FUNCTION)
              machine_fail(m, "Imported symbols can not be set");
            if (symbol == symbol_AMPERSAND) {
              arr_push(m->stack, t);
              module = -1;
              sym = -1;
              continue;
            }
          }
        } else {
          machine_fail(m, str_f(
            "Symbol '%s %s' not found",
            symbol_to_str(module), symbol_to_str(sym)
          ));
        }
      } else {
        t = heap_get(m->heap, sym);
        if (!t) t = heap_get(imports_base(), sym);
        if (t) {
          if (token_type(tk) == token_SYMBOL) {
            Symbol symbol = token_symbol(tk);
            if (symbol == symbol_EQUALS) {
              Token *tk = machine_pop(m);
              if (token_type(tk) == token_LIST) {
                char *name = symbol_to_str(sym);
                if (*name < 'A' || *name > 'Z')
                  machine_fail(m, str_f(
                    "'%s': List or object name must start with uppercase [A-Z]",
                    name
                  ));
              }
              heap_add(m->heap, sym, tk);
              sym = -1;
              continue;
            }
            if (symbol == symbol_FUNCTION) {
              Token *tk = machine_pop(m);
              if (token_type(tk) != token_LIST)
                machine_fail(m, "Symbol => not used with function");
              char *name = symbol_to_str(sym);
              if (*name >= 'A' && *name <= 'Z')
                machine_fail(m, str_f(
                  "'%s': Function name must not start with uppercase [A-Z]",
                  name
                ));
              heap_add(m->heap, sym, tk);
              sym = -1;
              continue;
            }
            if (symbol == symbol_AMPERSAND) {
              arr_push(m->stack, t);
              sym = -1;
              continue;
            }
          }
        } else {
          if (token_type(tk) == token_SYMBOL) {
            Symbol symbol = token_symbol(tk);
            if (symbol == symbol_EQUALS) {
              Token *tk = machine_pop(m);
              if (token_type(tk) == token_LIST) {
                char *name = symbol_to_str(sym);
                if (*name < 'A' || *name > 'Z')
                  machine_fail(m, str_f(
                    "'%s': List or object name must start with uppercase [A-Z]",
                    name
                  ));
              }
              heap_add(m->heap, sym, tk);
              sym = -1;
              continue;
            }
            if (symbol == symbol_FUNCTION) {
              Token *tk = machine_pop(m);
              if (token_type(tk) != token_LIST)
                machine_fail(m, "Symbol => not used with function");
              char *name = symbol_to_str(sym);
              if (*name >= 'A' && *name <= 'Z')
                machine_fail(m, str_f(
                  "'%s': Function name must not start with uppercase [A-Z]",
                  name
                ));
              heap_add(m->heap, sym, tk);
              sym = -1;
              continue;
            }
          }

          // List<Machine>
          List *pms = m->pmachines;
          for (;;) {
            if (list_empty(pms)) break;
            Machine *mch = list_head(pms);
            t = heap_get(mch->heap, sym);
            if (t) break;
            pms = list_tail(pms);
          }

          if (t) {
            if (token_type(tk) == token_SYMBOL) {
              Symbol symbol = token_symbol(tk);
              if (symbol == symbol_AMPERSAND) {
                arr_push(m->stack, t);
                module = -1;
                sym = -1;
                continue;
              }
            }
          } else {
            // List <Machines>
            List *pms = m->pmachines;
            while (!*((Machine *)list_head(pms))->source) pms = list_tail(pms);
            char *msg = "";
            if (heap_get(((Machine *)list_head(pms))->heap, sym))
              msg = "\n(Top symbols can no be referenced out of top scope. "
                    "Use 'this'.)";
            m->ix--;
            machine_fail(m, str_f(
              "Symbol '%s' not found%s", symbol_to_str(sym), msg
            ));
          }
        }
      }

      char sname = *symbol_to_str(sym);
      arr_push(m->stack, t);
      if (
        token_type(t) == token_LIST &&
        (sname > 'Z' || sname < 'A')
      ) {
        m->ix--;
        if (
          module != -1 && *symbol_to_str(module) == '/'
        ) runmod(m, symbol_to_str(module));
        else run(m);
        m->ix++;
      }
      module = -1;
      sym = -1;
    }

    if (module != -1) {
      if (token_type(tk) != token_SYMBOL)
        machine_fail(m, str_f(
          "'%s' is not a module symbol", symbol_to_str(module)
        ));
      sym = token_symbol(tk);
      continue;
    }

    if (token_type(tk) == token_SYMBOL) {
      Symbol symbol = token_symbol(tk);

      if (symbol == symbol_NOP) continue;
      else if (symbol == symbol_EVAL) eval(m);
      else if (symbol == symbol_RUN) run(m);
      else if (symbol == symbol_MRUN) mrun(m);
      else if (symbol == symbol_DATA) data(m);
      else if (symbol == symbol_SYNC) sync(m);
      else if (symbol == symbol_ELIF) elif(m);
      else if (symbol == symbol_ELSE) machine_push(m, tk);
      else if (symbol == symbol_IF) sif(m);
      else if (symbol == symbol_BREAK) {
        arr_push(m->stack, tk);
        fails_unregister_machine();
        return;
      }
      else if (symbol == symbol_LOOP) loop(m);
      else if (symbol == symbol_WHILE) swhile(m);
      else if (symbol == symbol_FOR) sfor(m);
      else if (symbol == symbol_IMPORT) import(m);
      else if (symbol == symbol_ASSERT) sassert(m);
      else if (symbol == symbol_EXPECT) expect(m);
      else if (symbol == symbol_STACK) types_fail(m);
      else if (symbol == symbol_STACK_CHECK) types_check(m);
      else  {
        Heap *h = imports_get(symbol);
        if (h) {
          module = symbol;
          moduleh = h;
        } else {
          sym = symbol;
        }
      }
    } else {
      arr_push(m->stack, tk);
    }
  }_EACH

  if (sym != -1) {
    Token *t;
    if (module != -1) {
      t = heap_get(moduleh, sym);
      if (!t) {
        machine_fail(m, str_f(
          "Symbol '%s %s' not found", symbol_to_str(module), symbol_to_str(sym)
        ));
      }
    } else {
      t = heap_get(m->heap, sym);
      if (!t) t = heap_get(imports_base(), sym);
      if (!t) {
        // List<Machine>
        List *pms = m->pmachines;
        for (;;) {
          if (list_empty(pms)) break;
          Machine *mch = list_head(pms);
          t = heap_get(mch->heap, sym);
          if (t) break;
          pms = list_tail(pms);
        }
      }
      if (!t) {
        m->ix--;
        machine_fail(m, str_f(
          "Symbol '%s' not found", symbol_to_str(sym)
        ));
      }
    }

    arr_push(m->stack, t);
    char sname = *symbol_to_str(sym);
    if (
      token_type(t) == token_LIST &&
      (sname > 'Z' || sname < 'A')
    ) {
      if (
        module != -1 && *symbol_to_str(module) == '/'
      ) runmod(m, symbol_to_str(module));
      else run(m);
    }
    module = -1;
    sym = -1;
  } else if (module != -1) {
    machine_fail(m, str_f(
      "Expected symbol of module '%s'", symbol_to_str(module)
    ));
  }

//if (!*m->source) exit(0);
  fails_unregister_machine();
}

// pmachines is List<Machine>
Machine *machine_process (char *source, List *pmachines, Token *prg) {
  Machine *m = MALLOC(Machine);
  m->source = source;
  m->pmachines = list_cons(pmachines, m);
  m->stack = list_empty(pmachines)
    ? arr_new()
    : ((Machine *)list_head(pmachines))->stack
  ;
  m->heap = heap_new();
  m->prg = prg;
  m->ix = 0;

  cprocess(m);
  return m;
}

// pmachines is List<Machine>
Machine *machine_isolate_process (char *source, List *pmachines, Token *prg) {
  Machine *m = MALLOC(Machine);
  m->source = source;
  m->pmachines = list_cons(pmachines, m);
  m->stack = arr_new();
  m->heap = heap_new();
  m->prg = prg;
  m->ix = 0;

  if (*source)
    imports_add(symbol_new(str_left(source, -4)), m->heap);

  cprocess(m);
  return m;
}
