// Copyright 29-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Immutable container of Heap.

#ifndef HEAPS_H
  #define HEAPS_H

#include "heap.h"

///
typedef struct heaps_Heaps Heaps;

/// Creates Heaps with a Heap.
Heaps *heaps_new (void);

/// Adds 'heap' on front of 'this'.
Heaps *heaps_add (Heaps *this, Heap *heap);

/// Adds a symbol in the top heap of 'this'.
/// Returns TRUE if 'symbol' could be added and FALSE otherwise.
int heaps_add_symbol (Heaps *this, char *symbol);

/// Returns TRUE if symbol is in the top Heap of 'heaps'.
int heaps_is_duplicate (Heaps *this, char *symbol);

/// Returns TRUE if symbol 'sym' is found or FALSE if it is not.
int heaps_contains (Heaps *this, char *symbol);

#endif
