// Copyright 02-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Functions heap.

#ifndef HEAP_H
  #define HEAP_H

#include "kut/opt.h"
#include "kut/map.h"
#include "exp.h"

///
typedef struct heap_Heap Heap;

/// Constructor.
Heap *heap_new (void);

/// Returns this as an Arr<char>.
Arr *heap_get_array (Heap *this);

/// Adds a new symbol to 'this' and returs TRUE. If the symbol already exists,
/// does nothing and returns FALSE.
int heap_add (Heap *this, char *symbol);

/// Returns TRUE if symbol 'sym' is found or FALSE if it is not.
int heap_contains(Heap *this, char *sym);

#endif
