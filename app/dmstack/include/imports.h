// Copyright 12-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Import cache.

#ifndef IMPORTS_H
  #define IMPORTS_H

#include "dmc/async.h"
#include "Machine.h"

///
void imports_init ();

/// Annotation of an import on way.
void imports_put_on_way (Symbol *key);

/// Removes nnotation of an import on way.
void imports_quit_on_way (Symbol *key);

/// Returns if an import is on way.
int imports_is_on_way (Symbol *key);

///
void imports_add (Symbol *key, Heap *heap);

/// Returns an import from library or NULL.
Heap *imports_get (Symbol *key);

/// Returns system heap.
Heap *imports_base (void);

/// Reads an import symbol. If reading fails return an
/// error message in 'key'. Otherwise key is "".
Kv *imports_read_symbol (Token *tk);

#endif
