// Copyright 16-Sept-2019 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Imports library

#ifndef LIB_H
  #define LIB_H

#include "dmc/async.h"
#include "Heap.h"

///
typedef struct lib_Entry LibEntry;

///
Symbol *libEntry_symbol (LibEntry *this);

///
Heap *libEntry_heap (LibEntry *this);

///
typedef struct lib_Lib Lib;

///
Lib *lib_new (void);

/// Adds a new element to 'this'
void lib_add (Lib *this, Symbol *s, Heap *heap);

/// Returns a Heap o NULL
Heap *lib_get (Lib *this, Symbol *s);

/// Arr<LibEntry>
Arr *lib_entries (Lib *this);

#endif

