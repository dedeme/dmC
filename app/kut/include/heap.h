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

/// Returns this as a Map<Exp> whose keys are the defined symbols.
/// Example:
///   Exp *e = opt_get(map_get(heap_get(h), key));
///   if (e) {
///    ...
///   }
Map *heap_get (Heap *this);

/// Adds a new symbol to 'this' and returs TRUE. If the symbol already exists,
/// does nothing and returns FALSE.
int heap_add (Heap *this, char *symbol, Exp *exp);

/// Returns the expression Opt<Exp> matching 'symbol', searching in heaps from
/// heaps[0] to heaps[end].
/// If the symbol is not found, it returns 'opt_none()'.
///   heaps: Arr<Heap>.
///   symbol: Symbol to search.
Opt *heap_get_exp (Arr *heaps, char *symbol);

/// Returns a new Arr<Heap> adding 'this' to the beginning (as new_heaps[0]).
///   this: The head to add.
///   heaps: Arr<Heap>
Arr *heap_add_to_arr (Heap *this, Arr *heaps);

#endif
