// Copyright 05-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Token 'Symbol'.

#ifndef SYMBOL_H
  #define SYMBOL_H

#include "dmc/async.h"

///
typedef struct symbol_Symbol Symbol;

/// Creates a new Symbol with an id different to name.
Symbol *symbol_new_id (char *id, char *name);

/// Creates a new Symbol with equals id and name.
Symbol *symbol_new (char *name);

/// Creates a new Symbol with equals id and name and hash == 0. This
/// constructor is intended only to use with pointers.
Symbol *symbol_new_pointer (char *name);

/// Returns the name of 'this'
char *symbol_name (Symbol *this);

/// Returns the id of 'this'
char *symbol_id (Symbol *this);

/// Returns the symbol hash.
int symbol_hash (Symbol *this);

///
Symbol *symbol_clone (Symbol *this);

///
int symbol_eq (Symbol *this, Symbol *other);

///
char *symbol_to_str (Symbol *this);

#endif
