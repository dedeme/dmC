// Copyright 02-Oct-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "types.h"
#include "tk.h"

enum types_Check { types_PUT, types_REMOVE, types_CHECK, types_FAIL };

static char *check_to_str (enum types_Check check) {
  switch (check) {
    case types_PUT: return "@+";
    case types_REMOVE: return "@-";
    case types_CHECK: return "@?";
    case types_FAIL: return "@";
  }
  EXC_ILLEGAL_STATE("switch not exhaustive");
  return NULL; // not reachable.
}

static char *typetk (Token *tk) {
  switch (token_type(tk)) {
    case token_INT: return "i";
    case token_FLOAT: return "f";
    case token_STRING: return "s";
    case token_SYMBOL: return token_symbol(tk) == symbol_STACK_STOP ? "|" : "y";
    case token_LIST: return "l";
    case token_NATIVE: {
      Symbol sym = token_native_symbol(tk);
      switch (sym) {
        case symbol_MAP_ : return "m";
        case symbol_REF_ : return "r";
        case symbol_OPTION_ : return "o";
        case symbol_EITHER_ : return "e";
        default: return str_f("<%s>", symbol_to_str(sym));
      }
    }
  }
  EXC_ILLEGAL_STATE("switch not exhaustive");
  return NULL; // not reachable.
}

static char *dots (char *type, char *ok) {
  if (strlen(type) - strlen(ok) > 1) return "...";
  return "";
}

static void fail (
  enum types_Check check, Machine *m, char *type, char *ok, Token *tk
) {
  machine_fail(m, str_f(
    "Expected '%s%s'. Actual '%s%s%s%s'",
    check_to_str(check), type,
    check_to_str(check), dots(type, ok), typetk(tk), ok
  ));
}

static void wrong (enum types_Check check, Machine *m, char *type, char *ok) {
  machine_fail(m, str_f(
    "Wrong type '%s%s' ('%s%s~%s')",
    check_to_str(check), type, check_to_str(check), dots(type, ok), ok
  ));
}

// stack is Arr<Token>
static int check (
  Machine *m, char *type, enum types_Check check
) {
  // Arr<Token>
  Arr *stack = machine_stack(m);
  int ix = arr_size(stack);
  char *p = type + strlen(type);

  for (;;) {
    if (p == type) break;
    if (!ix)
      machine_fail(m, str_f(
        "Expected '@%s', found '@[]%s'", type, p
      ));

    --ix;
    --p;
    Token *tk = arr_get(stack, ix);
    int f = 1;
    switch (*p) {
      case 'i': if (token_type(tk) == token_INT) f = 0; break;
      case 'f': if (token_type(tk) == token_FLOAT) f = 0; break;
      case 's': if (token_type(tk) == token_STRING) f = 0; break;
      case '|':
        if (
          token_type(tk) == token_SYMBOL &&
          token_symbol(tk) == symbol_STACK_STOP
        ) {
          // check === CHECK fails when not debug.
          if (p == type && check != types_CHECK) f = 0;
          else wrong(check, m, type, p);
        }
        break;
      case 'y':
        if (
          token_type(tk) == token_SYMBOL &&
          token_symbol(tk) != symbol_STACK_STOP
        ) f = 0;
        break;
      case 'l': if (token_type(tk) == token_LIST) f = 0; break;
      case 'm':
        if (
          token_type(tk) == token_NATIVE &&
          token_native_symbol(tk) == symbol_MAP_
        ) f = 0;
        break;
      case 'r':
        if (
          token_type(tk) == token_NATIVE &&
          token_native_symbol(tk) == symbol_REF_
        ) f = 0;
        break;
      case 'o':
        if (
          token_type(tk) == token_NATIVE &&
          token_native_symbol(tk) == symbol_OPTION_
        ) f = 0;
        break;
      case 'e':
        if (
          token_type(tk) == token_NATIVE &&
          token_native_symbol(tk) == symbol_EITHER_
        ) f = 0;
        break;
      case '>': {
        char *tmp = p;
        while (p > type) {
          if (*--p == '<') break;
        }
        if (*p != '<') wrong(check, m, type, tmp);
        Symbol sym = symbol_new(str_f("= %s", str_sub(p, 1, tmp - p)));
        if (
          token_type(tk) == token_NATIVE &&
          token_native_symbol(tk) == sym
        ) f = 0;
        else p = tmp;
        break;
      }
      case '*': f = 0; break;
      default: wrong(check, m, type, p);
    }

    if (f) {
      if (check == types_CHECK) return 0;
      fail(check, m, type, p + 1, tk);
    }
  }

  if (check == types_PUT)
    arr_insert(stack, ix, token_new_symbol(symbol_STACK_STOP));
  else if (check == types_REMOVE)
    arr_remove(stack, ix);

  return 1;
}

void types_fail (Machine *m) {
  char *t = symbol_to_str(token_symbol(*arr_start(tk_pop_list(m))));
  check(m, t, types_FAIL);
}

void types_check (Machine *m) {
  char *t = symbol_to_str(token_symbol(*arr_start(tk_pop_list(m))));
  int checked = check(m, t, types_CHECK);
  machine_push(m, token_new_int(checked));
}

void types_open_fail (Machine *m) {
  char *t = symbol_to_str(token_symbol(*arr_start(tk_pop_list(m))));
  check(m, t, types_PUT);
}

void types_close_fail (Machine *m) {
  char *t = symbol_to_str(token_symbol(*arr_start(tk_pop_list(m))));
  check(m, str_f("%c%s", '|', t), types_REMOVE);
}
