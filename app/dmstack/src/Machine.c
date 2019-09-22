// Copyright 27-Aug-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "Machine.h"
#include "Reader.h"
#include "imports.h"
#include "primitives.h"
#include "modules.h"
#include "fails.h"
#include "Heap.h"
#include "DEFS.h"

struct machine_Machine {
  char *sourcex;
  // List<Machine>
  List *pmachines;
  // Arr<Token>
  Arr *stack;
  Heap *heap;
  Token *prg;
  int ix;
};

static char *machine_source (Machine *this) {
  if (*this->sourcex) return this->sourcex;
  // List<Machine>
  List *pms = this->pmachines;
  while (!*((Machine *)list_head(pms))->sourcex) pms = list_tail(pms);
  return ((Machine *)list_head(pms))->sourcex;
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
  Reader *r = reader_new("[EVAL]", p, 1, 0);
  machine_process("", this->pmachines, reader_process(r));
}

static void run (Machine *this) {
  Token *prg = machine_pop_exc(this, token_LIST);
  machine_process("", this->pmachines, prg);
}

static void mrun (Machine *this) {
  char *fnc = token_string(machine_pop_exc(this, token_STRING));
  char *module = token_string(machine_pop_exc(this, token_STRING));
  primitives_Fn fn = opt_nget(primitives_get(module, fnc));
  if (fn) fn(this);
  else machine_fail(this, str_f("Symbol '%s.%s' not found", module, fnc));
}

static void sif (Machine *this) {
  // Arr<Token>
  Arr *a = arr_new();
  arr_push(a, machine_pop_exc(this, token_LIST));
  arr_push(a, machine_pop_exc(this, token_LIST));
  Token *last = NULL;
  for (;;) {
    if (!arr_size(this->stack)) break;
    Token *tk = machine_pop_opt(this, token_SYMBOL);
    if (tk) {
      char *name = symbol_name(token_symbol(tk));
      if (str_eq(name, "elif")) {
        arr_push(a, machine_pop_exc(this, token_LIST));
        arr_push(a, machine_pop_exc(this, token_LIST));
        continue;
      } else if (str_eq(name, "else")) {
        last = machine_pop_exc(this, token_LIST);
      } else {
        machine_fail(this, "Expected 'else' or condition");
      }
    }
    break;
  }
  Token **es = (Token **)arr_start(a);
  REPEAT(arr_size(a) / 2) {
    machine_process("", this->pmachines, *es++);
    if (token_int(machine_pop_exc(this, token_INT))) {
      machine_process("", this->pmachines, *es);
      return;
    }
    ++es;
  }_REPEAT
  if (last) machine_process("", this->pmachines, last);
}

static void loop (Machine *this) {
  Token *prg = machine_pop_exc(this, token_LIST);

  for (;;) {
    machine_process("", this->pmachines, prg);
    if (arr_size(this->stack)) {
      Token *tk = machine_peek_opt(this, token_SYMBOL);
      if (tk) {
        if (str_eq(symbol_name(token_symbol(tk)), "break")) {
          machine_pop(this);
          break;
        }
      }
    }
  }
}

