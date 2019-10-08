// Copyright 05-Oct-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include "tk.h"
#include "fails.h"

Int tk_int (Machine *m, Token *t) {
  if (token_type(t) != token_INT) fails_type_in(m, token_INT, t);
  return token_int(t);
}

double tk_float (Machine *m, Token *t) {
  if (token_type(t) != token_FLOAT) fails_type_in(m, token_FLOAT, t);
  return token_float(t);
}

char *tk_string (Machine *m, Token *t) {
  if (token_type(t) != token_STRING) fails_type_in(m, token_STRING, t);
  return token_string(t);
}

Arr *tk_list (Machine *m, Token *t) {
  if (token_type(t) != token_LIST) fails_type_in(m, token_LIST, t);
  return token_list(t);
}

Symbol tk_symbol (Machine *m, Token *t) {
  if (token_type(t) != token_SYMBOL) fails_type_in(m, token_SYMBOL, t);
  return token_symbol(t);
}

void *tk_pointer (Machine *m, Token *t, Symbol sym) {
  if (token_type(t) != token_LIST) fails_type_in(m, token_LIST, t);
  // Arr<Token>
  Arr *a = token_list(t);
  if (arr_size(a) != 2) fails_size_list(m, a, 2);
  Token *symtk = *arr_start(a);
  if (token_type(symtk) != token_SYMBOL) fails_type_in(m, token_SYMBOL, symtk);
  if (token_symbol(symtk) != sym)
    machine_fail(m, str_f(
      "Expected pointer of type '%s', found one of type '%s",
      symbol_to_str(sym), symbol_to_str(token_symbol(symtk))
    ));
  Token *p = *(arr_start(a) + 1);
  if (token_type(p) != token_POINTER) fails_type_in(m, token_POINTER, p);

  return (void *)token_int(p);
}

Int tk_pop_int (Machine *m) {
  return tk_int(m, machine_pop(m));
}

double tk_pop_float (Machine *m) {
  return tk_float(m, machine_pop(m));
}

char *tk_pop_string (Machine *m) {
  return tk_string(m, machine_pop(m));
}

Arr *tk_pop_list (Machine *m) {
  return tk_list(m, machine_pop(m));
}

Symbol tk_pop_symbol (Machine *m) {
  return tk_symbol(m, machine_pop(m));
}

void *tk_pop_pointer (Machine *m, Symbol sym) {
  return tk_pointer(m, machine_pop(m), sym);
}

Int tk_peek_int (Machine *m) {
  return tk_int(m, machine_peek(m));
}

double tk_peek_float (Machine *m) {
  return tk_float(m, machine_peek(m));
}

char *tk_peek_string (Machine *m) {
  return tk_string(m, machine_peek(m));
}

Arr *tk_peek_list (Machine *m) {
  return tk_list(m, machine_peek(m));
}

Symbol tk_peek_symbol (Machine *m) {
  return tk_symbol(m, machine_peek(m));
}

void *tk_peek_pointer (Machine *m, Symbol sym) {
  return tk_pointer(m, machine_peek(m), sym);
}
