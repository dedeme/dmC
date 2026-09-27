// Copyright 12-Sep-2025 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// Code runner.

#ifndef RUNNER_RUNNER_H
  #define RUNNER_RUNNER_H

#include "kut/map.h"
#include "heap0.h"
#include "stat.h"
#include "heaps.h"
#include "imports.h"
#include "bfunction.h"
#include "runner/stack.h"

/// Run an array of statement.
///   isTop: it is TRUE if the running code is in top of module.
///   stack_trace: Current stack trace when calling
///   imports: Module imports.
///   hp0: Module heap of not solved symbols.
///   hps: Heaps of solved symbols.
///   code: Arr<StatCode>. Statements Array to run.
Exp *runner_run(
  int isTop, Stack *stack_trace,
  Imports *imports, Heap0 *hp0, Heaps *heaps, Arr *code
);

/// Run a single statement.
///   stack_trace: Current stack trace when calling
///   imports: Module imports.
///   hp0: Module heap of not solved symbols.
///   hps: Heaps of solved symbols.
///   stat: Statement to run.
Exp *runner_run_stat(
  Stack *stack_trace, Imports *imports, Heap0 *hp0, Heaps *heaps, StatCode *stat
);

#endif
