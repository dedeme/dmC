// Copyright 02-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// File heap.
/// Store information about the file definition line.

#ifndef HEAP0_H
  #define HEAP0_H

#include "exp.h"

///
typedef struct heap0_Heap0Entry Heap0Entry;

/// Returns the number of line where the heap0 symbol was defined.
int heap0_entry_nline (Heap0Entry *entry);

/// Returns the expression associate to the heap0 symbol.
Exp *heap0_entry_exp (Heap0Entry *entry);

///
typedef struct heap0_Heap0 Heap0;

/// Constructor.
Heap0 *heap0_new (void);

/// Returns this as a Map<Heap0Entry> whose keys are the defined symbols.
/// Example:
///   Heap0Entry *e = opt_get(map_get(heap0_get(h), key));
///   if (e) {
///    int nline = heap0_entry_nline(e);
///     Exp *exp = heap0_entry_exp(e);
///   }
Map *heap0_get (Heap0 *this);

/// Adds a new symbol to 'this' and returs TRUE. If the symbol already exists,
/// does nothing and returns FALSE.
int heap0_add (Heap0 *this, char *symbol, int nline, Exp *exp);

#endif
