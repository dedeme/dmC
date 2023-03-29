// Copyright 14-Mar-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Code runner.

#ifndef RUNNER_RUNNER_H
  #define RUNNER_RUNNER_H

#include "kut/map.h"
#include "heap0.h"
#include "stat.h"

/// Run an array of statement.
///   stack_trace: Arr<StatCode>. Current stack trace when calling
///   imports: Map<int>. Module imports.
///   hp0: Module heap of not solved symbols.
///   hps: Arr<Heap>. Module heaps of solved symbols.
///   code: Arr<StatCode>. Statements Array to run.
Exp *runner_run(
  Arr *stack_trace, Map *imports, Heap0 *hp0, Arr *heaps, Arr *code
);

/// Run a single statement.
///   stack_trace: Arr<StatCode>. Current stack trace when calling
///   imports: Map<int>. Module imports.
///   hp0: Module heap of not solved symbols.
///   hps: Arr<Heap>. Module heaps of solved symbols.
///   stat: Statement to run.
Exp *runner_run_stat(
  Arr *stack_trace, Map *imports, Heap0 *hp0, Arr *heaps, StatCode *stat
);

#endif
