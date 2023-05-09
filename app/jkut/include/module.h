// Copyright 07-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Immutable class with code read from a .kut file, its imports and heaps.

#ifndef MODULE_H
  #define MODULE_H

#include "heap0.h"
#include "imports.h"

typedef struct module_Module Module;

/// Constructor.
///   imports: Link every import symbol with the matching file index.
///   exports: Arr<char> with symbols to export.
///   hp0    : Module heap.
///   code   : Arr<Statement>. Statement list.
///   js     : Javascript code.
Module *module_new (
  Imports *imports, Arr *exports, Heap0 *hp0, Arr *code, char *js
);

///
Imports *module_get_imports (Module *this);

///
Heap0 *module_get_heap0 (Module *this);

/// Returns Arr<StatCode>
Arr *module_get_code (Module *this);

/// Returns the javascript code.
char *module_get_js (Module *this);

/// Returns TRUE if this contains the export 'exp'
int module_contains_export (Module *this, char *exp);

#endif
