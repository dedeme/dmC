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

void imports_init () {
  base = heap_new();
  primitives_add_base(base);

  onway = arr_new();

  imports = lib_new();
  primitives_add_lib(imports);
}

void imports_put_on_way (Symbol key) {
  Symbol *s = ATOMIC(sizeof(Symbol));
  *s = key;
  arr_push(onway, s);
}

void imports_quit_on_way (Symbol key) {
  int fn (Symbol *s) { return !symbol_eq(*s, key); }
  arr_filter_in(onway, (FPRED)fn);
}

int imports_is_on_way (Symbol key) {
  int fn (Symbol *s) { return symbol_eq(*s, key); }
  return arr_index(onway, (FPRED)fn) != -1;
}

void imports_add(Symbol key, Heap *heap) {
  lib_add(imports, key, heap);
}

Heap *imports_get (Symbol key) {
  return lib_get(imports, key);
}

Heap *imports_base (void) {
  return base;
}

// Kv<SymbolKv>
Kv *imports_read_symbol (Token *tk) {
  if (token_type(tk) == token_STRING) {
    char *s = token_string(tk);
    Symbol sym = symbol_new(s);
    return kv_new("", symbolKv_new(-1, sym));
  } else if (token_type(tk) != token_LIST) {
    return kv_new(
      str_f(
        "Stack pop: Expected token of type ['%s' or '%s'], found type '%s'",
        token_type_to_str(token_STRING),
        token_type_to_str(token_LIST),
        token_type_to_str(token_type(tk))
      ),
      NULL
    );
  } else {
    // Arr<Token>
    Arr *a = token_list(tk);
    if (arr_size(a) != 2)
      return kv_new(
        str_f(
          "List %s\nExpected size:2, actual size: %d",
          token_to_str(token_new_list(a)), arr_size(a)
        ),
        NULL
      );
    Token *tk1 = arr_get(a, 0);
    if (token_type(tk1) != token_STRING)
      return kv_new(
        "Expected String as first list element in import", NULL
      );
    Token *tk2 = arr_get(a, 1);
    if (token_type(tk2) != token_SYMBOL)
      return kv_new(
        "Expected Symbol as second list element in import", NULL
      );

    Symbol sym = symbol_new(token_string(tk1));
    return kv_new("", symbolKv_new(token_symbol(tk2), sym));
  }
}
