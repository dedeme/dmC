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

void *tk_native (Machine *m, Token *t, Symbol sym) {
  if (token_type(t) != token_NATIVE) fails_type_in(m, token_NATIVE, t);
  if (token_native_symbol(t) != sym)
    machine_fail(m, str_f(
      "Expected pointer of type '%s', found one of type '%s'",
      symbol_to_str(sym), symbol_to_str(token_native_symbol(t))
    ));
  return token_native_pointer(t);
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

void *tk_pop_native (Machine *m, Symbol sym) {
  return tk_native(m, machine_pop(m), sym);
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

void *tk_peek_native (Machine *m, Symbol sym) {
  return tk_native(m, machine_peek(m), sym);
}
