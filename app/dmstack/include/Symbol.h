// Copyright 05-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Token 'Symbol'.

#ifndef SYMBOL_H
  #define SYMBOL_H

#include "dmc/async.h"

///
typedef struct symbol_Symbol Symbol;

/// Creates a new Symbo with name 'name' and machine and ix with value '-1'.
Symbol *symbol_new (char *name);

/// Returns the name of 'this'
char *symbol_name (Symbol *this);

/// Returns the symbol hash.
int symbol_hash (Symbol *this);

///
Symbol *symbol_clone (Symbol *this);

///
int symbol_eq (Symbol *this, Symbol *other);

///
char *symbol_to_str (Symbol *this);

#endif
