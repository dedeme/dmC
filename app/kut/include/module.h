// Copyright 07-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Immutable class with code read from a .kut file, its imports and heaps.

#ifndef MODULE_H
  #define MODULE_H

#include "heap0.h"
#include "heap.h"

typedef struct module_Module Module;

/// Constructor.
///   imports: Map<int>. Link every import symbol with the matching file index.
///   hp0    : Module heap.
///   code   : Arr<Statement>. Statement list.
Module *module_new (Map *imports, Heap0 *hp0, Arr *code);

/// Returns Map<int>
Map *module_get_imports(Module *this);

///
Heap0 *module_get_heap0(Module *this);

///
Heap *module_get_heap(Module *this);

/// Returns Arr<StatCode>
Arr *module_get_code(Module *this);

#endif
