// Copyright 14-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Code runner.

#ifndef RUNNER_RUNNER_H
  #define RUNNER_RUNNER_H

#include "kut/map.h"
#include "heap0.h"
#include "stat.h"
#include "heaps.h"
#include "bfunction.h"
#include "runner/stack.h"

/// Run an array of statement.
///   stack_trace: Current stack trace when calling
///   imports: Map<int>. Module imports.
///   hp0: Module heap of not solved symbols.
///   hps: Heaps of solved symbols.
///   code: Arr<StatCode>. Statements Array to run.
Exp *runner_run(
  Stack *stack_trace, Map *imports, Heap0 *hp0, Heaps *heaps, Arr *code
);

/// Run a single statement.
///   stack_trace: Current stack trace when calling
///   imports: Map<int>. Module imports.
///   hp0: Module heap of not solved symbols.
///   hps: Heaps of solved symbols.
///   stat: Statement to run.
Exp *runner_run_stat(
  Stack *stack_trace, Map *imports, Heap0 *hp0, Heaps *heaps, StatCode *stat
);

/// Run 'fn'.
/// See mod_arr.c where an intesive use of this function is done.
///   exp: An expression of type function or <bfunction>. If 'exp' is not
///        of such type, an EXC_KUT is thrown.
///   fn: Function to run passing as argument (bf) the function in 'exp'
///       converted to <bfunction> or the <bfunction> in 'exp'.
Exp *runner_fn (Exp *exp, Exp *(fn)(Bfunction bf));

#endif
