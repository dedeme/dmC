// Copyright 03-Aug-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Heap with Symbols layers.

#ifndef READ_SYM_HEAP_H
  #define READ_SYM_HEAP_H

#include "read/symbol.h"

///
typedef struct read_md_heap SymHeap;

/// Constructor
SymHeap *sym_heap_new (void);

/// Creates a new SymHeap with sym added.
SymHeap *sym_heap_add(Symbol *sym);

#endif