static void swhile (Machine *this) {
  Token *cond = machine_pop_exc(this, token_LIST);
  Token *prg = machine_pop_exc(this, token_LIST);

  for (;;) {
    machine_process("", this->pmachines, cond);
    if (token_int(machine_pop_exc(this, token_INT))) {
      machine_process("", this->pmachines, prg);
      if (arr_size(this->stack)) {
        Token *tk = machine_peek_opt(this, token_SYMBOL);
        if (tk) {
          if (str_eq(symbol_name(token_symbol(tk)), "break")) {
            machine_pop(this);
            break;
          }
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
  if (size < 1)
    machine_fail(this,
      "Expected al least one one value in 'for'");
  if (size > 4)
    machine_fail(this,
      "Expected as much three values in 'for'");

  if (token_type(arr_get(a, 0)) != token_LIST)
    machine_fail(this, "Expected a list as first value in 'for'");

  machine_process("", this->pmachines, arr_get(a, 0));
  int begin = token_int(machine_pop_exc(this, token_INT));
  int end = begin;
  if (size > 1) {
    if (token_type(arr_get(a, 1)) != token_LIST)
      machine_fail(this, "Expected a list as second value in 'for'");
    machine_process("", this->pmachines, arr_get(a, 1));
    end = token_int(machine_pop_exc(this, token_INT));
  } else {
    begin = 0;
  }
  int step = 1;
  if (size == 3) {
    if (token_type(arr_get(a, 2)) != token_LIST)
      machine_fail(this, "Expected a list as third value in 'for'");
    machine_process("", this->pmachines, arr_get(a, 2));
    step = token_int(machine_pop_exc(this, token_INT));
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
      if (tk) {
        if (str_eq(symbol_name(token_symbol(tk)), "break")) {
          machine_pop(this);
          break;
        }
      }
    }
    begin += step;
  }
}

static void import (Machine *this) {
  Token *tk = machine_pop(this);
  Kv *errorsym = imports_read_symbol(tk);
  if (*kv_key(errorsym)) machine_fail(this, kv_key(errorsym));
  Symbol *source = kv_value(errorsym);

  char *sid = symbol_id(source);
  char *f = str_cat(sid, ".dms", NULL);
  if (*sid != '/')
    f = path_cat(path_parent(machine_source(this)), f, NULL);

  char *fc = opt_nget(path_canonical(f));
  if (!fc)
    machine_fail(this, str_f("Import file '%s' not found.", f));

  char *fid = str_left(fc, -4);
  Symbol *ssource = symbol_new_id(fid, symbol_name(source));

  if (!imports_is_on_way(ssource) && !imports_get(ssource)) {
    imports_put_on_way(ssource);

    Reader *r = reader_new(fid, file_read(fc), 1, 0);
    machine_isolate_process(
      fc, this->pmachines, reader_process(r)
    );

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

  EACHL(list_reverse(this->pmachines), Machine, m) {
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

static Machine *cprocess (Machine *m) {
  fails_register_machine(m);

  Symbol *module = NULL;
  Heap *moduleh = NULL;
  Symbol *sym = NULL;
  EACH_IX(token_list(m->prg), Token, tk, ix) {
    m->ix = ix;

    Token *t;
    if (sym) {
      if (module) {
        t = heap_get(moduleh, sym);
        if (t) {
          if (token_type(tk) == token_SYMBOL) {
            char *symbol = symbol_name(token_symbol(tk));
            if (str_eq(symbol, "="))
              machine_fail(m, "Import symbols can not be set");
            if (str_eq(symbol, "&")) {
              arr_push(m->stack, t);
              module = NULL;
              sym = NULL;
              continue;
            }
          }
        } else {
          machine_fail(m, str_f(
            "Symbol '%s %s' not found", symbol_name(module), symbol_name(sym)
          ));
        }
      } else {
        t = heap_get(m->heap, sym);
        if (!t) t = heap_get(imports_base(), sym);
        if (t) {
          if (token_type(tk) == token_SYMBOL) {
            char *symbol = symbol_name(token_symbol(tk));
            if (str_eq(symbol, "=")) {
              heap_add(m->heap, sym, machine_pop(m));
              sym = NULL;
              continue;
            }
            if (str_eq(symbol, "&")) {
              arr_push(m->stack, t);
              sym = NULL;
              continue;
            }
          }
        } else {
          if (token_type(tk) == token_SYMBOL) {
            char *symbol = symbol_name(token_symbol(tk));
            if (str_eq(symbol, "=")) {
              heap_add(m->heap, sym, machine_pop(m));
              sym = NULL;
              continue;
            }
          }
          // List<Machine>
          List *pms = m->pmachines;
          for (;;) {
            Machine *mch = list_head(pms);
            if (*mch->sourcex) break;
            t = heap_get(mch->heap, sym);
            if (t) break;
            pms = list_tail(pms);
          }

          if (t) {
            if (token_type(tk) == token_SYMBOL) {
              char *symbol = symbol_name(token_symbol(tk));
              if (str_eq(symbol, "&")) {
                arr_push(m->stack, t);
                module = NULL;
                sym = NULL;
                continue;
              }
            }
          } else {
            // List <Machines>
            List *pms = m->pmachines;
            while (!*((Machine *)list_head(pms))->sourcex) pms = list_tail(pms);
            char *msg = "";
            if (heap_get(((Machine *)list_head(pms))->heap, sym))
              msg = "\n(Top symbols can no be referenced out of top scope. "
                    "Use 'this'.)";
            machine_fail(m, str_f(
              "Symbol '%s' not found%s", symbol_name(sym), msg
            ));
          }
        }
      }

      char sname = *symbol_name(sym);
      arr_push(m->stack, t);
      if (
        token_type(t) == token_LIST &&
        (sname > 'Z' || sname < 'A')
      ) {
        m->ix--;
        run(m);
        m->ix++;
      }
      module = NULL;
      sym = NULL;
    }

    if (module) {
      if (token_type(tk) != token_SYMBOL)
        machine_fail(m, str_f(
          "Expected symbol of module '%s'", symbol_name(module)
        ));
      sym = token_symbol(tk);
      continue;
    }

    if (token_type(tk) == token_SYMBOL) {
      Symbol *symbol = token_symbol(tk);
      char *name = symbol_name(symbol);

      if (str_eq(name, "nop")) continue;
      else if (str_eq(name, "eval")) eval(m);
      else if (str_eq(name, "run")) run(m);
      else if (str_eq(name, "mrun")) mrun(m);
      else if (str_eq(name, "else") || str_eq(name, "elif"))
        machine_push(m, tk);
      else if (str_eq(name, "if")) sif(m);
      else if (str_eq(name, "break")) {
        arr_push(m->stack, tk);
        fails_unregister_machine();
        return m;
      }
      else if (str_eq(name, "loop")) loop(m);
      else if (str_eq(name, "while")) swhile(m);
      else if (str_eq(name, "for")) sfor(m);
      else if (str_eq(name, "import")) import(m);
      else if (str_eq(name, "assert")) sassert(m);
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

  if (sym) {
    Token *t;
    if (module) {
      t = heap_get(moduleh, sym);
      if (!t) {
        machine_fail(m, str_f(
          "Symbol '%s %s' not found", symbol_name(module), symbol_name(sym)
        ));
      }
      module = NULL;
    } else {
      t = heap_get(m->heap, sym);
      if (!t) t = heap_get(imports_base(), sym);
      if (!t) {
        // List<Machine>
        List *pms = m->pmachines;
        for (;;) {
          Machine *mch = list_head(pms);
          if (*mch->sourcex) break;
          t = heap_get(mch->heap, sym);
          if (t) break;
          pms = list_tail(pms);
        }
      }
      if (!t) {
        machine_fail(m, str_f(
          "Symbol '%s' not found", symbol_name(sym)
        ));
      }
    }

    arr_push(m->stack, t);
    char sname = *symbol_name(sym);
    sym = NULL;
    if (
      token_type(t) == token_LIST &&
      (sname > 'Z' || sname < 'A')
    ) run(m);
  } else if (module) {
    machine_fail(m, str_f(
      "Expected symbol of module '%s'", symbol_name(module)
    ));
  }

  fails_unregister_machine();
  return m;
}

// pmachines is List<Machine>
Machine *machine_process (char *source, List *pmachines, Token *prg) {
  Machine *m = MALLOC(Machine);
  m->sourcex = source;
  m->pmachines = list_cons(pmachines, m);
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
  m->sourcex = source;
  m->pmachines = list_cons(pmachines, m);
  m->stack = arr_new();
  m->heap = heap_new();
  m->prg = prg;
  m->ix = 0;

  if (*source)
    imports_add(symbol_new_id(str_left(source, -4), "this"), m->heap);

  return cprocess(m);
}
