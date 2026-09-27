// Copyright 11-Sep-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Immutable container of Heap.

#ifndef HEAPS_H
  #define HEAPS_H

#include "heap.h"

///
typedef struct heaps_Heaps Heaps;

/// Creates Heaps with a Heap.
Heaps *heaps_new (Heap *hp);

/// Adds 'heap' on front of 'this'.
Heaps *heaps_add (Heaps *this, Heap *heap);

/// Adds a symbol solved as 'exp' in the top heap of 'this'.
/// Returns TRUE if 'symbol' could be added and FALSE otherwise.
int heaps_add_symbol (Heaps *this, int symbol, Exp *exp);

/// Returns TRUE if symbol is in the top Heap of 'heaps'.
int heaps_is_duplicate (Heaps *this, int symbol);

/// Returns TRUE if 'this' has only one heap.
int heaps_is_initial (Heaps *this);

/// Search for a symbol in 'this' and returns a Tp<Opt<Exp>, Heap>:
///   tp_e1: It is an empty option if 'symbol' is not found. Otherwhise returns
///          the pointed expression by 'symbol'.
///   to_e2: It is the bottom heap (corresponding to heap0) when tp_e1 is
///          opt_none, and NULL otherwise.
Tp *heaps_get (Heaps *this, int symbol);

#endif
