// Copyright 02-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// File heap.
/// Store information about the file definition line.

#ifndef HEAP0_H
  #define HEAP0_H

#include "exp.h"

///
typedef struct heap0_Heap0Entry Heap0Entry;

/// Returns the heap0 entry symbol.
int heap0_entry_symbol (Heap0Entry *entry);

/// Returns the number of line where the heap0 symbol was defined.
int heap0_entry_nline (Heap0Entry *entry);

/// Returns the expression associate to the heap0 symbol.
Exp *heap0_entry_exp (Heap0Entry *entry);

///
typedef struct heap0_Heap0 Heap0;

/// Constructor.
Heap0 *heap0_new (void);

/// Returns a new Heap0 duplicate of 'this' and with the symbol of 'entry'
/// marked as cyclic.
/// Used to prevent cyclic references.
Heap0 *heap0_new_cyclic (Heap0 *this, Heap0Entry *entry);

/// Returns this as an Arr<Heap0Entry>.
Arr *heap0_get_array (Heap0 *this);

/// Adds a new symbol to 'this' and returs TRUE. If the symbol already exists,
/// does nothing and returns FALSE.
int heap0_add (Heap0 *this, int symbol, int nline, Exp *exp);

/// Returns an Opt<Heap0Entry> with the entry with symbol 'sym' or 'opt_none'
/// if such symbol is not found.
Opt *heap0_get(Heap0 *this, int sym);

#endif
