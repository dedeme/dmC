// Copyright 05-Oct-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Utility to read tokens.
/// Every function try to read a type and raises a fail if it fails.

#ifndef TK_H
  #define TK_H

#include "dmc/async.h"
#include "Machine.h"

///
Int tk_int (Machine *m, Token *t);

///
double tk_float (Machine *m, Token *t);

///
char *tk_string (Machine *m, Token *t);

/// Returns Arr<Token>
Arr *tk_list (Machine *m, Token *t);

///
Symbol tk_symbol (Machine *m, Token *t);

///
void *tk_native (Machine *m, Token *t, Symbol sym);

///
Int tk_pop_int (Machine *m);

///
double tk_pop_float (Machine *m);

///
char *tk_pop_string (Machine *m);

/// Returns Arr<Token>
Arr *tk_pop_list (Machine *m);

///
Symbol tk_pop_symbol (Machine *m);

///
void *tk_pop_native (Machine *m, Symbol sym);

///
Int tk_peek_int (Machine *m);

///
double tk_peek_float (Machine *m);

///
char *tk_peek_string (Machine *m);

/// Returns Arr<Token>
Arr *tk_peek_list (Machine *m);

///
Symbol tk_peek_symbol (Machine *m);

///
void *tk_peek_native (Machine *m, Symbol sym);

#endif
