// Copyright 16-Apr-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Immutable class of a kut function.

#ifndef FUNCTION_H
  #define FUNCTION_H

#include "stat.h"
#include "heap0.h"
#include "heaps.h"
#include "imports.h"
#include "kut/arr.h"
#include "kut/map.h"

typedef struct function_Function Function;

/// Constructor
///   pars: Arr<char>. Symbols array.
///   stat: Statement.
Function *function_new (Arr *pars, StatCode *stat);

/// Returns 'this' imports.
Imports *function_get_imports (Function *this);

/// Returns the heap0.
Heap0 *function_get_heap0 (Function *this);

/// Returns Arr<Heap> with 'this' heaps.
Heaps *function_get_heaps (Function *this);

/// Returns an Arr<char> with function parameters.
Arr *function_get_pars (Function *this);

/// Returns the function statement.
StatCode *function_get_stat (Function *this);

///
char *function_to_str (Function *this);

#endif
