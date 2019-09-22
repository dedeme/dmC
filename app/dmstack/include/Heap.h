// Copyright 05-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Variables heap.

#ifndef HEAP_H
  #define HEAP_H

#include "dmc/async.h"
#include "Token.h"

///
typedef struct heap_Entry HeapEntry;

///
Symbol *heapEntry_symbol (HeapEntry *this);

///
Token *heapEntry_token (HeapEntry *this);

///
typedef struct heap_Heap Heap;

///
Heap *heap_new (void);

/// Adds a new element to 'this'
void heap_add (Heap *this, Symbol *s, Token *tk);

/// Returns a token o NULL
Token *heap_get (Heap *this, Symbol *s);

/// Arr<HeapEntry>
Arr *heap_entries (Heap *this);

#endif
