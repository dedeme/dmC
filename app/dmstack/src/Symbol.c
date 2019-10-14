// Copyright 05-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "Symbol.h"

struct symbol_SymbolKv {
  Symbol key;
  Symbol value;
};

// Arr<char>
Arr *syms = NULL;

void symbol_init (void) {
  syms = arr_new();
  REPEAT(symbol_SYSTEM_COUNT) {
    arr_push(syms, "");
  }_REPEAT

  arr_set(syms, symbol_IMPORT, "import");
  arr_set(syms, symbol_IF, "if");
  arr_set(syms, symbol_ELIF, "elif");
  arr_set(syms, symbol_ELSE, "else");
  arr_set(syms, symbol_BREAK, "break");
  arr_set(syms, symbol_EQUALS, "=");
  arr_set(syms, symbol_FUNCTION, "=>");
  arr_set(syms, symbol_AMPERSAND, "&");
  arr_set(syms, symbol_NOP, "nop");
  arr_set(syms, symbol_EVAL, "eval");
  arr_set(syms, symbol_RUN, "run");
  arr_set(syms, symbol_MRUN, "mrun");
  arr_set(syms, symbol_DATA, "data");
  arr_set(syms, symbol_SYNC, "sync");
  arr_set(syms, symbol_LOOP, "loop");
  arr_set(syms, symbol_WHILE, "while");
  arr_set(syms, symbol_FOR, "for");
  arr_set(syms, symbol_ASSERT, "assert");
  arr_set(syms, symbol_EXPECT, "expect");
  arr_set(syms, symbol_THIS, "this");
  arr_set(syms, symbol_STACK, "= @");
  arr_set(syms, symbol_STACK_CHECK, "= @?");

  arr_set(syms, symbol_PLUS, "+");
  arr_set(syms, symbol_TO_STR, "toStr");
  arr_set(syms, symbol_REF_OUT, ">>");

  arr_set(syms, symbol_BLOB_, "= Blob");
  arr_set(syms, symbol_THREAD_, "= Thread");
  arr_set(syms, symbol_ITERATOR_, "= Iterator");
  arr_set(syms, symbol_FILE_, "= File");
  arr_set(syms, symbol_ISERVER_, "= Iserver");
  arr_set(syms, symbol_ISERVER_RQ_, "= IserverRq");
}

Symbol symbol_new (char *name) {
  EACH_IX(syms, char, s, ix) {
    if (str_eq(s, name)) return ix;
  }_EACH
  arr_push(syms, name);
  return arr_size(syms) - 1;
}

int symbol_eq (Symbol this, Symbol other) {
  return this == other;
}

char *symbol_to_str (Symbol this) {
  return arr_get(syms, this);
}

SymbolKv *symbolKv_new (Symbol key, Symbol value) {
  SymbolKv *this = MALLOC(SymbolKv);
  this->key = key;
  this->value = value;
  return this;
}

Symbol symbolKv_key (SymbolKv *this) {
  return this->key;
}

Symbol symbolKv_value (SymbolKv *this) {
  return this->value;
}
