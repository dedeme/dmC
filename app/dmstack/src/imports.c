// Copyright 12-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "imports.h"
#include "primitives.h"
#include "Machine.h"
#include "Lib.h"

// Map<Heap>
static Lib *imports = NULL;
// Arr<Symbol>
static Arr *onway = NULL;
static Heap *base = NULL;
static char *mprimitives[] = {
  "str", "lst", "stk", "wrap", "sys", "map", "obj", "js", "path", "file",
  "time", "cryp", "b64", "it", "float", "int", "math",
  "blob", "iserver", "clock", NULL
};

void imports_init () {
  base = heap_new();
  EACH(opt_get(primitives_module("")), Kv, kv) {
    heap_add(base, symbol_new(kv_key(kv)), kv_value(kv));
  }_EACH

  onway = arr_new();

  imports = lib_new();

  void loadp (char *module) {
    Heap *h = heap_new();
    EACH(opt_get(primitives_module(module)), Kv, kv) {
      heap_add(h, symbol_new(kv_key(kv)), kv_value(kv));
    }_EACH
    lib_add(imports, symbol_new(module), h);
  }

  char **p = mprimitives;
  while (*p) loadp(*p++);
}

void imports_put_on_way (Symbol *key) {
  arr_push(onway, key);
}

void imports_quit_on_way (Symbol *key) {
  int fn (Symbol *s) { return !symbol_eq(s, key); }
  arr_filter_in(onway, (FPRED)fn);
}

int imports_is_on_way (Symbol *key) {
  int fn (Symbol *s) { return symbol_eq(s, key); }
  return arr_index(onway, (FPRED)fn) != -1;
}

void imports_add(Symbol *key, Heap *heap) {
  lib_add(imports, key, heap);
}

Heap *imports_get (Symbol *key) {
  return lib_get(imports, key);
}

Heap *imports_base (void) {
  return base;
}

// Kv<Symbol>
Kv *imports_read_symbol (Token *tk) {
  if (token_type(tk) == token_STRING) {
    char *s = token_string(tk);
    return kv_new("", symbol_new_id(s, path_name(s)));
  } else if (token_type(tk) != token_LIST) {
    return kv_new(
      str_f(
        "Stack pop: Expected token of type ['%s' or '%s'], found type '%s'",
        token_type_to_str(token_STRING),
        token_type_to_str(token_LIST),
        token_type_to_str(token_type(tk))
      ),
      symbol_new("Error")
    );
  } else {
    // Arr<Token>
    Arr *a = token_list(tk);
    if (arr_size(a) != 2)
      return kv_new(
        str_f(
          "List %s\nExpected size:2, actual size: %d",
          token_to_str(token_new_list(0, a)), arr_size(a)
        ),
        symbol_new("Error")
      );
    Token *tk1 = arr_get(a, 0);
    if (token_type(tk1) != token_STRING)
      return kv_new(
        "Expected String as first list element in import", symbol_new("Error")
      );
    Token *tk2 = arr_get(a, 1);
    if (token_type(tk2) != token_SYMBOL)
      return kv_new(
        "Expected Symbol as second list element in import", symbol_new("Error")
      );

    return kv_new(
      "",
      symbol_new_id(token_string(tk1), symbol_name(token_symbol(tk2)))
    );
  }
}
