// Copyright 11-Sep-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Immutable class of a kut function.

#ifndef FUNCTION_H
  #define FUNCTION_H

#include "iarr.h"
#include "stat.h"
#include "heap0.h"
#include "heaps.h"
#include "imports.h"
#include "kut/arr.h"
#include "kut/map.h"

typedef struct function_Function Function;

/// Constructor
///   pars: Symbols array.
///   stat: Statement.
Function *function_new (Iarr *pars, StatCode *stat);

/// Returns 'this' imports.
Imports *function_get_imports (Function *this);

/// Returns the heap0.
Heap0 *function_get_heap0 (Function *this);

/// Returns Arr<Heap> with 'this' heaps.
Heaps *function_get_heaps (Function *this);

/// Returns function parameters.
Iarr *function_get_pars (Function *this);

/// Returns the function statement.
StatCode *function_get_stat (Function *this);

/// Returns a new Function with a new context.
///   this: Function.
///   imports: Map<int>. New function imports.
///   heap0: New heap0.
///   heaps: New heaps.
Function *function_set_context (
  Function *this, Imports *imports, Heap0 *heap0, Heaps *heaps
);

/// Run 'this' and returns an expression which can be empty, break, continue or
/// normal.
///   this: Function to run.
///   pars: Arr<Exp> with solved parameters.
Exp *function_run (Function *this, Arr *params);

/// Returns 'this' as expression of type 'pr'.
///   this: Function.
///   params: Arr<Exp>. Paramter values.
Exp *function_to_exp (Function *this, Arr *params);

///
char *function_to_str (Function *this);

#endif
