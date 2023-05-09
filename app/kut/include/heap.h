// Copyright 02-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Functions heap.

#ifndef HEAP_H
  #define HEAP_H

#include "kut/opt.h"
#include "kut/map.h"
#include "exp.h"

///
typedef struct heap_HeapEntry HeapEntry;

/// Returns the heap entry symbol.
int heap_entry_symbol (HeapEntry *entry);

/// Returns the expression associate to the heap symbol.
Exp *heap_entry_exp (HeapEntry *entry);

///
typedef struct heap_Heap Heap;

/// Constructor.
Heap *heap_new (void);

/// Returns this as an Arr<Heap0Entry>.
Arr *heap_get_array (Heap *this);

/// Adds a new symbol to 'this' and returs TRUE. If the symbol already exists,
/// does nothing and returns FALSE.
int heap_add (Heap *this, int symbol, Exp *exp);

/// Returns an Opt<HeapEntry> with the entry with symbol 'sym' or 'opt_none'
/// if such symbol is not found.
Opt *heap_get(Heap *this, int sym);

#endif
